int main(){
// 	char str[80], buffer[512];
// 	syscall_printString("Enter a line: ");
// 	syscall_readString(str);
// 	syscall_printString("You wrote: ");
// 	syscall_printString(str);
// 	syscall_printString("\r\n");
// 	syscall_printString("Reading sector 30 from floppy\r\n");
// 	syscall_printString("\r\n");
// 	syscall_readSector(buffer, 30);
// 	syscall_printString("It read: ");
// 	syscall_printString(buffer);


	// char buffer[13312];
	// /*this is the maximum size of a file*/
	// syscall_readFile("mara18", buffer); /*read the file into buffer*/
	// syscall_printString(buffer);
	// while(1);


	syscall_clearScreen();
	syscall_executeProgram("tstpr2",0x2000); 
	syscall_terminate();

 }
