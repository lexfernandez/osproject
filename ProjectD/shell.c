int strlen(char *str);
char* getFileName(char *source,char *destination);

void main(){
	char str[80];
	char buffer[13312];
	char * aux;
	char f1[6];
	char f2[6];
	int isTyping=1;
	int address=0;
	int t=0;
	while(1){

		syscall_printString("shell >> ");
		syscall_readString(str);
		syscall_printString("\r\n");

		if(str[0]=='c' && str[1]=='l' && str[2]=='e' && str[3]=='a' && str[4]=='r'){
			syscall_clearScreen();
		}
		else if (str[0]=='t' && str[1]=='y' && str[2]=='p' && str[3]=='e' && str[4]==' '){
			getFileName(str+4,f1);
			if(f1[0]==0x00){
				syscall_printString("please enter a valid file name!");
			}else{
				syscall_readFile(f1, buffer);
				if(buffer[0]==0x00)
					syscall_printString("404 - File not found!");
				else
					syscall_printString(buffer);

			}
		}
		else if (str[0]=='e' && str[1]=='x' && str[2]=='e' && str[3]=='c' && str[4]=='u' && str[5]=='t' && str[6]=='e' && str[7]==' '){
			getFileName(str+7,f1);
			syscall_executeProgram(f1,0x2000);
		}
		else if (str[0]=='d' && str[1]=='e' && str[2]=='l' && str[3]=='e' && str[4]=='t' && str[5]=='e' && str[6]==' '){
			getFileName(str+7,f1);
			syscall_deleteFile(f1);
		}
		else if (str[0]=='c' && str[1]=='r' && str[2]=='e' && str[3]=='a' && str[4]=='t' && str[5]=='e' && str[6]==' '){
			getFileName(str+7,f1);

			aux=&buffer;
			syscall_clearScreen();
			address=0;
			while(isTyping){
				syscall_readString(str);
				if(str[0]==0x0A && str[1]==0x00){
					isTyping=0;
				}else{
					for(t=0;t<80;t++){
						*aux=str[t];
						str[t]=0x00;
						aux++;
						address++;
					}
				}
			}
			isTyping=1;
			syscall_printString(f1);
			syscall_printString(buffer);
			syscall_writeFile(f1,buffer,address);
		}
		else if(str[0]=='c' && str[1]=='o' && str[2]=='p' && str[3]=='y' && str[4]==' '){
			aux = getFileName(str+4,f1);
			getFileName(aux,f2);

			if(f1[0]==0x00 || f2[0]==0x00){
				syscall_printString("plese use copy [filename1] [filename2]\r\n");
				syscall_printString("file name must be between 1 - 6 character long\r\n");
			}else{
				syscall_readFile(f1,buffer);

				t=strlen(buffer);

				syscall_writeFile(f2,buffer,t);
				syscall_printString("Copied!");
			}

		}else if(str[0]=='l' && str[1]=='s'){
			syscall_listDirectory();
		}
		else{
			syscall_printString("comando desconocido :D\r\n");
		}


		if(!(str[0]=='c' && str[1]=='l' && str[2]=='e' && str[3]=='a' && str[4]=='r')){
			syscall_printString("\r\n");
		}

	}

}

int strlen(char *str){
	int i=0;

	while(*str!=0x00){
		i++;
		str++;
	}

	return i;
}


char* getFileName(char *source,char destination[]){
	int i=0;
	while(*source==' '){source++;};
	while(*source!=0x00 && *source!=0x0A && *source!=' ' && i<6){
		destination[i] = *source;
		i++;
		source++;
	}
	if(*source!=0x00 && *source!=0x0A && *source!=' ' && *source!='\n' && *source!='\r'){
		syscall_printString("file name is too long!");
		syscall_printString("\r\n");
		i=0;
	}
	while(i<6){
		destination[i] = 0x00;
		i++;
	}

	return source;
}
