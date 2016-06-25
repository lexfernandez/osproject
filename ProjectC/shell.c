void main(){
	char str[80];
	char buffer[13312];
	while(1){

		syscall_printString("shell >> ");
		syscall_readString(str);
		syscall_printString("\r\n");

		if(str[0]=='c' && str[1]=='l' && str[2]=='e' && str[3]=='a' && str[4]=='r' && (str[5]==' ' || str[5]=='\n' || str[5]=='\0')){
			syscall_clearScreen();
		}
		else if (str[0]=='t' && str[1]=='y' && str[2]=='p' && str[3]=='e' && str[4]==' '){
			syscall_readFile(str+5, buffer);
			syscall_printString(buffer);
			syscall_printString("\r\n");
		}
		else if (str[0]=='e' && str[1]=='x' && str[2]=='e' && str[3]=='c' && str[4]=='u' && str[5]=='t' && str[6]=='e' && str[7]==' '){
			syscall_executeProgram(str+8,0x2000); 
		}
		else{
			syscall_printString("comando desconocido :D");
			syscall_printString("\r\n");
		}
	}

}