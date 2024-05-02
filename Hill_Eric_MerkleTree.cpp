#include <iostream>
#include <sstream>
#include <list>
#include <iomanip>
#include <chrono>
#include <string>
#include <vector>
#include <openssl/evp.h>

using namespace std;
using namespace chrono;

// Transaction class to create the leaf nodes, implemented after a block is mined -->> then it's hashed to the Merkle root
class Transaction {
public:
    string fromAddress;
    string toAddress;
    int amount;

    Transaction(string from, string to, int amt) : fromAddress(from), toAddress(to), amount(amt) {}

    string toString() const {
        stringstream ss;
        ss << fromAddress << toAddress << amount;
        return ss.str();
    }
};

// Common Block structure, added merkleRoot
class Block {
public:
    string previousHash;
    string hash;
    int nonce = 0;
    list<Transaction> transactions;
    string merkleRoot;
    time_t timestamp;
    int difficulty;

    Block(string prevHash, int diff) : previousHash(prevHash), difficulty(diff) {
        timestamp = system_clock::to_time_t(system_clock::now());
    }

    void addTransaction(const Transaction& transaction) {
        transactions.push_back(transaction);
    }

// Difficulty initialized, not modified here to increase leading zeros
    void mineBlock() {
        string target = string(difficulty, '0');
        while (hash.substr(0, difficulty) != target) {
            nonce++;
            hash = calculateHash();
            if (nonce % 500 == 0) {
                cout << "Mining attempt " << nonce << ", Current Hash: " << hash.substr(0, 20) << "..." << endl;
            }
        }
    }

// Added merkleRoot, hashes the transactions in the Merkle tree for the block and pushes to string
    string calculateMerkleRoot() {
        vector<string> hashes;
        for (const auto& tx : transactions) {
            hashes.push_back(sha256(tx.toString()));
        }

        while (hashes.size() > 1) {
            vector<string> newLayer;
            for (size_t i = 0; i < hashes.size(); i += 2) {
                if (i + 1 == hashes.size()) {
                    newLayer.push_back(hashes[i]);
                } else {
                    newLayer.push_back(sha256(hashes[i] + hashes[i + 1]));
                }
            }
            hashes = newLayer;
        }
        return hashes.empty() ? "" : hashes.front();
    }

// Generic implementation of SHA_256
    string calculateHash() {
        EVP_MD_CTX* ctx = EVP_MD_CTX_new();
        EVP_DigestInit_ex(ctx, EVP_sha256(), NULL);

        stringstream ss;
        ss << previousHash << nonce << merkleRoot << timestamp;
        string data = ss.str();
        EVP_DigestUpdate(ctx, data.c_str(), data.length());

        unsigned char hashOutput[EVP_MAX_MD_SIZE];
        unsigned int lengthOfHash = 0;
        EVP_DigestFinal_ex(ctx, hashOutput, &lengthOfHash);
        EVP_MD_CTX_free(ctx);

        stringstream hashStream;
        hashStream << hex << setfill('0');
        for (unsigned int i = 0; i < lengthOfHash; ++i) {
            hashStream << setw(2) << (int)hashOutput[i];
        }
        return hashStream.str();
    }

    static string sha256(const string& str) {
        EVP_MD_CTX* ctx = EVP_MD_CTX_new();
        EVP_DigestInit_ex(ctx, EVP_sha256(), NULL);
        EVP_DigestUpdate(ctx, str.c_str(), str.size());

        unsigned char hash[EVP_MAX_MD_SIZE];
        unsigned int lengthOfHash = 0;
        EVP_DigestFinal_ex(ctx, hash, &lengthOfHash);
        EVP_MD_CTX_free(ctx);

        stringstream ss;
        for (unsigned int i = 0; i < lengthOfHash; i++) {
            ss << hex << setw(2) << setfill('0') << (int)hash[i];
        }
        return ss.str();
    }
};

// Structuring blocks as linked list, difficulty can be modified here
class Blockchain {
private:
    list<Block> chain;
    int difficulty = 4; // Adjust the difficulty level as needed, anything above 5 takes 4-5 million attemps. 

// Genesis block starts with 0 for its hash value, no transactions are currently stored 
public:
    Blockchain() {
        Block genesis("0", difficulty);
        genesis.merkleRoot = genesis.calculateMerkleRoot();
        genesis.mineBlock();
        chain.push_back(genesis);
    }

// Manual addblock function. Can add more blocks in main
    void addBlock(Block& newBlock) {
        newBlock.previousHash = getLastHash();
        newBlock.merkleRoot = newBlock.calculateMerkleRoot();
        newBlock.mineBlock();
        chain.push_back(newBlock);
    }

    string getLastHash() const {
        return chain.back().hash;
    }

    int getDifficulty() const {
        return difficulty;
    }

// Displaying the info of the block
    void displayChain() {
        for (const auto& block : chain) {
            cout << "Prev Hash: " << block.previousHash << endl;
            cout << "Merkle Root: " << block.merkleRoot << endl;
            cout << "Block Hash: " << block.hash << endl;
            cout << "Nonce: " << block.nonce << endl;
            cout << "Transactions:" << endl;
            for (const auto& tx : block.transactions) {
                cout << "  From: " << tx.fromAddress << ", To: " << tx.toAddress << ", Amount: " << tx.amount << endl;
            }
            cout << endl;
        }
    }
};

// Removed the creation of new blocks via looping after implementing the Merkel root, was having trouble with access of private/public members. Genesis block doesn't have transactions stored, so the Merkle root will output nothing. But block 1 implements the transaction function and still validates current previous hash with the previous hash. 
int main() {
    Blockchain bc;
    Block block1(bc.getLastHash(), bc.getDifficulty());
    block1.addTransaction(Transaction("Alice", "Bob", 100));
    block1.addTransaction(Transaction("Charlie", "Dave", 50));
    bc.addBlock(block1);

    // Outputs the info of blocks in the chain
    bc.displayChain();
    return 0;
}
