#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

typedef struct Base {
	int *base;
	int baseSize;	
} Base;
typedef struct Row {
	long long value,squareValue;
	bool *vector;
	Row *nextRow;
} Row;
typedef struct Memory {
	int rows;
	Row *firstRow;
} Memory;
void addRow(Memory *m, Row *r){
	if(m->firstRow == NULL) m->firstRow = r;
	else {
		Row *tmp = m->firstRow;
		while(tmp->nextRow != NULL) tmp = tmp->nextRow;
		tmp->nextRow = r;
	}
	m->rows++;
}

long long modulo(long long a, long long b, long long n);
int jakobi(long long a, long long n);
long long gcd(long long b, long long n);
long long toInt(char* argv);

//glowne funkcje
/***************************************************/
int countB(long long n){
	int result;
	double exp = sqrt(log(n)*log(log(n)))/2;
	exp = pow(M_E,exp);
	result = exp;
	if(result < exp) result++;
	return result;
}
bool primal(int x){
	int a,j;
	for(j=0;j<50;j++){
		a = 1+rand()%(x-1);
		if(modulo(a,x-1,x) != 1) return 0;
	}
	return 1;
}
Base* getBase(long long n, int B){
	Base *b = (Base*)malloc(sizeof(Base));
	int *tmp = new int[B];
	int bSize=2, counter=3, a;
	tmp[0]=-1;
	tmp[1]=2;
	while(counter<=B){
		if(primal(counter)){
			if(jakobi(counter,n)==1){
				tmp[bSize]=counter;
				bSize++;
			}
		}
		counter+=2;
	}
	b->baseSize = bSize;
	int *base = new int[bSize];
	for(int a=0;a<bSize;a++) base[a]=tmp[a];
	b->base = base;
	delete []tmp;
	tmp = 0;
	return b;
}
Memory* getVectors(Memory *memory, int shift){
	int i;
	Row *tmp = memory->firstRow;
	for(i=0;i<shift;i++){
		tmp = tmp->nextRow;
	}
	Memory *result = (Memory*)malloc(sizeof(Memory));
	result->firstRow = NULL;
	addRow(result,tmp);
	result->rows=memory->rows-shift;
	return result;
}
bool** intoTable(Memory *m){
	Row *tmp = m->firstRow;
	bool** result = new bool*[m->rows];
	int i,j;
	for(i=0;i<m->rows;i++) result[i] = new bool[m->rows];
	for(i=0;i<m->rows;i++){
		for(j=0;j<m->rows;j++) result[j][i] = tmp->vector[j];
		tmp = tmp->nextRow;
	}
	return result;
}
int* gaussJordan(int variables, bool** factors){
	int col=0, j=0, row=0;
	for(col=0;col<variables;col++){
		if(!factors[col][col]){
			row=(col+1)%variables;
			while(!factors[row][col] && row!=col){
				row=(row+1)%variables;
			}
			for(j=0;j<variables;j++) factors[col][j] = factors[col][j] ^ factors[row][j];
		}
		row=(col+1)%variables;
		while(row!=col){
			if(factors[row][col]){
				for(j=0;j<variables;j++) factors[row][j] = factors[row][j] ^ factors[col][j];
			}
			row = (row+1)%variables;
		}
	}
	int *valid = new int[variables];
	for(j=0;j<variables;j++){
		valid[j]=-1;
		int i, check=0;
		for(i=0;i<variables;i++) {
			if(factors[j][i]) check++;
		}
		if(check == 0) valid[j]=1;
		else if(check == 1) valid[j]=0;
	}
	bool xorV;
	for(row=0;row<variables;row++){
		if(valid[row]==-1){
			xorV = 0;
			for(col=0;col<variables;col++){
				if(valid[col]!=-1) xorV^=factors[row][col];
			}
			valid[row]=xorV;
		}
	}
	return valid;
}
int sito(long long n){
	int B = countB(n);
	Base *base = getBase(n,B);																									
	Memory *memory = (Memory*)malloc(sizeof(Memory));
	memory->firstRow = NULL;
	memory->rows=0;
	long long A = sqrt(n); 
	long long X = A, intCheck, Xp;
	double doubleCheck;
	bool *v;
	int i;
	while(1){																											
		X = (A*A)%n;
		if(X > n/2) X-=n;
		v = new bool[base->baseSize];
		for(i=0;i<base->baseSize;i++){
			v[i]=0;
		}
		if(X<0) {
			v[0]=1;
			X*=-1;
		}
		else v[0]=0;
		i=1;
		Xp=X;
		while(i<base->baseSize && Xp>1){
			v[i]=0;
			intCheck = 0;
			doubleCheck = 0;
			while(doubleCheck == intCheck){
				doubleCheck = (double)Xp/(double)base->base[i];
				intCheck = (int)Xp/(int)base->base[i];
				if(doubleCheck == intCheck){
					Xp/=base->base[i];
					v[i]^=1;
				}
			}
			i++;
		}
		if(Xp==1){
			Row *row = (Row*)malloc(sizeof(Row));
			row->nextRow = NULL;
			row->value = A;
			row->squareValue = X;
			row->vector = v;
			addRow(memory,row);
			if(memory->rows%base->baseSize == 0) {
				Memory *tmp = getVectors(memory,memory->rows-base->baseSize);
				int *index = gaussJordan(base->baseSize,intoTable(tmp));
				Row *x = tmp->firstRow;
				long long r1=1,r2=1;
				for(i=0;i<base->baseSize;i++){
					if(index[i]==1) {
						r1 = (r1*x->value)%n;
						r2 = (r2*x->squareValue)%n;	
					}
					x = x->nextRow;
				}
				r2 = sqrt(r2);
				long long result = gcd(r1-r2,n);
				if(result!=n && result!=1) {
					printf("%lld",result);
					return result;	
				}
				free(index);
				free(x);
				free(tmp);
			}
		}
		A++;
	}
	return 0;
}
/***************************************************/

