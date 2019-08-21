#include <iostream>
#include <string>
#include "AES_Common.h"

using namespace std;

extern unsigned char sBoxDe[16][16];

unsigned char** keyExpansion(string &key);
void addRoundKey(unsigned char **state, unsigned char **roundKey);
void invSubBytes(unsigned char **state);
void shiftRows(unsigned char **state);
void mixColumns(unsigned char **state);
void round(unsigned char **state, unsigned char **roundKey);
string aesDe(string &input, unsigned char **word);