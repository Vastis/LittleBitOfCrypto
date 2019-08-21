#include <iostream>
#include <string>
#include "AES.h"

using namespace std;

int main() {
	string mode = "128";
	string key = generateKey(mode);
	string	encryptedMessage = "",
			decryptedMessage = "";

		/*unsigned char zero = 0;
		string inputMessage = "";
		inputMessage += (unsigned char)zero;
		inputMessage += (unsigned char)0x11;
		inputMessage += (unsigned char)0x22;
		inputMessage += (unsigned char)0x33;
		inputMessage += (unsigned char)0x44;
		inputMessage += (unsigned char)0x55;
		inputMessage += (unsigned char)0x66;
		inputMessage += (unsigned char)0x77;
		inputMessage += (unsigned char)0x88;
		inputMessage += (unsigned char)0x99;
		inputMessage += (unsigned char)0xaa;
		inputMessage += (unsigned char)0xbb;
		inputMessage += (unsigned char)0xcc;
		inputMessage += (unsigned char)0xdd;
		inputMessage += (unsigned char)0xee;
		inputMessage += (unsigned char)0xff;

		key = "";
		key += (unsigned char)zero;
		key += (unsigned char)0x01;
		key += (unsigned char)0x02;
		key += (unsigned char)0x03;
		key += (unsigned char)0x04;
		key += (unsigned char)0x05;
		key += (unsigned char)0x06;
		key += (unsigned char)0x07;
		key += (unsigned char)0x08;
		key += (unsigned char)0x09;
		key += (unsigned char)0x0a;
		key += (unsigned char)0x0b;
		key += (unsigned char)0x0c;
		key += (unsigned char)0x0d;
		key += (unsigned char)0x0e;
		key += (unsigned char)0x0f;*/

		string inputMessage = "Dzialam? Dzialam!";
		key = "klucz jest spoko";
		
		cout << "Input(string) = " << inputMessage << endl;
		cout << "Input = ";
		for (int i = 0; i < 16; i++) {
			printHex(inputMessage[i]);
			cout << " ";
		}
		cout << endl;
		cout << "Cipher Key = " << key << endl;
		cout << "Cipher Key = ";
		for (int i = 0; i < 16; i++) {
			printHex(key[i]);
			cout << " ";
		}
		cout << endl;
		cout << endl;

	unsigned char **word = keyExpansion(key);
	for (int i = 0; i < inputMessage.length(); i += 16) encryptedMessage += aesEn(inputMessage.substr(i, 16), word);

		cout << "Encrypted message = ";
		for (int i = 0; i < 16; i++) {
			printHex(encryptedMessage[i]);
			cout << " ";
		}
		cout << endl;
		for (int i = 0; i < inputMessage.length(); i += 16) decryptedMessage += aesDe(encryptedMessage.substr(i, 16), word);
		cout << "Decrypted message = ";
		for (int i = 0; i < 16; i++) {
			printHex(decryptedMessage[i]);
			cout << " ";
		}
		cout << endl;
		cout << "Decrypted message = " << decryptedMessage << endl;
	system("pause");
	return 0;
}