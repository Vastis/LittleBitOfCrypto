#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <map>

double engilshIoC = 1.73;
/*
	1. sprawdzić dla ciągów znaków (jaka długość?) odległości między powtarzającymi się
        - dzielniki liczby też wchodzą
        - jeżeli liczba się powtarza, to nie wchodzi
    2. zrobić substringi prawdopodobnej długości (najlepiej jednej z tych z 1.)
        - sprawdzić IoC każdego
        - wybrać zestaw z najbliższym IoC
    3. analiza częstości, porównać, zobaczyć czy ma sens
*/
using namespace std;

class IndexOfCoincidence {
    public:
        IndexOfCoincidence(){};
        string* chooseSubstring(string &s, int period){
            string *result = new string[period];
            int i = 0;
            int p = 0;
            while(i != s.length()){
                result[p]+=s[i];
                i++;
                p++;
                if(p == period) p = 0;
            }
            return result;
        };
        int makeKasiskiTest(string &str, string &substr){
            int i = 0,
                    delta = 0,
                    pointer = 0;
            bool exists = false, good = false;
            while(i != str.length() && !exists){
                if(good){
                    if(str[i] != substr[pointer]) {
                        good = false;
                        pointer = -1;
                    }
                    pointer++;
                    if(pointer == substr.length()) exists = true;
                } else {
                    if(str[i] == substr[pointer]) {
                        good = true;
                        pointer++;
                        if(substr.length() == 1) exists = true;
                    }
                }
                i++;
            }
            good = false;
            pointer = 0;
            while(i != str.length()){
                if(good){
                    if(str[i] != substr[pointer]) {
                        good = false;
                        delta += pointer+1;
                        pointer = -1;
                    }
                    pointer++;
                    if(pointer == substr.length()) return delta;
                } else {
                    if(str[i] == substr[pointer]) {
                        good = true;
                        pointer++;
                        if(substr.length() == 1) return delta;
                    } else delta++;
                }
                i++;
            }
            return -1;
        };
        double calculateIoC(string &s){
            map<char,int> lettersCounter;
            int i = 0,
                    sum = 0,
                    down,
                    alphabetSize;
            while(i != s.length()){
                if(lettersCounter.count(s[i]) <= 0) lettersCounter[s[i]] = 0;
                lettersCounter[s[i]]++;
                i++;
            }
            alphabetSize = lettersCounter.size();
            for(map<char,int>::iterator it=lettersCounter.begin(); it!=lettersCounter.end(); ++it){
                int tmp = it->second;
                tmp *= tmp-1;
                sum += tmp;
            }
            sum *= alphabetSize;
            down = s.length();
            down *= down-1;
            double result = (double)sum/(double)down;
            return result;
        };
};
class Vigener {
    public:
        Vigener(){}
        Vigener(string &cipherText){
            this->cipherText = cipherText;
        }
        Vigener(string &text, string &key, bool flag){
            if(!flag) this->plainText = text;
            else this->cipherText = text;
            this->key = key;
        }
        string encryption(){
            string ciphertext = encryption(plainText, key);
            this->cipherText = ciphertext;
            return ciphertext;
        }
        string decryption(){
            string plaintext = decryption(cipherText, key);
            this->plainText = plaintext;
            return plaintext;
        }
        string getPlainText(){
            return plainText;
        }
        string getCipherText(){
            return cipherText;
        }
        string getKey(){
            return key;
        }
    private:
        string  plainText,
                cipherText,
                key;
        string encryption(string &plaintext, string &key){
            string ciphertext = "";
            int j = 0;
            for(int i=0;i<plaintext.length();i++){
                if(j == key.length()) j = 0;
                if(plaintext[i] == ' ' || plaintext[i] == '.' || plaintext[i] == ',') ciphertext += plaintext[i];
                else {
                    ciphertext += (tolower(plaintext[i])+tolower(key[j])-'a'-'a')%26 + 'a';
                    j++;
                }

            }
            return ciphertext;
        }
        string decryption(string &ciphertext, string &key){
            string plaintext = "";
            int j = 0;
            for(int i=0;i<ciphertext.length();i++){
                if(j == key.length()) j = 0;
                if(ciphertext[i] == ' ' || ciphertext[i] == '.' || ciphertext[i] == ',') plaintext += ciphertext[i];
                else {
                    plaintext += (tolower(ciphertext[i])-tolower(key[j])+26)%26 + 'a';
                    j++;
                }

            }
            return plaintext;
        }
};

