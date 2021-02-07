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

// Unit Test for bitwise operations 
// Display the binary for "ab"  and compare it with the result of bit_xxx
// Change xxx @ line 54+(29-current_signature_start_line) in mbit_xxx
// to display the result of the corresponding bitwise operation 
void test_bitwise(){

 const unsigned char* m = (const unsigned char*) "ab";
 const unsigned char* m2 = (const unsigned char*) "dc";
 unsigned char mbit_set[2] = "ab"; // Look reminder
 unsigned char mbit_xor[2];
 unsigned char mbit_rot_left[2]="ab";

 // Print "ab" in binary
 printf("ba: "BIT_PATTERN" "BIT_PATTERN"\n",BIT(m[1]),BIT(m[0]));
 printf("dc: "BIT_PATTERN" "BIT_PATTERN"\n",BIT(m2[1]),BIT(m2[0]));
 
 // !!!*****!!!!!***** IMPORTANT REMINDER ******!!!!!!******!!!!!!
 // In C, String litterals CAN NOT be modified => Undefined Behaviour (bus 10) 
 // It is impossible to write the value of x[i] for a given char * x
 // To do that you need to set x as a local array like so 
 // char x[]="something";
 // Of course if y is a char pointer he will be able to receive the address of x
 // char * y = x ; <- No problem
 
 bit_set(mbit_set,5,0);
 bit_xor(m,m2,mbit_xor,16);
 bit_rot_left(mbit_rot_left,16,3);
 // We go from last bit to first to display as in 
 // (10110)base2 and not 01101 
 printf("rr: "); // Visual alignment of the bits
 for(int k=sizeof(m)*2-1;k>=0;k--){ // Note that sizeof(m) returns the size of 
	 			    // one element stored in the char pointer
				    // m ; So since it's char element, 8
				    // There is no way to get the size of a 
				    // pointer in C, because it's dynamic.  
  if(!((k+1)%sizeof(m)) && k!=sizeof(m)*2-1){
   printf(" ");
 }	   
   printf("%d",bit_get(mbit_rot_left,k));
   
 }
 printf("\n");

}

int bit_get(const unsigned char * bits, int pos){
 
  unsigned char  masque = 0x01;
  
  for (int i=0;i<pos%8;i++){
    masque<<=1;    	  
  }
  return (((masque & bits[pos/8])== masque) ? 1 : 0 );	
}

void bit_set(unsigned char *bits, int pos, int etat){
  
  unsigned char masque = 0x01;
  
  for(int i=0;i<pos%8;i++){
    masque<<=1;
  }
  
  if(etat)
     bits[pos/8] = masque | bits[pos/8];	
  else
    bits[pos/8] = ~masque & bits[pos/8];
  return;
}

void bit_xor(const unsigned char *bits1, const unsigned char *bits2,
	     unsigned char *bitsXor, int taille){

for (int i=0;i<taille;i++){
  if(bit_get(bits1,i) != bit_get(bits2,i))
    bit_set(bitsXor,i,1);
  else 
    bit_set(bitsXor,i,0);
}

return;

}

void bit_rot_left(unsigned char* bits, int taille, int nbre){

int fbit;

while(nbre!=0){
  // For every octet in the char * bits ( <=> Every char)
  for(int i=0;i<(taille)/8;i++){ // taille/8 = number of chars stored in bits
    // fbit stores furthest bit to the left in the current octet
    // In (10000000)base2 fbit=1
    fbit=bit_get(&bits[i],7); //&bits[i] = char pointer
    /*if(i==taille/8){
      // Save the value of the last bit 
      lbit=fbit;
    }*/
    /*if(i){
     bit_set(bits,i,fbit);
    }*/
    bits[i]<<=1;
    if(i<(taille)/8-1){	
      bit_set(&bits[i+1],0,fbit);
    }
  }
  nbre--;
}
bit_set(&bits[0],0,fbit);
return ;

}

int main(int argc, char* argv[]){ 
  test_bitwise();
}
