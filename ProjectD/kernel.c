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
int writeSector(char *buffer, int sector);
void deleteFile(char* name);
void writeFile(char* name, char* buffer, int size);
void listDirectory();
//void terminate();
int mod(int a, int b);
int div(int a,int b);
int strlen(char * str);
char* itoa(int num, char* buffer, int base);
void reverse(char str[], int length);
void swap(int *i, int *j);
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
	while(*str!=0x00){
		//printhex(*str);
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

int mod(int a, int b){
	while(a >= b){
		a = a - b;
	}
	return a;
}
int div(int a,int b){
	int q = 0;
	while( q*b < a ){
		q = q+1;
	}
	return q-1;
}

int strlen(char * str){
	int i=0;
	while(*str!=0){
		i++;
		str++;
	}

	return i;
}

// int readSector(char* buffer, int sector){

// 	int op = div(sector,18);
// 	int head = mod(op,2);
// 	int DX = head * 256 + 0;

// 	int relativeSector = mod(sector,18) + 1;
// 	int track = div(sector,36);
// 	int CX = track * 256 + relativeSector;
// 	return interrupt(0x13, 2*256+1 , buffer, CX, DX);
// }


int writeSector(char *buffer, int sector){

	int op = div(sector,18);
	int head = mod(op,2);
	int DX = head * 256 + 0;

	int relativeSector = mod(sector,18) + 1;
	int track = div(sector,36);
	int CX = track * 256 + relativeSector;
	return interrupt(0x13, 3*256+1 , buffer, CX, DX);
}

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
	else if (AX == 1){
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
		writeSector(BX,CX);
	}
	else if(AX == 7){
		deleteFile(BX);
	}
	else if(AX == 8){
		writeFile(BX,CX,DX);
	}
	else if(AX == 9){
		listDirectory();
	}
	else if(AX == 10){
		clear();
	}
	else{
	    printString("La interrupcion no existe!\r\n");
	}
}

int readFile(char *filename, char *buffer){
	int file = -1;
	int i=6;
	char t[100];
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
		if(sectors[i]==0x00) //check if it is the last sector
			break;
		readSector(buffer+address,sectors[i]); //read the sector at sectors[i]
		address+=512; //create more space for the next sector to be read
	}

	if(i==0){
		buffer[0]==0x0A;
		buffer[1]==0x00;
	}

	return file;
}

void deleteFile(char name[]){
	char map[512];
	char directory[512];
	int sectors[26];
	int file;
	int i;
	int sector;

	readSector(map,1);
	readSector(directory,2);

	file = searchFileInDirectory(directory,name);

	if(file>=0){
		file*=32; //set the rigth file offset
		for(i=0;i<26;i++){
			sectors[i]=directory[file+i+6]; //pass each sector number to sectors array
		}

		//Set directory entry to 00000000000000000000000000000000
		for(i=0; i < 32; i++){
			directory[file+i] = 0x0;
		}
		//Set map sector to 0x00 fir every sector position
		for(i=0;i<26;i++){
			sector=sectors[i];
			if(sector==0) //check if it is the last sector
				break;
			map[sector]=0x0;
		}
		writeSector(map,1);
		writeSector(directory,2);
	}else{
		printString("File not found!\r\n");
	}
}

void writeFile(char* name, char* buffer, int size){
	char map[512];
	char directory[512];
	char chunk[512];
	int entryIndex;
	int hasFreeEntry=0;
	int name_length = 0;
	int i = 0,j=0;
	int numberOfSectors = 0;
	int mapIndex=0;
	int bufferIndex = 0;

	readSector(map,1);
	readSector(directory,2);

	for (entryIndex = 0; entryIndex < 16; entryIndex++){
		if (directory[32*entryIndex] == 0x00){
			hasFreeEntry = 1;
			break;
		}
	}

	if (hasFreeEntry == 0){
		printString("No empty entries!\r\n");
		return;
	}

	name_length = strlen(name);

	for(i=0; i< 6; i++){
		directory[32*entryIndex+i] = 0x00;
		if(name_length>i){
			directory[32*entryIndex+i] = name[i];
		}
	}

	numberOfSectors=size/512;
	if(size<512){
		numberOfSectors=1;
	}

	for (i = 0; i < numberOfSectors; i++){

		while(map[mapIndex] != 0x0){
			mapIndex++;
		}
		if (mapIndex==26)
		{
			printString("Not enough space in directory\r\n");
			return;
		}

		map[mapIndex] = 0xFF;

		directory[32*entryIndex+6+i] = mapIndex;

		for(j=0;j<512;j++){
			if(bufferIndex<size){
				chunk[j] = buffer[bufferIndex];
			}else{
				chunk[j] = 0x00;
			}
			bufferIndex++;

		}
		writeSector(chunk,mapIndex);
	}


	for(i=6+numberOfSectors; i < 32; i++){
		directory[32*entryIndex+i] = 0x00;
	}

	writeSector(map,1);
	writeSector(directory,2);
}

