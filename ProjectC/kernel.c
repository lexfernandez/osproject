//Alex Omar Fernandez Lorenzo
//Github: https://github.com/lexfernandez
//Email: olorenzo@outlook.com

void printString(char *str);
// int readChar();
void readString(char *str);
void handleInterrupt21 (int AX, int BX, int CX, int DX);
int readFile(char *filename, char *buffer);
int searchFileInDirectory(char *directory, char * filename);
void cleanBuffer(char *buffer);
void executeProgram(char* name, int segment);
//void terminate();
// int mod(int a, int b);
// int div(int a,int b);
//int readSector(char* buffer, int sector);
// void handleInterrupt21(int ax, int bx, int cx, int dx); 
char p[6];

void main(){
	
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
	//loadProgram();
	executeProgram("shell",0x2000);

}
	

void printString(char *str){
	while(*str!='\0'){
		printChar(*str++);
	}
}

// int readChar(){
// 	return interrupt(0x16, 0, 0, 0, 0);
// }

void readString(char *str){
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

void handleInterrupt21 (int AX, int BX, int CX, int DX){
	if (AX == 0){
	    printString(BX);
	}
	else if (AX ==1){
	    readString(BX);
	}
	else if(AX == 2){
	    readSector(BX,30);
	}
	else if (AX == 3){
		readFile(BX, CX);
	}
	else if(AX == 4){
		executeProgram(BX,CX);
	}
	else if(AX == 5){
		terminate();
	}
	else if(AX == 6){
		clear();
	}
	else{
	    printString("La interrupcion no existe!\0");
	}
}

int readFile(char *filename, char *buffer){
	int file = 0;
	int i=6;
	int sectors[26];
	int address=0;

	readSector(buffer,2);

	file = searchFileInDirectory(buffer,filename);
	if(file<0){
		cleanBuffer(buffer);
		return -1; //File was not found
	}

	file*=32; //set the rigth file offset
	for(i=0;i<26;i++){
		sectors[i]=buffer[file+i+6]; //pass each sector number to sectors array
	}

	cleanBuffer(buffer);

	for(i=0;i<26;i++){
		if(sectors[i]==0) //check if it is the last sector 
			break; 
		readSector(buffer+address,sectors[i]); //read the sector at sectors[i]
		address+=512; //create more space for the next sector to be read
	}

	return file;
}

//void listDirectory(char *buffer,char )

void cleanBuffer(char *buffer){
	int bufferSize=26*512;
	int i=0;
	for(i;i<bufferSize;i++){
		buffer[i] = 0x0;
	}
}

int searchFileInDirectory(char *directory, char * filename){
	int i,j;
	int entryOffset;
	int wasFound = 0;
	//check the 16 entries of the directory
	for(i=0; i<16; i++){

		entryOffset=i*32;
		//printString("looking at position "+0);
		if(directory[entryOffset]!='\0'){
			for (j = 0; j < 6; j++)
			{
				if(	directory[entryOffset+j]=='\0' || directory[entryOffset+j]=='\n' || directory[entryOffset+j]=='\r' ||
					filename[j]=='\0' || filename[j]=='\n' || filename[j]=='\r'){
					break;
				}

				if(directory[entryOffset+j]==filename[j]){
					wasFound=1;
				}else{
					wasFound=0;
					break;
				}
			}

			if(wasFound){
				//printString("Found at position "+0);
				return i;
			}
		}
	}
	//printString("404 - Not Found");
	return -1;
}

void executeProgram(char* name, int segment){
	char buffer[26*512];
	int i;
	int bufferSize=26*512; //26 sector , 512 bytes per sector
	int t= readFile(name,buffer);

	if(t>-1){
		for (i=0; i<13312;i++){
			moveToSegment(segment,i,buffer[i]);
		} 

		launchProgram(segment);
	}

}

// void terminate(){
// 	//while(1);
// 	p[0]='s';
// 	p[1]='h';
// 	p[2]='e';
// 	p[3]='l';
// 	p[4]='l';
// 	p[5]='\0';
// 	executeProgram(p,0x2000);
// }
