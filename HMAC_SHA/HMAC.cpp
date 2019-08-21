#include "HMAC.h"

HMAC::HMAC(SHA *sha) {
	this->sha = sha;
}

void HMAC::computeHash(uint8_t *key, uint64_t keyLength, uint8_t *text, uint64_t textLength, uint8_t *output) {
	const uint8_t	B = this->sha->getBlockSize(),
					shaOutSize = this->sha->getOutputSize();
	const uint8_t	opad = 0x5c,
					ipad = 0x36;
	int j;

	uint8_t *K = new uint8_t[B];
	if (keyLength > B) {
		uint8_t *tmp = new uint8_t[shaOutSize];
		this->sha->hash(key, keyLength, tmp);
		for (int i = 0; i < shaOutSize; i++) K[i] = tmp[i];
		for (int i = shaOutSize; i < B; i++) K[i] = 0;
		free(tmp);
	}
	else {
		for (int i = 0; i < keyLength; i++) K[i] = key[i];
		for (int i = keyLength; i < B; i++) K[i] = 0;
	}
	uint64_t innerConcatLen = B + textLength;
	uint8_t *innerConcat = new uint8_t[innerConcatLen];
	for (int i = 0; i < B; i++) innerConcat[i] = K[i] ^ ipad;
	j = 0;
	for (int i = B; i < innerConcatLen; i++) {
		innerConcat[i] = text[j];
		j++;
	}

	uint8_t *innerHashOutput = new uint8_t[shaOutSize];
	this->sha->hash(innerConcat, innerConcatLen, innerHashOutput);

	uint64_t outerConcatLen = B + shaOutSize;
	uint8_t *outerConcat = new uint8_t[outerConcatLen];
	for (int i = 0; i < B; i++) outerConcat[i] = K[i] ^ opad;
	j = 0;
	for (int i = B; i < outerConcatLen; i++) {
		outerConcat[i] = innerHashOutput[j];
		j++;
	}
	this->sha->hash(outerConcat, outerConcatLen, output);

	free(K);
	free(innerHashOutput);
	free(innerConcat);
	free(outerConcat);
}