int main(int argc, char** argv){
	long long a,n;
	int i,j;
	n = toInt(argv[1]);
	i = 20;
	srand(time(NULL));
	for(j=0;j<i;j++){
		a = rand()%n;
		if(modulo(a,(n-1)/2,n) != jakobi(a,n)) {
			return sito(n);
		}
	}
	printf("1");
	return 1;
}

int tabSize(long long b){
	long long check = 1;
	int size = 0;

	while(check <= b) {
		size++;
		check*=2;
	}
	return size;
}
long long modulo(long long a, long long b, long long n){
	int i, tmp, t=tabSize(b);							
	long long rests[t], index, x=b, result=1;
	rests[0] = a;
		if(rests[0]>n/2) rests[i]-=n;
	for(i=1;i<t;i++) {
		rests[i] = (rests[i-1]*rests[i-1])%n;
			if(rests[i]>n/2) rests[i]-=n;
	}
	for(i=t-1;i>=0;i--){
		tmp=i;
		index = (1<<tmp);
		if(index <= x){
			x-=index;
			result=(result*rests[i])%n;
				if(result>n/2) result-=n;
				if(result<-n/2) result+=n;
		}
	}
	return result;
}
int jakobi(long long a, long long n){
	int x=0,y=a,result;
	if(a == 0) return 0;
	if(a == 1) return 1;
	while(y%2==0){
		y/=2;
		x++;
	}
	if(x%2==1 && (n%8==3||n%8==5)) result = -1;
	else result = 1;
	if(n%4==3 && y%4==3) result = -result;
	if(y == 1) return result;
	else return result*jakobi(n%y,y);
}
long long toInt(char* argv){
	long long r=0;
	int n=0,i;
	
	while(argv[n]) n++;
	for(i=0;i<n;i++){
		r+=(argv[i]-'0')*pow(10,n-i-1);
	}
	return r;
}
long long gcd(long long b, long long n){
	long long x=b,y=n,tmp;
	while(y!=0) {
		tmp = x%y;
		x = y;
		y = tmp;
	}
	return x;
}
