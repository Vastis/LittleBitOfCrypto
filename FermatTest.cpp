#include <stdio.h>
#include <math.h>
#include <stdlib.h>

unsigned long long int random(unsigned long long int n){
	unsigned long long int a;
	unsigned char c;
	int i;
	for(i=0;i<8;i++){
		c=rand()%256;
		a=(a<<8)^c;
	}
	if(n>3) a = a%(n-2)+2;
	else a=1;
	return a;
}

int tabSize(unsigned long long int t){
	unsigned long long int check = 1;
	int size = 0;

	while(check <= t) {
		size++;
		check*=2;
	}
	return size;
}

void bits(bool* bitsT, int tabS, unsigned long long int t){
	unsigned long long int x = t, p;
	int i;
	for(i=(tabS-1);i>=0;i--){
		p = pow(2,i);
		if(x >= p) {
			bitsT[i] = true;
			x-=p;
		} else bitsT[i] = false;
	}
}

void rests(unsigned long long int a, unsigned long long int n, int tabS, unsigned long long int* restsT){
	int i;
	restsT[0] = a%n;
	for(i=1;i<tabS;i++)
		restsT[i] = (restsT[i-1]*restsT[i-1])%n;
}

int sumUp(unsigned long long int n, int tabS, bool* bitsT, unsigned long long int* restsT){
	int i, s=1;
	for(i=0; i<tabS; i++){
		if(bitsT[i]) s = (s*restsT[i])%n;
	}
	return s;
}

int potegowanieModulo(unsigned long long int a, unsigned long long int n){
	int tabS = tabSize(n-1);
	bool* bitsT = (bool*) malloc(tabS * sizeof(bitsT));
	unsigned long long int* restsT = (unsigned long long int*) malloc(tabS * sizeof(restsT));
	
					printf("\n\tIlosc bitow: %d\n",tabS);
	bits(bitsT, tabS, n-1);
					printf("\tt = %llu = ", n-1);
					int i;
					for(i=(tabS-1);i>=0;i--){
						printf("%d", bitsT[i]);
					}
	rests(a,n,tabS,restsT);
					for(i=0;i<tabS;i++){
						printf("\n\trests[%d] = %llu",i, restsT[i]);
					}
	unsigned long long int sum = sumUp(n, tabS, bitsT, restsT);
					printf("\n\n\twynik = %llu\n\n",sum);
	free(bitsT);
	free(restsT);
	return sum;
}

int main(void){
	unsigned long long int n,a,wynik;
	int i;
	int k = 100;
	printf("Podaj liczbe ktora mam sprawdzic: ");
	scanf("%llu",&n);
	for(i=0;i<k;i++){
		a = random(n);
		printf("%llu\n",a);
		wynik = potegowanieModulo(a,n);
		if(wynik != 1) {
			printf("Liczba zlozona.");
			return 1;
		}
	}
	printf("Liczba prawdopodobnie pierwsza.");
	return 0;
}
