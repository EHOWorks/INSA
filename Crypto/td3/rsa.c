#include <stdio.h>
#include <stdlib.h>
#include <sodium.h> // Library for Cryptographically secure Randomization
#include <math.h>

typedef unsigned long long int huge; // set an int of at least 64 bits 
				     //	deny overflow & segfault issues with big keys
			 	     // Limitation : slower on 32 bit machines

// Modexp Comprehensive Guide 
// The point of this algorithm is to compute (a^b)%n , 2^n time faster than a regular multiplicative approach
// Let b = (100100)base2 = (2^2 + 2^5)base10 = (36)base10
// So we can write a^b = a^(2^2)*a^(2^5) = a^4*a^32 ( <- remember this) =a^36
// What the while loop does is : 
// I1 : a^2 ; b >> 1 = (010010)base2
// I2 : (a^2)*(a^2) = a^4 ; b >> 1 = (001001)base2
// I3 : b & 1 = true =>  a^4 -> y : We store the value of (a) to the lowest pow of 2 contained in b so in this case 2^2 
// Still I3 : (a^4)*(a^4) = a^8 ; b >> 1 = (000100)base2
// The algorithm got rid of the first pow of 2 in b which was 2^2 but still has to compute the remaining pow in 2^5
// So he right shifts 2 more times until 
// I5 : (a^16)*(a^16) = a^32 ; b >> 1 = (000001) base2
// I6 : b & 1 = true => a^16 * y -> y <=> a^16 * a^4 -> y Remember that y already had a^4 in it from the I3 iteration
// I6 : b >> 1 = (000000) base2
// There is no more round to compute after 6 since the loop reaches end when b==0 which is the case now
// We return y which is exactly a^b%n <=> a^Sigma(2^i)%n Where Sigma(2^i) is the binary sum of b (2^2+2^5 in prev. exple) 
static huge modexp(huge a, huge b, huge n){ // Computes (a^b)mod(n)
	huge y; 
	y=1;
	while(b!=0){
		if(b & 1) // If the power is odd, save a^(2^i) in the buffer y
		  y=(y*a)%n; 
		a = (a*a)%n;// Iteration 1 : a=a^2 , I2 : a=a^4 ; I4 : a=a^8 ; In : a=a^(2^n)
		b=b>>1; // Divides the power by 2 (Meaning that the complexity of the algorithm is reduced by 2^n factor)
			// If b is odd, removes 1 and divides the remainder by 2 
	}
	return y;
}
huge rsa_encrypt(huge e, huge n, huge m){
	return modexp(m,e,n);	
}
huge rsa_decrypt(huge c,huge d, huge n){
	return modexp(c,d,n);
}

huge genPrime(huge N){
  huge prime;
  if(sodium_init() < 0 ){
	  // Checks if the sodium library loaded correctly
	  return -1;
  }
  while(!(isPrime(prime=randombytes_uniform(N)))){
  }
  return prime;
}

huge gcd(huge a,huge b){ // Euler Algorithm
  while(b!=0){
    huge save = b;  
    b=a%b; 
    a=save;
  }
  return a;  
}

huge gcdRecursive(huge a, huge  b){
  huge r=a%b;
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
huge gcdExtended(huge a, huge b, huge  *x, huge *y){
 huge x1,y1; // Save Bezout Coeff for each (a,b)^i recursive iteration
 if(!a){ // If the last b%a=0, then gcd(b%a,a) = gcd(0,a) = a = 0*x1 + 1*y1   
   *x=0;
   *y=1;
   return b; // return the gcd which is the last Non Null remainder
  }
 else{
   huge gcd=gcdExtended(b%a,a,&x1,&y1); 
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

//Computes the secret key non-recursively (Requires less memory because compiler doesn't reallocate space at each function call as in the recursive way)
huge secret_keyGen(huge e,huge phi){
	huge d=2;
	if(gcd(e,phi)){	
	  while((e*d)%phi!=1){
	    printf("d: %lld e: %lld  phi:%lld \n", d, e,phi);
	    d+=1;       	  
	  }
	  return d;
	}
	return -1;
}

// Computes the secret key recursively using gcdExtended algorithm
huge secret_keyGen2(huge e,huge phi){
	huge x,y; // Set the return variables for the Bezout coefficients in ax+by=1
	huge r=gcdExtended(e,phi,&x,&y); // Solves (x,y) in  e*x + phi*y = 1
	return x; // e*x + phi*y = 1 <=> e*x = 1 mod(phi) <=> x = e^-1 mod(phi) 
}

// Check if a number is prime : 1 if prime, 0 if not
int isPrime(huge a){
  int max=3;
  if(!(a%2)){
    return 0;
  }
  while(max<sqrt(a)){
  	if(!(a%max)){
	  return 0;
	}
	max+=2; // If a is prime he must not be even
  }
  return 1;
}
int keyGen(huge * e, huge * d,huge *  phi, huge N){
	huge prime1=genPrime(N),prime2;
	while((prime2=genPrime(N))==prime1){
	}
	*phi=(prime1-1)*(prime2-1);
	*e=genPrime(*phi); // NB : Normally e should be coprime with phi, but 
			 // this doesn't mean that it should be a prime number
			 // I use genPrime() here for convenience
	*d=secret_keyGen2(*e,*phi)%*phi; // Using the recursive generator
	return 0;
}
int main(int argc, char* argv[]){	
	huge N = 99999999999; // Upper Limit for Prime Generation
	huge e,d,phi;
	keyGen(&e,&d,&phi,N);
	/*************************** UNIT TESTS *******************************/
	// ***************** Unit Test rsa_descrypt, rsa_encrypt ************
	//printf("%d",rsa_decrypt(rsa_encrypt((huge)967,(huge)4247,(huge)3333),(huge)2983,(huge)4247));
	//********************* Unit Test modexp *****************
	//modexp(2,4,5);
	//******************** Unit Test gcdExtended **************
	/*
	huge x,y;
	huge r= gcdExtended(40,7,&x,&y);
	printf("\n %d %d",x,y);
        */	
	//******** Unit Test secret_keyGen recursive(2) and non recursive(1) ***
	/*
	huge d1= secret_keyGen(7,40);	
	huge d2 = secret_keyGen2((huge)7,(huge)40);
	printf("\n %d \n",(int)d2);
	*/
	/*********************** Unit Test prime generators **************/
	//printf("Prime 1 : %d \n Prime 2: %d", prime1, prime2);
	/*********************** Unit Test keyGen *****************/
	printf("e : %lld , d: %lld", (int)e, (int)d);
}	