string convert(string &text){
    string result = "";
    for(int i=0;i<text.length();i++){
        if(text[i] != ' ' && text[i] != '.' && text[i] != ',') result += tolower(text[i]);
    }
    return result;
}
string* iocComp(string &inputText, int maxKeyLength, int &keySize){
    IndexOfCoincidence ioc = IndexOfCoincidence();
    double closestIoC = 0.0;
    string *goodSubs;
    //int keySize = 0;
    for(int kLen=2;kLen<=maxKeyLength;kLen++){
        string *substring = ioc.chooseSubstring(inputText,kLen);
        for(int i=0;i<kLen;i++){
            double ic = ioc.calculateIoC(substring[i]);
            if(abs(engilshIoC - ic) < abs(engilshIoC - closestIoC)) {
                closestIoC = ic;
                goodSubs = substring;
                keySize = kLen;
            }
        }
    }
    return goodSubs;
}
string analysis(map<char, double> lettersFreq, string &cipherText, string *subs, int keySize, int acc){
    IndexOfCoincidence ioc = IndexOfCoincidence();
    string potentialKey;
    for(int i=0;i<keySize;i++) potentialKey += 'a';
    for(int a=0;a<acc;a++){
        string key = "";
        char keyChar;
        for(int keyPos=0;keyPos<keySize;keyPos++) {
            double closest = 100.0;
            for (int l = 0; l < 26; l++) {
                potentialKey[keyPos] = 'a' + l;
                Vigener v = Vigener(cipherText, potentialKey, 1);
                string s = v.decryption();
                map<char,int> freq;
                int i = 0;
                while(i != s.length()){
                    if(freq.count(s[i]) <= 0) freq[s[i]] = 0;
                    freq[s[i]]++;
                    i++;
                }
                double sum = 0;
                for(i=0;i<freq.size();i++) sum += abs((double)freq['a'+i]/(double)s.length()-lettersFreq['a'+i]);
                if(sum < closest) {
                    closest = sum;
                    keyChar = 'a'+l;
                }
            }
            key += keyChar;
        }
        potentialKey = key;
    }
    cout << "Key: " << potentialKey << endl << endl;
    return potentialKey;
}
string z2(map<char, double> lettersFreq){
    int maxKeyLength = 6;
    string cipherText = "Xbpvv guyrfx vp e cehryrky xsii oymmildec eho qfvy dmdtfp, qfvy qvvi zcsd ilcsiw uyh ffmnyimntij...qici nslelp xi pbgvydw kly trmeltespy cicentsew iq ecp hlxlvuw xymhrw kluy qrxbpqrxcnw. Zx cyxvvjcikw uwp glyysdihl fp xbp wrqy weekolkv, em tj ks uexvwn elv yhtxp eho wzqjwmtmnj sw xbp tceh zj kly frzzycwv, eho xf quvi jxcwp dslp immxprk xblx lrwseekylfci ichvv qsmtl jcijmxpw fzyc ecp hlxlvuw grympw";
    cout << cipherText << endl << endl;
    string inputText = convert(cipherText);
    int keySize = 0;
    string *substrings = iocComp(inputText, maxKeyLength, keySize);
    string key = analysis(lettersFreq, cipherText, substrings, keySize, 10);
    Vigener v = Vigener(cipherText, key, 1);
    string decrypted = v.decryption();
    return decrypted;
}
string z3(map<char, double> lettersFreq, string &cipherText){
    int maxKeyLength = 8;
    cout << cipherText << endl << endl;
    string inputText = convert(cipherText);
    int keySize = 0;
    string *substrings = iocComp(inputText, maxKeyLength, keySize);
    string key = analysis(lettersFreq, cipherText, substrings, keySize, 10);
    Vigener v = Vigener(cipherText, key, 1);
    string decrypted = v.decryption();
    return decrypted;
}

