//Alex Omar Fernandez Lorenzo
//Github: https://github.com/lexfernandez
//Email: olorenzo@outlook.com

int printString(char *str);
// int readChar();
int readString(char *str);
// int mod(int a, int b);
// int div(int a,int b);
//int readSector(char* buffer, int sector);
// void handleInterrupt21(int ax, int bx, int cx, int dx); 

int main(){
	
	//printString("Hello World\0");

	// char line[80];
	// printString("Enter a line: \0");
	// readString(line);
	// printString(line);

	//char buffer[512];
	// readSector(buffer, 30);
	// printString(buffer);

	// makeInterrupt21();
	// interrupt(0x21,0,0,0,0);

	// char line[80];
	// makeInterrupt21();
	// interrupt(0x21,1,line,0,0);
	// interrupt(0x21,0,line,0,0);
	// interrupt(0x21,2,buffer,30,0);
	// printString(buffer);
	// interrupt(0x21,4,line,0,0);

	makeInterrupt21();
	loadProgram();
}
	

int printString(char *str){
	while(*str!='\0'){
		printChar(*str++);
	}
}

// int readChar(){
// 	return interrupt(0x16, 0, 0, 0, 0);
// }

int readString(char *str){
	int i=0;

	while(i<80){
		char c = readChar();
		if(c==0x0D){
			//enter pressed
			str[i] = 0x0A;
			str[i+1] = 0x00;
			break;
		}
		else if(c==0x08){
			//backspace pressed
			if(i>0){
				str[i]=0x00;
				i--;
				printChar(0x8);
                //interrupt(0x10,0xe*256+0x8,0,0,0);
                i++;
                printChar(0x0);
                //interrupt(0x10,0xe*256+0x0,0,0,0);
                i--;
                printChar(0x8);
                //interrupt(0x10,0xe*256+0x8,0,0,0);
			}
		}else{
			str[i]= c;
			printChar(c);
			//interrupt(0x10, 0xe*256+c, 0, 0, 0);
			i++;
		}
	}
	return 1;
}

// int mod(int a, int b){
// 	while(a >= b){
// 		a = a - b;
// 	}
// 	return a;
// }
// int div(int a,int b){
// 	int q = 0;
// 	while( q*b < a ){
// 		q = q+1;
// 	}
// 	return q-1;
// }

// int readSector(char* buffer, int sector){
	
// 	int op = div(sector,18);
// 	int head = mod(op,2);
// 	int DX = head * 256 + 0;

// 	int relativeSector = mod(sector,18) + 1;
// 	int track = div(sector,36);
// 	int CX = track * 256 + relativeSector;
// 	return interrupt(0x13, 2*256+1 , buffer, CX, DX);
// }

 // void handleInterrupt21(int ax, int bx, int cx, int dx){
 	
 // 	switch(ax){
 // 		case 0:
 // 			printString(bx);
 // 			break;
 // 		case 1:
 // 			readString(bx);
 // 			break;
 // 		case 2:
 // 			readSector(bx,cx);
 // 			break;
 // 		default:
 // 			printString("Error handle interrupt 21");
 // 	}

 // }