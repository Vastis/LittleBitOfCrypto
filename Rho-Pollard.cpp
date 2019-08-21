#include <stdio.h>
#include <math.h>
long long toInt(char* argv){
	long long r=0;
	int n=0,i;
	
	while(argv[n]) n++;
	for(i=0;i<n;i++){
		r+=(argv[i]-'0')*pow(10,n-i-1);
	}
	return r;
}
long long GCD(long long b, long long n){
	long long x=b,y=n,tmp;
	while(y!=0) {
		tmp = x%y;
		x = y;
		y = tmp;
	}
	return x;
}
int main(int argc, char **argv){
	long long n = toInt(argv[1]), a=2, b=2, d;
	while(true){
		a=(a*a+1)%n;
		b=(b*b+1)%n;
		b=(b*b+1)%n;
		d=GCD(a-b,n);
		if(d>1 && d<n) {
			printf("%lld",d);
			return d;
		}
		if(d == n) {
			printf("-1");
			return -1;
		}
	}
}
