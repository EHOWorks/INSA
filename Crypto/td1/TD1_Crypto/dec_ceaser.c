#include <stdio.h>
	int main(int argc, char* argv[]){
		FILE *fi, *fo;
		char *cp;
		int c;
		if((cp=argv[1])){
			if((fi=fopen(argv[2],"rb"))!=NULL){
				if((fo=fopen(argv[3],"wb"))!=NULL){
					while((c=getc(fi))!=EOF){
						c+=*(cp);
						putc(c,fo);	
					}
				}
				fclose(fo);
			}
			fclose(fi);
		}
	}
