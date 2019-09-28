#include <stdio.h>

typedef unsigned long long int huge; // set an int of at least 64 bits 
				     //	deny overflow & segfault issues with big keys
			 	     // Limitation : slower on 32 bit machines

static huge modexp(huge a, huge b, huge n){ // Computes (a^b)mod(n)
	huge y;
	y=1;
	while(b!=0){
		if(b & 1)
		  y=(y*a)%n;
		a = (a*a)%n;
		b=b>>1;
	}
	return y;
}
huge rsa_encrypt(huge e, huge n, huge m){
	return modexp(m,e,n);	
}
huge rsa_decrypt(huge c,huge d, huge n){
	return modexp(c,d,n);
}

int gcd(int a,int b){ // Euler Algorithm
  if(!b){
	return a;
  }else{
	b=a%b; 
  	a=b;
  }	  
}

int gcdRecursive(int a, int b){
  int r=a%b;
  if(!r){
    return b;
  }
  else{
   return gcdRecursive(b,r);
  }    
}

// Gcd Extended Comprehensive Guide
// Let a=35, b=15
// 1st call (from main) = gcdExtended(35,15,x,y)
// 2nd call (from gcd) = gcdExtended(15,35,x,y)
// 3rd call (from (gcd)o(gcd) = gcdExtended(35%15,35,x,y)
// 4th call + is classic gcdRecursive
int gcdExtended(int a, int b, int *x, int *y){
 int x1,y1; // Save Bezout Coeff for each (a,b)^i recursive iteration
 if(!a){ // If the last b%a=0, then gcd(b%a,a) = gcd(0,a) = a = 0*x1 + 1*y1   
   *x=0;
   *y=1;
   return b; // return the gcd which is the last Non Null remainder
  }
 else{
   int gcd=gcdExtended(b%a,a,&x1,&y1); 
   /*
    * Lines Below computes recursively from last to first
    * gcd(a,b) = ax+by = gcd(b%a,a)
    * b%a=b-E(b/a)*a
    * gcd(b%a,a)=(b-E(b/a)*a)*x1 + a*y1
    * Where x1 and y1 are the intermediary coefficient for each recursive iteration
    * x1 and y1 are the bezout coefficient for the first two inputs of gcdRecursive();
    * x1 and y1 recursively regress to x and y until the final computation which 
    * corresponds to the first call 
    * Meaning that the last x and y corresponds to Bezout Coefficient for a and b. 
    ********************* 
    * Unsolved Question : Can I compute the same program with (a%b,b) instead of (b%a,a)?
    * In that case we would get *x=x1 ; *y=(-a/b)x1+y1 
    * Which makes the value of x1=0 recursively regress to x 
    * Why ? 
    */
   *x=-(b/a)*x1+y1;
   *y=x1;
   return gcd;
 }
}

//Computes the secret key non-recursively (Faster than recursive execution 
// because compiler doesn't have to reallocate space for function every iteration)
int secret_keyGen(int e,int phi){
	int d=2;
	if(gcd(e,phi)){	
	  while((e*d)%phi!=1){
	    d+=1;       	  
	  }
	  return d;
	}
	return -1;
}

// Computes the secret key recursively using gcdExtended algorithm
int secret_keyGen2(int e,int phi){
	int x,y; // Set the return variables for the Bezout coefficients in ax+by=1
	int r=gcdExtended(e,phi,x,y); // Solves (x,y) in  e*x + phi*y = 1
	return x; // e*x + phi*y = 1 <=> e*x = 1 mod(phi) <=> x = e^-1 mod(phi) 
}

int isPrime(huge a){
  int max=3;
  if(!(a%2)){
    return 0;
  }
  while(max<sqrt(a)){
  	if(!(a%max)){
	  return 0;
	}
	max+=2;
  }
  return 1;
}

int main(int argc, char* argv[]){
	printf("%d",rsa_decrypt(rsa_encrypt((huge)967,(huge)4247,(huge)3333),(huge)2983,(huge)4247));
	int x,y;
	int r= gcdExtended(40,7,&x,&y);
	int d1= secret_keyGen(7,40);
	int d2= secret_keyGen(7,40);
	printf("\n %d %d",x,y); 
	printf("\n %d", d2);
}
