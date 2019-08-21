#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <map>

using namespace std;

typedef struct Element {
	Element *nextElement;
	int value;
	char letter;
} Element;

typedef struct List {
	Element *firstElement;
	int size;
} List;

void addElement(List *list, char letter, int value){
	Element *element = (Element*)malloc(sizeof(Element));
	element->letter = letter;
	element->value = value;
	element->nextElement = NULL;
	if(list->size == 0){
		list->firstElement = element;
	} else {
		Element *e = list->firstElement;
		while(e->nextElement != NULL) e = e->nextElement;
		e->nextElement = element;
	}
	list->size++;
}

List* sort(map<char,int> &letters){
	List *list = (List*)malloc(sizeof(List));
	list->size = 0;
	list->firstElement = NULL;
	int loopCounter = letters.size();
	map<char,int>::iterator iter;
	for(int i=0;i<loopCounter;i++){
		iter = letters.begin();
		for(map<char,int>::iterator it=letters.begin();it!=letters.end();++it){
			if(it->second > iter->second) iter = it;
		}
		addElement(list, iter->first, iter->second);
		letters.erase(iter);
	}
	return list;
}

void printList(List *list){
	Element *e = list->firstElement;
	while(e != NULL){
		cout << e->letter << " " << e->value << endl;
		e = e->nextElement;
	}
}

void getTextFreq(string &s, map<char,int> &left, map<char,int> &right){
	for(int i=0;i<s.length()/2;i+=2){
		if(left.count(tolower(s[i])) > 0) left[tolower(s[i])]++;
		else left[tolower(s[i])]=1;
		if(right.count(tolower(s[i+1])) > 0) right[tolower(s[i+1])]++;
		else right[tolower(s[i+1])]=1;
	}
}

string getFileText(int &size){
	fstream file;
	file.open("cipherText.txt", ios::in);
	if(!file.good()) {
		throw "Blad otwarcia pliku\n";
	}
	else {
		string s = ""; 
		while(!file.eof()){
			string sp;
			getline(file,sp);
			s+=sp;
		}
		
		cout << s << endl;
		size = s.length();
		return s;
	}
}

void intoFile(string &s){
	fstream file;
	file.open("message.txt", ios::out);
	file << s;
	file.close();
}

int main(){
	string s;
	int stringSize;
	try {
		s = getFileText(stringSize);
		intoFile(s);
	} catch(string s){
		cout << s << endl;
	}
	cout << endl;
	map<char,int> left, right;
	getTextFreq(s, left, right);
	cout << "Left" << endl;
	printList(sort(left));
	cout << endl << "Right" << endl;
	printList(sort(right));
	cout << endl << stringSize << endl;
	system("PAUSE");
	return 0;
}
