#ifndef SHA1_H
#define SHA1_H

#include "SHA.h"

class SHA1 : public SHA {
public:
	SHA1();
	virtual void hash(uint8_t *input, uint64_t inputLength, uint8_t *output);

private:
	uint32_t K[64];

	uint32_t ch(uint32_t x, uint32_t y, uint32_t z);
	uint32_t maj(uint32_t x, uint32_t y, uint32_t z);
	uint32_t parity(uint32_t x, uint32_t y, uint32_t z);
	uint32_t rotateLeft(uint32_t x, uint32_t n);
};

#endif