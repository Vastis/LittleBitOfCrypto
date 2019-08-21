#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>

using namespace std;

class Matrix {
	public:
		Matrix(vector< vector<int> > &values){
			this->rows = values.size();
			this->columns = values.size();
			this->values = values;
		}
		Matrix(string &input){
			this->rows = 1;
			this->columns = input.length();
			vector< vector<int> > in;
			vector<int> tmp;
			for(int i=0;i<input.length();i++){
				tmp.push_back(tolower(input[i])-'a');
			}
			in.push_back(tmp);
			this->values = in;
		}
		Matrix(vector<int> &input){
			this->rows = 1;
			this->columns = input.size();
			vector< vector<int> > values;
			values.push_back(input);
			this->values = values;
		}
		Matrix(Matrix* matrix, int det){
			this->rows = matrix->getRows();
			this->columns = matrix->getColumns();
			int invertedDet = inversion(det);
			this->values = invertedMatrixValues(matrix->getValues(), invertedDet);
		}
		
		int getRows(){
			return this->rows;
		};
		int getColumns(){
			return this->columns;
		};
		vector< vector<int> > getValues(){
			return this->values;
		};
		
		int isInvertible(){
			vector<int> columnVector;
			for(int i=0;i<this->columns;i++) columnVector.push_back(i);
			vector< vector<int> > tmp;
			tmp = values;
			int det = determinant(this->columns, 0, columnVector, tmp)%26;
			if(det < 0) det += 26;
			//wyj¹tki...
			if(det == 0) cout << "Key determinant equals 0." << endl;//throw "Key determinant equals 0.";
			if(gcd(det,26) != 1) cout << "Key determinant is not invertible." << endl;//throw "Key determinant is not invertible.";
			return det;
		}
	private:
		int rows,
			columns;
		vector< vector<int> > values;
		