int main(){
    map<char,double> lettersFreq;

    lettersFreq['e'] = 0.1249;
    lettersFreq['t'] = 0.0928;
    lettersFreq['a'] = 0.0804;
    lettersFreq['o'] = 0.0764;
    lettersFreq['i'] = 0.0757;
    lettersFreq['n'] = 0.0723;
    lettersFreq['s'] = 0.0651;
    lettersFreq['r'] = 0.0628;
    lettersFreq['h'] = 0.0505;
    lettersFreq['l'] = 0.0407;
    lettersFreq['d'] = 0.0382;
    lettersFreq['c'] = 0.0334;
    lettersFreq['u'] = 0.0273;
    lettersFreq['m'] = 0.0251;
    lettersFreq['f'] = 0.0240;
    lettersFreq['p'] = 0.0214;
    lettersFreq['g'] = 0.0187;
    lettersFreq['w'] = 0.0168;
    lettersFreq['y'] = 0.0166;
    lettersFreq['b'] = 0.0148;
    lettersFreq['v'] = 0.0105;
    lettersFreq['k'] = 0.0054;
    lettersFreq['x'] = 0.0023;
    lettersFreq['j'] = 0.0016;
    lettersFreq['q'] = 0.0012;
    lettersFreq['z'] = 0.0009;

    /*string s = "efaaabbbcccddde1234567aaabbbcccdddefa"; //fa->fa - 32
    string sub = "fa";
    int period = 3;
    IndexOfCoincidence ioc = IndexOfCoincidence();
    string *substring = ioc.chooseSubstring(s,period);
    for(int i=0;i<period;i++){
        cout << substring[i] << endl;
    }

    cout << endl << ioc.makeKasiskiTest(s,sub) << endl;

    cout << ioc.calculateIoC(s);*/

    /*string plain = "Wrobel jest gupi, lysy i niski.";
    string key = "kulfa";
    Vigener v = Vigener(plain, key);
    cout << v.getPlainText() << endl << v.getKey() << endl << endl;
    string ciphert = v.encryption();
    cout << v.decryption();*/
    cout << "Zadanie 2: " << endl << endl;
    string resultZ2 = z2(lettersFreq);
    cout << resultZ2 << endl << endl << endl;
    //string resultZ3 = z3(lettersFreq);
    cout << "Zadanie 3 wariant 1: " << endl << endl;
    string c31 = "MBVBEUNVCMJOMBZVGKZYIAVYMBRBJKAUMMRDTGZVRJHHCGGNXPVZLYFUCTBTXMDILHXUCTGNXYOKVZBHXAGAYZYICVXHJIGXXUCTLHBAECZHXLJWAKLQVKNTMYMMAHXAZVGUMBZVXGUILBVTTHPDRXRXVNVTBNVENEHOIJEGBHJPNBXYMWYBXXKWTKMOJWHZHZKPRXTCENVTWQYMEKMBVJRXKCVANXXUELXKXJLASXHGXMGZBHXSVREYUWHXULRQAYWCUVBZXPFTIKMIYMYVNMXZNYILVIYRRFRZTKGODJRXLIIBBRHIBIGZACEOFOGUYCAJKYUBUUNMRVQJBGVVFOHHJ";
    string resultZ31 = z3(lettersFreq, c31);
    cout << resultZ31 << endl << endl;
    cout << "Zadanie 3 wariant 2: " << endl << endl;
    string c32 = "ZHPHZGABWSEAZHTBBWMECGQKZHLHEWNAGSMPGMTBMVUNWMBZKVPFGKSAWZWFKSXOGTKAWZBZKEIQQLONRGBMLFSOXHKNDOBJKAWZGTOGYIUTKRDCVWYWPQIFZEGSVTKGTBBGZHTBSSHOFHQFGNJJMJEDPTQFOTPKIQUUCPZSONDVINKEGCTNKDECOWZUDCCLUFEVMJGIYTQFJWSSZWZHPPMJXIPGIJKAYRSWKPFGNJUMRSBLONRYQDREOCCJHRLWVKJIOBWLKVZZDWZOSSTHASRFIKVRPOTDELLFOWTUXPMJYOCHWDUOVOBLNIYUAATASIVVXEOHPGASLBLVOMPBAAUND";
    string resultZ32 = z3(lettersFreq, c32);
    cout << resultZ32 << endl << endl;
    return 0;
}






