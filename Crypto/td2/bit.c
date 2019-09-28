#include <string.h>
#include <stdio.h>
#include "bit.h"
/*
 * In #define x y 
 * x > identifier (can be parameterized)
 * y > token-string (must use given parameters)
 * Define the parameterized identifier BIT(byte) 
 * that displays the binary of a given byte.
 * The backslach '\' allows for line concatenation.  
 */
#define BIT_PATTERN "%d%d%d%d%d%d%d%d"
#define BIT(byte) \
	(byte & 0x80 ? 1 : 0), \
	(byte & 0x40 ? 1 : 0), \
	(byte & 0x20 ? 1 : 0), \
	(byte & 0x10 ? 1 : 0), \
	(byte & 0x08 ? 1 : 0), \
	(byte & 0x04 ? 1 : 0), \
	(byte & 0x02 ? 1 : 0), \
	(byte & 0x01 ? 1 : 0)

int main(int argc, char* argv[]){ 
 const unsigned char* m = (const unsigned char*) "ab";
 printf(BIT_PATTERN" "BIT_PATTERN"\n",BIT(m[0]),BIT(m[1]));
 for(int k=sizeof(m)*2-1;k>=0;k--){
  if(!((k+1)%sizeof(m)) && k!=sizeof(m)*2-1){
   printf(" ");
 }	   
   printf("%d",bit_get(m,k));
   
 }
 printf("\n");
}

int bit_get(const unsigned char * bits, int pos){
 
  unsigned char masque;
  int i;

  masque = 0x01;
  for (i=0;i<pos%8;i++){
    masque<<=1;    	  
  }
  return (((masque & bits[pos/8])== masque) ? 1 : 0 );	
}