		int determinant(int dimension, int row, vector<int> &columnVector, vector< vector<int> > &values){
			int	col, multiplier=1, det=0;
			if(dimension == 1) return values[row][columnVector[0]];
			else {
				for(int i=0;i<dimension;i++){
					vector<int> cV;
			  		col = 0;
			  		for(int j=0;j<dimension-1;j++){
			    		if(col == i) col++;
			    		cV.push_back(columnVector[col++]);
			  		}
				  	det += multiplier * values[row][columnVector[i]] * determinant(dimension-1,row+1,cV,values);
				  	multiplier = -multiplier;
				}
				return det;
			}
		}
		int gcd(int a, int b){
			int x=a,y=b,tmp;
			while(y!=0) {
				tmp = x%y;
				x = y;
				y = tmp;
			}
			return x;
		}
		vector< vector<int> > transposeAndMultiply(vector< vector<int> > &matrix, int invertedDeterminant){
			vector< vector<int> > result;
			int n = 1;
			for(int i=0;i<matrix.size();i++){
				int m = 1;
				vector<int> tmp;
				for(int j=0;j<matrix.size();j++){
					int v = (m*n*invertedDeterminant*matrix[j][i])%26;
					if(v < 0) v+= 26;
					tmp.push_back(v);
					m = -m;
				}
				n = -n;
				result.push_back(tmp);
			}
			return result;
		}
		vector< vector<int> > invertedMatrixValues(vector< vector<int> > matrixValues, int invertedDet){
			vector< vector<int> > result;
			for(int i=0;i<matrixValues.size();i++){
				vector<int> tmp;
				for(int j=0;j<matrixValues.size();j++){
					vector< vector<int> > minor;
					int row = 0,
						col = 0;
					for(int k=0;k<matrixValues.size()-1;k++){
						vector<int> tmp2;
						if(row == i) row++;
						col = 0;
						for(int l=0;l<matrixValues.size()-1;l++){
							if(col == j) col++;
							tmp2.push_back(matrixValues[row][col++]);
						}
						row++;
						minor.push_back(tmp2);
					}
					vector<int> columnVector;
					for(int i=0;i<matrixValues.size()-1;i++) columnVector.push_back(i);
					tmp.push_back(determinant(matrixValues.size()-1,0,columnVector,minor));
				}	
				result.push_back(tmp);
			}
			return transposeAndMultiply(result,invertedDet);
		}
		int inversion(int det){
			for(int i=1;i<26;i++)
				if((det*i)%26 == 1) return i;
		}
};
class FileReader {
	public:
		FileReader(string &fileName){
			this->file.open(fileName.c_str(), ios::in);
			if(!this->file.good()) throw "Blad otwarcia pliku";	
		};
		~FileReader(){
			file.close();	
		};
		void getInputAsString(string &input){
			getline(file,input);
		};
		Matrix* computeInputAsSquareMatrix(){
			string input;
			vector< vector<int> > values;
			int dim;
			file >> dim;
			while(!file.eof()){
				vector<int> tmp;
				int a;
				for(int i=0;i<dim;i++){
					file >> a;
					tmp.push_back(a);
				}
				for(int i=0;i<input.length();i++){
					tmp.push_back(tolower(input[i])-'a');
				}
				values.push_back(tmp);
			}
			Matrix *result = new Matrix(values);
			return result;
		};
	private:
		fstream file;
};
class HillCipher {
	public:
		HillCipher(Matrix *publicText, Matrix *encryptionKey){
			this->publicText = publicText;
			this->encryptionKey = encryptionKey;
			//determinant = encryptionKey->isInvertible();
		}
		void encrypt(){
			int blockSize = encryptionKey->getRows();
			int fullBlocksNumber = publicText->getColumns()/blockSize;
			int position = 0;
			vector<int> result;
			for(int i=0;i<fullBlocksNumber;i++){
				vector<int> messageBlock;
				for(int p=0;p<blockSize;p++) messageBlock.push_back(publicText->getValues()[0][position++]);
				for(int row=0;row<blockSize;row++){
					int sum = 0;
					for(int col=0;col<blockSize;col++){
						sum += encryptionKey->getValues()[row][col] * messageBlock[col];
					}
					result.push_back(sum%26);
				}
			}
			if(position < publicText->getColumns()){
				vector<int> messageBlock;
				while(position < publicText->getColumns()) {
					messageBlock.push_back(publicText->getValues()[0][position]);
					position++;
				}
				while(position < blockSize*(fullBlocksNumber+1)){
					messageBlock.push_back(23);
					position++;
				}
				for(int row=0;row<blockSize;row++){
					int sum = 0;
					for(int col=0;col<blockSize;col++){
						sum += encryptionKey->getValues()[row][col] * messageBlock[col];
					}
					result.push_back(sum%26);
				}
			}
			cipherText = new Matrix(result);
		}
		void decrypt(){
			decryptionKey = new Matrix(encryptionKey, determinant);
			int blockSize = decryptionKey->getRows();
			int fullBlocksNumber = cipherText->getColumns()/blockSize;
			int position = 0;
			vector<int> result;
			for(int i=0;i<fullBlocksNumber;i++){
				vector<int> messageBlock;
				for(int p=0;p<blockSize;p++) messageBlock.push_back(cipherText->getValues()[0][position++]);
				for(int row=0;row<blockSize;row++){
					int sum = 0;
					for(int col=0;col<blockSize;col++){
						sum += decryptionKey->getValues()[row][col] * messageBlock[col];
					}
					result.push_back(sum%26);
				}
			}
			if(position < cipherText->getColumns()){
				vector<int> messageBlock;
				while(position < cipherText->getColumns()) {
					messageBlock.push_back(cipherText->getValues()[0][position]);
					position++;
				}
				while(position < blockSize*(fullBlocksNumber+1)){
					messageBlock.push_back(23);
					position++;
				}
				for(int row=0;row<blockSize;row++){
					int sum = 0;
					for(int col=0;col<blockSize;col++){
						sum += decryptionKey->getValues()[row][col] * messageBlock[col];
					}
					result.push_back(sum%26);
				}
			}
			
			publicText = new Matrix(result);
		}
		void printPublicText(){
			cout << "Encrypted message: \n\t";
			for(int i=0;i<publicText->getColumns();i++) cout << (char)(publicText->getValues()[0][i]+'a');
			cout << endl;
		}
		void printCipherText(){
			cout << "Public message: \n\t";
			for(int i=0;i<cipherText->getColumns();i++) cout << (char)(cipherText->getValues()[0][i]+'a');
			cout << endl;
		}
	private:
		Matrix 	*publicText,
				*cipherText,
				*encryptionKey,
				*decryptionKey;
		int determinant;
		
};

int main(){
	string s = "message.txt";
	FileReader *fr;
	
	fr = new FileReader(s);
	fr->getInputAsString(s);
	delete fr;
	Matrix *message = new Matrix(s);
	
	cout << "Message:\n\t" << s << endl;
	
	s = "key.txt";
	fr = new FileReader(s);
	Matrix *key = fr->computeInputAsSquareMatrix();
	
	vector< vector<int> > tmp;
	tmp = key->getValues();
	cout << "\nEncryption Key:" << endl;
	for(int i=0;i<key->getRows();i++){
		cout << "\t";
		for(int j=0;j<key->getColumns();j++) cout << tmp[i][j] << " ";
		cout << endl;
	}
	delete fr;
	
	cout << endl << endl;
	HillCipher *hc = new HillCipher(message,key);
	try {
		hc->printPublicText();
		hc->encrypt();
		hc->printCipherText();
		//hc->decrypt();
		//hc->printPublicText();
	} catch (string s){
		cout << s << endl;
	}
	return 0;
}

/*
3) wyj¹tki...
4) operator mno¿enia...
*/
