#ifndef SHA256_H
#define SHA256_H

#include "SHA.h"

class SHA256 : public SHA {
public:
	SHA256();
	virtual void hash(uint8_t *input, uint64_t inputLength, uint8_t *output);

private:
	uint32_t K[64];

	uint32_t ch(uint32_t x, uint32_t y, uint32_t z);
	uint32_t maj(uint32_t x, uint32_t y, uint32_t z);
	uint32_t rightShift(uint32_t x, uint32_t n);
	uint32_t rotateRight(uint32_t x, uint32_t n);
	uint32_t upperSigma0(uint32_t x);
	uint32_t upperSigma1(uint32_t x);
	uint32_t lowerSigma0(uint32_t x);
	uint32_t lowerSigma1(uint32_t x);
};

#endif
