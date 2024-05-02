#pragma once
#include <vector>
#include <sstream>
#include <iomanip>
#include <stdint.h>
#include <random>
using namespace std;
// Fractional Values of the cubic roots of the first 64 prime numbers
// Prime Numbers 2-311
const uint32_t K[64] = {
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
};
// Initializing Varibles for incorporating constant hex values into mixing process
const int a = 0, b = 1, c = 2, d = 3, e = 4, f = 5, g = 6, h = 7;

// Shifting All the bits to the right by 32, since the words are 32 bits this rotates them by 32-n
uint32_t rightRotate(uint32_t x, uint32_t n) {
	return (x >> n) | (x << (32 - n));
}
// Mixes up a 32-bit word by using exculsive or and right rotateing twice; version 0
uint32_t littleSigma0(uint32_t x) {
	return rightRotate(x, 7) ^ rightRotate(x, 18) ^ (x >> 3);
}
// Mixes up a 32-bit word by using exculsive or and right rotateing twice; version 1
uint32_t littleSigma1(uint32_t x) {
	return rightRotate(x, 17) ^ rightRotate(x, 19) ^ (x >> 10);
}
// Mixes up a 32-bit word by using exculsive or and right rotateing  three times: version 0
uint32_t bigSigma0(uint32_t x) {
	return rightRotate(x, 2) ^ rightRotate(x, 13) ^ rightRotate(x, 22);
}
// Mixes up a 32-bit word by using exculsive or and right rotateing three times: version 1
uint32_t bigSigma1(uint32_t x) {
	return rightRotate(x, 6) ^ rightRotate(x, 11) ^ rightRotate(x, 25);
}
// intnilizes the mixing process by integrating the message chunk into the message schedule array or MSA
void MSA(uint32_t(&W)[64], uint8_t(&block)[64]) {
	// Adding message to the first 16 words
	for (int i = 0; i < 16; i++) {
		W[i] = (block[i * 4] << 24) |
			(block[i * 4 + 1] << 16) |
			(block[i * 4 + 2] << 8) |
			(block[i * 4 + 3]);
	}
	// Spreading message to the other 48 words
	for (int i = 16; i < 64; i++) {
		W[i] = littleSigma1(W[i - 2]) + W[i - 7] +
			littleSigma0(W[i - 15]) + W[i - 16];
	}
}
// 
uint32_t choice(uint32_t x, uint32_t y, uint32_t z) {
	return (x & y) ^ (~x & z);
}
// Returns dominate value
uint32_t majority(uint32_t x, uint32_t y, uint32_t z) {
	return (x & y) ^ (x & z) ^ (y & z);
}
// Does one round of mixing values
void round(uint32_t(&H)[8], uint32_t round_constant, uint32_t schedule_word) {
	// Incorporates consant hex vals and message word chunks
	uint32_t T1 = H[h] + bigSigma1(H[e]) + choice(H[e], H[f], H[g]) + round_constant + schedule_word;

	uint32_t T2 = bigSigma0(H[a]) + majority(H[a], H[b], H[c]);
	for (int i = 7; i > 0; i--) {
		H[i] = H[i - 1];
	}
	H[a] = T1 + T2;
	H[e] += T1;
}
//Computes and hashs one chunk of 512 bit size or in other words compresses the block 
void compressBlock(uint32_t(&H)[8], uint8_t(&block)[64]) {
	uint32_t W[64];
	uint32_t h[8];

	MSA(W, block);

	for (int i = 0; i < 8; i++) h[i] = H[i]; // Initializing consant hex vals
	for (int i = 0; i < 64; i++) round(h, W[i], K[i]); // Performs rounds for each word in chunk
	for (int i = 0; i < 8; i++) H[i] += h[i];// Modifies Consant Hex Vals
}
// Main function call 
pair<string, string> sha256(string m, string S = "") {
	/*First 8 Prime fractional values of their square roots, meaning the whole digits are removed, it's just the decimal values
 Prime numbers 2,3,5,7,11,13,17,19*/
	uint32_t H[8] = { 0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19 };
	int size = m.size();
	int l = (size * 8); // Translateing from chars to bits
	vector<uint8_t> message;

	int N = 1;//Finding number of 512-bit chunks
	if ((l + 64) > 512) { N += ((l + 64) / 512);}

	int k = 0;// Number of Padding Digits
	for (int i = l; i < ((N * 512) - 8 - 64); i += 8) {k++;}

	stringstream sS; // Generatoring Salt Value
	if (S == "") {
		int tempK = 0b0;
		for (int i = 0; i < k; i++) {
			double randVal = static_cast<double>(rand()) / RAND_MAX;
			randVal = round(randVal);
			tempK += randVal;
		}
		sS << hex << tempK;
		S = sS.str();
	}
	//Preparing for Mixing and appyling Salt value
	message.reserve(size + k + 1 + 8);
	copy(m.c_str(), m.c_str() + size, back_inserter(message));
	message.push_back(128);
	message.insert(message.end(), S.begin(), S.end());

	for (int i = 0; i < 8; ++i) {
		message.push_back(((((unsigned long long) l) >> (56 - (8 * i))) & 0xFFu));
	}

	uint8_t block[64];
	for (int i = 0; i < N; i++) {
		memcpy(block, &message[i * 64], 64);
		compressBlock(H, block);
	}
	// Converting output into hex string
	std::stringstream ss;
	for (size_t i = 0; i < 8; ++i) {
		ss << std::hex << std::setw(8) << std::setfill('0') << H[i];
	}
	// Returning Hex Digest as a string and the Salt Value as a string
	return make_pair(ss.str(), S);
}