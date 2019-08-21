#include "SHA1.h"

SHA1::SHA1()
{
	this->blockSize = 64;
	this->outputSize = 20;
}

void SHA1::hash(uint8_t *input, uint64_t inputLength, uint8_t *output)
{
	uint32_t **M;
	uint64_t N;
	uint32_t H[] = {
		0x67452301,
		0xefcdab89,
		0x98badcfe,
		0x10325476,
		0xc3d2e1f0
	};

	uint32_t W[80];
	uint32_t a, b, c, d, e;
	uint32_t T;
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
		for (int t = 16; t < 80; t++) W[t] = rotateLeft(W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16] , 1);

		a = H[0];
		b = H[1];
		c = H[2];
		d = H[3];
		e = H[4];

		for (int t = 0; t < 20; t++) {
			T = rotateLeft(a, 5) + ch(b, c, d) + e + 0x5a827999 + W[t];
			e = d;
			d = c;
			c = rotateLeft(b, 30);
			b = a;
			a = T;
		}
		for (int t = 20; t < 40; t++) {
			T = rotateLeft(a, 5) + parity(b, c, d) + e + 0x6ed9eba1 + W[t];
			e = d;
			d = c;
			c = rotateLeft(b, 30);
			b = a;
			a = T;
		}
		for (int t = 40; t < 60; t++) {
			T = rotateLeft(a, 5) + maj(b, c, d) + e + 0x8f1bbcdc + W[t];
			e = d;
			d = c;
			c = rotateLeft(b, 30);
			b = a;
			a = T;
		}
		for (int t = 60; t < 80; t++) {
			T = rotateLeft(a, 5) + parity(b, c, d) + e + 0xca62c1d6 + W[t];
			e = d;
			d = c;
			c = rotateLeft(b, 30);
			b = a;
			a = T;
		}

		H[0] = a + H[0];
		H[1] = b + H[1];
		H[2] = c + H[2];
		H[3] = d + H[3];
		H[4] = e + H[4];
	}


	for (int i = 0; i < 5; i++) {
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

uint32_t SHA1::ch(uint32_t x, uint32_t y, uint32_t z) {
	return ((x & y) ^ (~x & z));
}
uint32_t SHA1::maj(uint32_t x, uint32_t y, uint32_t z) {
	return ((x & y) ^ (x & z) ^ (y & z));
}
uint32_t SHA1::parity(uint32_t x, uint32_t y, uint32_t z) {
	return (x ^ y ^ z);
}
uint32_t SHA1::rotateLeft(uint32_t x, uint32_t n) {
	return ((x << n) | (x >> (32 - n)));
}