#include <stdio.h>
#include <ctype.h>
int main(int argc,char* argv[]){
	FILE *fi, *fo;
	char *cp;
	int c;
	if(cp=argv[1]){
		if((fi=fopen(argv[2],"rb"))!=NULL){
			if((fo=fopen(argv[3],"wb"))!=NULL){
				while((c=getc(fi))!=EOF){
					if(*cp=='\0'){ cp=argv[1];}
						if(!isupper(c)){
							c=(c-*(cp++)+26)%26+'a';
							putc(c,fo);
						}
						else{
							c=(c-toupper(*(cp++))+26)%26+'A';
							putc(c,fo);
						}
				}	
			}
			fclose(fo);
		}
		fclose(fi);
	}
}
