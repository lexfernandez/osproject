void main(){
		char str[80];
		syscall_printString("If you see this message, tstpr2 is running correctly!");
		syscall_readString(str);
		syscall_printString("Welcome ");
		syscall_printString(str);
		syscall_printString("\r\n");
}