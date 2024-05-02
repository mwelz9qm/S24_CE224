# Blockchain Implementation with Merkle Root

This repository contains my C++ implementation of a simple blockchain that now includes Merkle roots for increased security. This is an improvement on the base project, which was a very simple blockchain structure that successfully used SHA_256, but failed to securely store transactions; previously stored as a string linked to a block. The program now successfully validates between blocks and hashes the transactions ledgered in each block. 

## Features

- **Transaction Management**: Add transactions to blocks.
- **Block Mining**: Mine blocks with a difficulty level that requires the hash of the block to start with a certain number of zeros.
- **Merkle Root Calculation**: Calculate Merkle roots for each block to efficiently and securely verify the contents of transactions.
- **Chain Validation**: Validate the blockchain by ensuring that each block is linked to the previous block and that the data has not been tampered with.

## Merkle Root Implementation

The Merkle root is implemented in the `Block` class. It is calculated by recursively hashing pairs of transactions until only one hash remains. This final hash is known as the Merkle root. Each transaction is first converted to a string and hashed. These hashes are then paired and hashed together, and the process is repeated until there is only one hash left. 

The process of the code is as follows:

1. **Convert Transactions to Hashes**: Each transaction in a block is converted to a string and hashed using SHA-256.
2. **Pair and Hash**: The list of transaction hashes is then paired and hashed iteratively. If an odd number of hashes exist, the last hash is duplicated to ensure pairs can be formed.
3. **Recursive Hashing**: This pairing and hashing continue until only one hash remains, which is the block's Merkle root.

The Merkle root efficiently verifies transactions without needing to check every individual transaction, providing security and performance benefits.

## Getting Started

### In order to run the code you will need:

- C++ compiler that supports C++11 or later (e.g., GNU GCC, Clang).
- OpenSSL installed on your system for cryptographic functions.
- For reference, an easy way to run this 

### Compiling and Running

1. **Clone the Repository in Terminal or Download from Branch Directly**
- Openssl (openssl@3.0.7) will run the code

2. **Compiling**
-In the shell input:  -g++ -o name_of_program name_of_file.cpp -lssl -lcrypto
After compiling run the code: ./name_of_program
-Set name name_of_program and name_of_file.cpp to whatever you name them on your machine

3. **Running and Modifying**
-The difficulty of the mining process is determined by leading '0' in the `Blockchain` class. int difficulty = 4 means the target hash needs four leading zeros. 0000xxxx

It is found on line: 123
class Blockchain {
private:
    list<Block> chain;
    int difficulty = 4; 

-To add a new block you must create it in the main function

 Blockchain bc;                                                  <-- bc should be changed to bc2 or bc3; needs a unique name. Change this value to call class and create new block
    Block block1(bc.getLastHash(), bc.getDifficulty());          <-- new block would be Block block2 (change bc here too)
    block1.addTransaction(Transaction("Alice", "Bob", 100));     <-- block2.addTransaction(Transaction "Name", "Name", intVal));
    block1.addTransaction(Transaction("Charlie", "Dave", 50));   "add as many transactions to a block" 
    bc.addBlock(block1);                                         <-- add block to the chain (change bc and block1 here as well)

-To change the Nonce output modify line 52

if (nonce % 10000 == 0) <-- will show every 10000th Nonce. 

### Asymptotic Analysis

The following section will walk through the code's features and provide an in-depth analysis, including Big(O).

1. Mining a Block (mineBlock method)
The mining process requires finding a hash that meets a certain difficulty level. The complexity of this step is dynamic and is directly defined by the number of leading zeros in the hash.

Complexity: O(N * H)
N: Number of attempts needed to find a suitable nonce.
H: Time complexity of calculating the hash.
This operation has a probabilistic, similar in complexity to primality tests like Fermat's/Miller-Rabin, and potentially exponential time complexity depending on the difficulty level, making it the most computationally intensive part of the blockchain. *Setting the difficulty, leading zeros to 6 or higher, will massively impact the time complexity.

2. Calculating the Merkle Root (calculateMerkleRoot method)
This method constructs a Merkle tree from transaction hashes, efficiently summarizing and verifying all transactions in a block.

Complexity: O(T)
T: Total number of transactions in a block.
Each transaction hash is computed once, and pairs of hashes are combined logarithmically, Tx1 hashed with Tx2 = Tx12... Tx3 hashed with Tx4 = Tx34... Tx12 hashed with Tx34 = Tx1234... and so on, until one hash remains. The linear complexity arises from initially processing each transaction and the number of operations on each level to complete the hash. For example, to hash Tx1 and Tx2, one operation is required. So T/2 operations, at level 2, T/4 operations, which would combine to T

3. Adding a Block (addBlock method)
Adding a new block involves setting its previous hash, calculating the Merkle root, and mining the block.

Complexity: Dominated by the mineBlock method.
While setting the previous hash and calculating the Merkle root are relatively quick operations, the mining process, as mentioned above, involves a probabilistic and computational challenge based on the difficulty level. 

4. SHA-256 Hash Calculation
The hash calculation for transactions and blocks uses a cryptographic hash function, SHA-256.

Complexity: O(1)
Each hash computation involves a constant series of steps and is **independent** of the data size because the output of a single character or a thousand is hashed to the same sized output.

Summary
Mining a Block: High computational complexity, especially at higher difficulty levels.
Merkle Root Calculation: Efficient in handling transactions but costly with a very high number of transactions.
Adding a Block: Predominantly influenced by the mining process.
Hash Calculation: Constant time complexity per hash.


## Benchmarking 

Block mined in 100 ms at difficulty 3 (000)
Merkle Root of two transactions in < 0ms

Block mined in 832 ms at difficulty 4 (0000) 
Merkle Root of two transactions in < 0ms

Block mined in 9385 ms at difficulty 5 (00000)
Merkle Root of six transactions in < 0ms

Block mined in 125318 ms at difficulty 6 (000000)
Merkle Root of six transactions in < 0ms





   
