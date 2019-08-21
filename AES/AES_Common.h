#include <iostream>
#include <string>

using namespace std;

extern unsigned char rCon[16][4];
extern int	keyLength,
			keyColumnsNo,
			roundsNo;

string getMessage();
unsigned char** intoState(string &input);
string generateKey(string &mode);
string intoString(unsigned char **input);

void rotateRight(unsigned char *word);
void rotateLeft(unsigned char *word);
unsigned char* xor(unsigned char *word1, unsigned char *word2);

unsigned char mul(unsigned char a, unsigned char b);

/////////////////////////////////
void printHex(unsigned char c);