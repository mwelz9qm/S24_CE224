#include <chrono>
#include <iomanip>
#include <iostream>
#include <list>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <sstream>
#include <string>

using namespace std;
using namespace std::chrono;

class Transaction;

struct Block {
  string data;
  string timestamp;
  string previousHash;
  string hash;
  int nonce;
  list<Transaction> transactions;
};

class Transaction {
public:
  string fromAddress;
  string toAddress;
  int amount;

  Transaction(string fromAddress, string toAddress, int amount) {
    this->fromAddress = fromAddress;
    this->toAddress = toAddress;
    this->amount = amount;
  }
};

class blockchain {
private:
  int difficulty = 2; // Initialize difficulty level

  string calculateHash(const Block &block) {
    stringstream ss;
    ss << block.data << block.timestamp << block.previousHash << block.nonce;
    string input = ss.str();
    unsigned char hash[SHA256_DIGEST_LENGTH];

    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    EVP_DigestInit(ctx, EVP_sha256());
    EVP_DigestUpdate(ctx, input.c_str(), input.length());
    EVP_DigestFinal(ctx, hash, nullptr);
    EVP_MD_CTX_free(ctx);

    stringstream hashStream;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
      hashStream << hex << setw(2) << setfill('0') << static_cast<int>(hash[i]);
    }

    return hashStream.str();
  }

  Block mineBlock(Block block) {
    string target = string(difficulty, '0');

    auto startTime = high_resolution_clock::now();

    while (true) {
      block.nonce++;
      string hash = calculateHash(block);
      if (hash.substr(0, difficulty) == target) {
        block.hash = hash;
        auto endTime = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(endTime - startTime).count();

        cout << "Block mined in " << duration << " milliseconds with difficulty " << difficulty << "\nWith a Hash value of: " << block.hash << "\n\n" << endl;

        // Reset difficulty for the next block
        difficulty++; // Reset difficulty for the next block
        return block;
      }
    }
  }

public:
  list<Block> blocks;
  list<Transaction> pendingTransactions;
  int miningreward;
  string miningRewardsAddress = "your_address";

  void addBlock(string data) {
    Block block;
    block.data = data;

    this->pendingTransactions.clear();
    this->miningreward = 100;

    system_clock::time_point now = system_clock::now();
    time_t now_c = system_clock::to_time_t(now);
    stringstream ss;
    ss << put_time(gmtime(&now_c), "%Y-%m-%d %H:%M:%S");
    block.timestamp = ss.str();

    if (blocks.empty()) {
      block.previousHash = "000000000000000000000000000000000000000000000000000000000000000";
    } else {
      block.previousHash = blocks.back().hash;
    }
    block = mineBlock(block);
    blocks.push_back(block);

    cout << "Hash of the block: " << block.hash << endl;
    cout << "Previous hash of the block: " << block.previousHash << endl;
    cout << "Nonce used: " << block.nonce << endl;
  }

  bool is_block_valid(const Block &block, const Block &prevBlock) {
    return block.previousHash == prevBlock.hash;
  }

  void minePendingTransactions(string miningRewardsAddress) {
    Block newBlock;
    newBlock.data = "some data";

    system_clock::time_point now = system_clock::now();
    time_t now_c = system_clock::to_time_t(now);
    stringstream ss;
    ss << put_time(gmtime(&now_c), "%Y-%m-%d %H:%M:%S");
    newBlock.timestamp = ss.str();

    newBlock.previousHash = blocks.back().hash;
    newBlock.nonce = 0;

    newBlock.transactions = pendingTransactions;
    newBlock = mineBlock(newBlock);

    cout << "Block successfully mined" << endl;
    blocks.push_back(newBlock);
    pendingTransactions.clear();

    Transaction rewardTransaction("", miningRewardsAddress, miningreward);
    pendingTransactions.push_back(rewardTransaction);
  }

  void createTransaction(Transaction transaction) {
    this->pendingTransactions.push_back(transaction);
  }

  double getbalance(string address) {
    double balance = 0;

    for (const Block &block : blocks) {
      for (const Transaction &trans : block.transactions) {
        if (trans.fromAddress == address) {
          balance -= trans.amount;
        }

        if (trans.toAddress == address) {
          balance += trans.amount;
        }
      }
    }
    for (const Transaction &trans : pendingTransactions) {
      if (trans.fromAddress == address) {
        balance -= trans.amount;
      }
      if (trans.toAddress == address) {
        balance += trans.amount;
      }
    }

    return balance;
  }

  bool is_chain_valid() {
    auto it = blocks.begin();
    auto prevIt = it;
    ++it;
    while (it != blocks.end()) {
      if (it->previousHash != prevIt->hash) {
        return false;
      }
      ++it;
      ++prevIt;
    }
    return true;
  }
};
void printBorder() {
    cout << "--------------------------------------------------" << endl;
}
int main() {
  blockchain blockchain;
  blockchain.addBlock("Genesis Block");
  blockchain.addBlock("Second Block");

  cout << "Start the miner..." << endl;
  blockchain.minePendingTransactions("Alicia-address");

  cout << "Balance of Alicia is " << blockchain.getbalance("Alicia-address") << endl;

  blockchain.createTransaction(Transaction("address 1", "address 2", 30));
  blockchain.createTransaction(Transaction("address 2", "Alicia-address", 20));

  cout << "Starting the miner again..." << endl;
  blockchain.minePendingTransactions("Alicia-address");
  cout << "Balance of Alicia is " << blockchain.getbalance("Alicia-address");

  return 0;
}
