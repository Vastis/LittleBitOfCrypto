#include <stdio.h>
#include <math.h>
#include <ctime>
#include <cstdlib>

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
int main(int argc, char** argv){
	long long a,n;
	int i,j;
	n = toInt(argv[1]);
	i = toInt(argv[2]);
	srand(time(NULL));
	for(j=0;j<i;j++){
		a = rand()%n;
		if(modulo(a,(n-1)/2,n) != jakobi(a,n)) {
			printf("0");
			return 0;
		}
	}
	printf("1");
	return 1;
}
