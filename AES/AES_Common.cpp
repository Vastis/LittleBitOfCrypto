#include <iostream>
#include <string>

using namespace std;

#pragma region constants
unsigned char rCon[16][4] = {
	{ 0x00 , 0x00 , 0x00 , 0x00 },//instead of substracting
	{ 0x01 , 0x00 , 0x00 , 0x00 },
	{ 0x02 , 0x00 , 0x00 , 0x00 },
	{ 0x04 , 0x00 , 0x00 , 0x00 },
	{ 0x08 , 0x00 , 0x00 , 0x00 },
	{ 0x10 , 0x00 , 0x00 , 0x00 },
	{ 0x20 , 0x00 , 0x00 , 0x00 },
	{ 0x40 , 0x00 , 0x00 , 0x00 },
	{ 0x80 , 0x00 , 0x00 , 0x00 },
	{ 0x1B , 0x00 , 0x00 , 0x00 },
	{ 0x36 , 0x00 , 0x00 , 0x00 },
	{ 0x6C , 0x00 , 0x00 , 0x00 },
	{ 0xD8 , 0x00 , 0x00 , 0x00 },
	{ 0xAB , 0x00 , 0x00 , 0x00 },
	{ 0x1B , 0x00 , 0x00 , 0x00 },
	{ 0x4D , 0x00 , 0x00 , 0x00 },
};

int keyLength,
	keyColumnsNo,
	roundsNo;
#pragma endregion

string getMessage() {
	string inputMessage;
	cout << "Message: ";
	cin >> inputMessage;
	return inputMessage;
}

unsigned char** intoState(string &input) {
	if (input.length() < 16) {
		int tmp = 16 - input.length();
		string append = "";
		for (int i = 0; i < tmp; i++) append += (char)0;
		input += append;
	}
	unsigned char **state = new unsigned char*[4];
	for (int i = 0; i < 4; i++) state[i] = new unsigned char[4];
	int k = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			state[j][i] = input[k];
			k++;
		}
	}
	return state;
}

string keyGen(int keyLength) {
	string key;
	for (int i = 0; i < keyLength; i += 8) key += 'a';
	return key;
}

string generateKey(string &mode) {
	string key;
	if (mode == "128") {
		keyLength = 128;
		keyColumnsNo = 4;
		roundsNo = 10;
	}
	else if (mode == "192") {
		keyLength = 192;
		keyColumnsNo = 6;
		roundsNo = 12;
	}
	else if (mode == "256") {
		keyLength = 256;
		keyColumnsNo = 8;
		roundsNo = 14;
	}
	else {
		cout << "Invalid mode!" << endl;
		key = generateKey(mode);
	}
	return keyGen(keyLength);
}

string intoString(unsigned char **input) {
	string result = "";
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result += input[j][i];
		}
	}
	return result;
}




unsigned char mul2(unsigned char a) {
	unsigned char result = a;
	if (result >= 0x80) {
		result = result << 1;
		result ^= 0x1b;
	}
	else result = result << 1;
	return result;
}

unsigned char recursiveMul2(unsigned char a, int flag) {
	if (flag == 0) return a;
	else recursiveMul2(mul2(a), flag - 1);
}

unsigned char mul(unsigned char a, unsigned char b) {
	unsigned char result = 0;
	for (int i = 0; i < 4; i++) {
		bool bit = b % 2;
		if(bit) result ^= recursiveMul2(a, i);
		b = b >> 1;
	}
	return result;
}

void rotateRight(unsigned char *word) {
	unsigned char tmp = word[3];
	word[3] = word[2];
	word[2] = word[1];
	word[1] = word[0];
	word[0] = tmp;
}

void rotateLeft(unsigned char *word) {
	unsigned char tmp = word[0];
	word[0] = word[1];
	word[1] = word[2];
	word[2] = word[3];
	word[3] = tmp;
}

unsigned char* xor(unsigned char *word1, unsigned char *word2) {
	unsigned char *xor = new unsigned char[4];
	for (int i = 0; i < 4; i++) xor [i] = word1[i] ^ word2[i];
	return xor;
}



//////////////////////////////////
unsigned char hexSwitch(unsigned char x) {
	if (x < 10) return '0' + x;
	else return 'a' - 10 + x;
}
void printHex(unsigned char c) {
	unsigned char x = c >> 4;
	cout << hexSwitch(x);
	x = c - (x << 4);
	cout << hexSwitch(x);
}