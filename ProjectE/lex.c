void main(){
		char str[80];
		syscall_printString("Whats your name?");
		syscall_readString(str);
		syscall_printString("Welcome ");
		syscall_printString(str);
		syscall_printString("\r\n");
		syscall_terminate();
}
