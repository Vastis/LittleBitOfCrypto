#ifndef HMAC_H 
#define HMAC_H

#include "SHA.h"

class HMAC {
public:
	HMAC(SHA *sha);
	void computeHash(uint8_t *key, uint64_t keyLength, uint8_t *text, uint64_t textLength, uint8_t *output);

private:
	SHA *sha;
};

#endif 