void listDirectory(){
	char buffer[512];
	char directory[512];
	char sector[512];
	char itoaSize[100];
	int i,j;
	int index = 0;
	int size=0;
	for(i=0;i<512;i++){
		buffer[i] = 0x0;
		directory[i]=0x0;
		sector[i]=0x00;
	}

    readSector(directory, 2);

	for(i=0;i<16;i++){

		if (directory[32*i] != 0x00){

			for (j=0; j < 6; j++){
				if(directory[j+32*i]!=0x00){
					buffer[index] = directory[j+32*i];
					index++;
				}

			}

			buffer[index] = '\t';
			index++;

			for (j=6; j < 32; j++){
				if(directory[j+32*i]==0x00){
					break;
				}
			}
			//j=j-1;//real amount of sectors
			if((j-6)>=0){
				size = (j-6)*512;
				//readSector(sector, directory[j+32*i]);
				//for(j=0; j<512;j++){
				//	if(sector[j]==0x00){
				//		break;
				//	}
				//	size++;
				//}
			}
			//size=div(size,1024); // bytes to kilobytes
			itoa(size, itoaSize, 10);
			for(j=0;j<100;j++){
				if(itoaSize[j]==0x00){
					break;
				}
				buffer[index] = itoaSize[j];
				index++;
			}
			buffer[index] = 'B';
			index++;

			buffer[index] = '\r';
			index++;
			buffer[index] = '\n';
			index++;


		}
	}

	for(i=0;i<512;i++){
		if(buffer[i]!=0x00){
			printChar(buffer[i]);
		}
	}
}

void cleanBuffer(char *buffer){
	int bufferSize=26*512;
	int i=0;
	for(i;i<bufferSize;i++){
		buffer[i] = 0x0;
	}
}

int searchFileInDirectory(char *directory, char * filename){
	int i=0,j=0;
	int entryOffset;
	int wasFound;
	char str[20];
	//check the 16 entries of the directory
	for(i=0; i<16; i++){
		wasFound = 0;
		entryOffset=i*32;
		if(directory[entryOffset]!=0x00){
			for (j = 0; j < 6; j++)
			{
				if(directory[entryOffset+j]==0x00 && filename[j]!=0x00){
					wasFound=0;
					break;
				}else if(directory[entryOffset+j]!=0x00 && filename[j]==0x00){
					wasFound=0;
					break;
				}else if(directory[entryOffset+j]==0x00 && filename[j]==0x00){
					wasFound=1;
					break;
				}
				else if(directory[entryOffset+j]==filename[j]){
					wasFound=1;
				}else{
					wasFound=0;
					break;
				}
			}

			if(wasFound){
				return i;
			}
		}
	}
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

void swap(char *i, char *j) {
   char t = *i;
   *i = *j;
   *j = t;
}

void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        swap((str+start), (str+end));
        start++;
        end--;
    }
}

char* itoa(int num, char* str, int base)
{
    int i = 0;
    int isNegative = 0;

    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = 1;
        num = -num;
    }

    // Process individual digits
    while (num != 0)
    {
        int rem = mod(num, base);
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }

    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0'; // Append string terminator
    // Reverse the string
    reverse(str, i);

    return str;
}

// void terminate(){
// 	//while(1);
// 	p[0]='s';
// 	p[1]='h';
// 	p[2]='e';
// 	p[3]='l';
// 	p[4]='l';
// 	p[5]=0x00;
// 	executeProgram(p,0x2000);
// }
