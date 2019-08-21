#ifndef SHA_H
#define SHA_H

#include <iostream>

class SHA {
public:
	virtual void hash(uint8_t *input, uint64_t inputLength, uint8_t *output) = 0;
	
	uint8_t getBlockSize() {
		return blockSize;
	};
	uint8_t getOutputSize() {
		return outputSize;
	};
protected:
	uint8_t blockSize;
	uint8_t outputSize;
};

#endif
