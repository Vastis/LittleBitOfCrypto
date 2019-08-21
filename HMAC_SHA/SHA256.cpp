#include "SHA256.h"

const uint32_t kLoc[] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

SHA256::SHA256()
{
	for (int i = 0; i < 64; i++) K[i] = kLoc[i];
	this->blockSize = 64;
	this->outputSize = 32;
}

void SHA256::hash(uint8_t *input, uint64_t inputLength, uint8_t *output)
{
	uint32_t **M;
	uint64_t N;
	uint32_t H[] = {
		0x6a09e667, 
		0xbb67ae85, 
		0x3c6ef372, 
		0xa54ff53a, 
		0x510e527f, 
		0x9b05688c, 
		0x1f83d9ab, 
		0x5be0cd19
	};
	uint32_t W[64];
	uint32_t a, b, c, d, e, f, g, h;
	uint32_t T1, T2;
	/*****************************PADDING & PARSING*********************************/
	{
		int j = 0, l = 0;
		uint8_t complement = 64 - (inputLength % 64);
		if (complement < 9) complement += 64;
		uint64_t length = inputLength + complement;
		uint8_t *tmp = new uint8_t[length];
		for (int i = 0; i < inputLength; i++) tmp[i] = input[i];
		tmp[inputLength] = 0x80;
		for (int i = inputLength + 1; i < length - 8; i++) tmp[i] = 0;
		uint8_t byteInputLength[8];
		{
			uint64_t inLen = inputLength * 8;
			for (int i = 56; i >= 0; i -= 8) {
				byteInputLength[j] = (uint8_t)(inLen >> i);
				j++;
			}
		}
		j = 0;
		for (int i = length - 8; i < length; i++) {
			tmp[i] = byteInputLength[j];
			j++;
		}

		N = length / 64;
		M = new uint32_t*[N];

		for (int i = 0; i < N; i++) {
			M[i] = new uint32_t[16];
			for (j = 0; j < 16; j++) {
				uint32_t temp = 0;
				for (int k = 0; k < 4; k++) {
					temp = temp << 8;
					temp = temp ^ tmp[l];
					l++;
				}
				M[i][j] = temp;
			}
		}
		free(tmp);
	}
	/*******************************************************************************/

	for (int i = 0; i < N; i++) {
		for (int t = 0; t < 16; t++) W[t] = M[i][t];
		for (int t = 16; t < 64; t++) W[t] = lowerSigma1(W[t - 2]) + W[t - 7] + lowerSigma0(W[t - 15]) + W[t - 16];

		a = H[0];
		b = H[1];
		c = H[2];
		d = H[3];
		e = H[4];
		f = H[5];
		g = H[6];
		h = H[7];

		for (int t = 0; t < 64; t++) {
			T1 = h + upperSigma1(e) + ch(e, f, g) + K[t] + W[t];
			T2 = upperSigma0(a) + maj(a, b, c);
			h = g;
			g = f;
			f = e;
			e = d + T1;
			d = c;
			c = b;
			b = a;
			a = T1 + T2;
		}

		H[0] = a + H[0];
		H[1] = b + H[1];
		H[2] = c + H[2];
		H[3] = d + H[3];
		H[4] = e + H[4];
		H[5] = f + H[5];
		H[6] = g + H[6];
		H[7] = h + H[7];
	}


	for (int i = 0; i < 8; i++) {
		int k = 3;
		for (int j = 0; j < 32; j += 8) {
			uint8_t out = (uint8_t)(H[i] >> j);
			output[4 * i + k] = out;
			k--;
		}
	}
	
	for (int i = 0; i < N; i++) free(M[i]);
	free(M);
}

uint32_t SHA256::ch(uint32_t x, uint32_t y, uint32_t z) {
	return ((x & y) ^ (~x & z));
}
uint32_t SHA256::maj(uint32_t x, uint32_t y, uint32_t z) {
	return ((x & y) ^ (x & z) ^ (y & z));
}
uint32_t SHA256::rightShift(uint32_t x, uint32_t n) {
	return (x >> n);
}
uint32_t SHA256::rotateRight(uint32_t x, uint32_t n) {
	return ((x >> n) | (x << (32 - n)));
}
uint32_t SHA256::upperSigma0(uint32_t x) {
	return (rotateRight(x, 2) ^ rotateRight(x, 13) ^ rotateRight(x, 22));
}
uint32_t SHA256::upperSigma1(uint32_t x) {
	return (rotateRight(x, 6) ^ rotateRight(x, 11) ^ rotateRight(x, 25));
}
uint32_t SHA256::lowerSigma0(uint32_t x) {
	return (rotateRight(x, 7) ^ rotateRight(x, 18) ^ rightShift(x, 3));
}
uint32_t SHA256::lowerSigma1(uint32_t x) {
	return (rotateRight(x, 17) ^ rotateRight(x, 19) ^ rightShift(x, 10));
}