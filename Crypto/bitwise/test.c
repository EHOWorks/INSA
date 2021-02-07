#include <stdio.h>
#include <string.h>

void modifyCharPointer( unsigned char * charPointer){
	charPointer=(unsigned char*)"aaabbbbaaaa";
}

int main(int argc, char* argv[]){

  unsigned char * charPointer = (unsigned char *) "abc";
 // charPointer = "aaaaaaaaa";
  //charPointer[0]++;
  modifyCharPointer(charPointer);
  printf("%s",charPointer);
}
