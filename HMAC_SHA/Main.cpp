#include "HMAC.h"
#include "SHA1.h"
#include "SHA256.h"

int main() {
	SHA *sha1 = new SHA1();
	SHA *sha256 = new SHA256();
	HMAC hmac_sha1(sha1);
	HMAC hmac_sha256(sha256);

	uint64_t	keyLength = 16,
				messageLength = 10;
	uint8_t *key = new uint8_t[keyLength],
			*message = new uint8_t[messageLength],
			*computedHash = new uint8_t[32];

	for (int i = 0; i < keyLength; i++) {
		key[i] = i;
		printf("%02x ", key[i]);
	}
	std::cout << std::endl;
	for (int i = 0; i < messageLength; i++) {
		message[i] = i;
		printf("%02x ", message[i]);
	}
	std::cout << std::endl;

	hmac_sha1.computeHash(key, keyLength, message, messageLength, computedHash);
	printf("SHA-1: ");
	for (int i = 0; i < 20; i++) printf("%02x ", computedHash[i]);
	std::cout << std::endl;

	hmac_sha256.computeHash(key, keyLength, message, messageLength, computedHash);
	printf("SHA-256: ");
	for (int i = 0; i < 32; i++) printf("%02x ", computedHash[i]);
	std::cout << std::endl; 


	/*
	TO DO:

	SHA 224
	SHA 384
	SHA 512
	SHA 512/224
	SHA 512/256
	SHA 3
	
	*/
	system("pause");

	delete(sha1);
	delete(sha256);
	return 0;
}