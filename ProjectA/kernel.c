//Alex Omar Fernandez Lorenzo
//Github: https://github.com/lexfernandez
//Email: olorenzo@outlook.com

#define VIDEO_MEMORY_SEGMENT 0xB000
#define VIDEO_MEMORY_ADDRESS 0x8000
#define COLUMNS 80
#define ROWS 25

void print_char(char c,int color,int row,int column);
void print_text(char *c,int color,int row);


int main(){
	int r;
	int c;
	int color = 0x01;
	int v=0;
	char *a[20];
	a[0] = " Hola Mundo !! ";
	a[1] = " Hello World !! ";
	a[2] = " Salut monde !! ";
	a[3] = " Hallo Welt !! ";
	a[4] = " Alo Mondyal !! ";
	a[5] = "  alert(\"Hello, World\"); ";
	a[6] = " Hallo Heimur !! ";
	a[7] = " Ciao mondo !! ";
	a[8] = " System.Console.Write(\"Hello World\"); ";
	a[9] = " echo(\"Hello World!\"); ";
	a[10] = " Salve mundo !! ";
	a[11] = " Ola mundo !! ";
	a[12] = " Talofa Lalolagi !! ";
	a[13] = " Merhaba Dunya !! ";
	a[14] = " Hello Cruinne !! ";
	a[15] = " std::cout << \"Hello, World.\"; ";
	a[16] = " Bonghjornu Monde !! ";
	a[17] = " Pozdrav svima !! ";
	a[18] = " Salom Dunyo !! ";
	a[19] = " Helo Byd !! ";

	for(r=1;r<=ROWS;r++){
		for(c=1;c<=COLUMNS;c++){
			print_char('\0',color,r, c);
		}
	}

	print_text("SISTEMAS OPERATIVOS I",color,1,'\0');
	print_text("UNIVERSIDAD TECNOLOGICA CENTROAMERICANA (UNITEC)",color,2,'\0');
	print_text("21111044 - ALEX OMAR FERNANDEZ LORENZO",color,3,'\0');
	print_text("MI PRIMER SISTEMA OPERATIVO",color,4,'\0');
	print_text("\0",color+1,5,'-');

	for(r=0;r<20;r++){
		if(r>=13){
			v=-15;
		}
		print_text(a[r],color+2+r+v,6+r,'*');
	}

	while(1==1){

	}
	return 0;
}

void print_char(char c,int color,int row,int column){
	int pos =  (COLUMNS * (row-1))*2 + ((column-1	)*2);
	putInMemory(VIDEO_MEMORY_SEGMENT,VIDEO_MEMORY_ADDRESS + pos, c);
	putInMemory(VIDEO_MEMORY_SEGMENT,VIDEO_MEMORY_ADDRESS + pos+1, color);
}

void print_text(char *text,int color,int row,char decorator){
	int i=1;
	int offset=1;

	while(text[offset]!='\0'){
		offset++;
	}

	offset = (COLUMNS - offset)/2;

	if(decorator!='\0'){
		for(i;i<offset;i++){
			print_char(decorator,color,row,i);
		}	
	}else{
		i=offset;
	}

	while (*text != '\0' && i<=COLUMNS) {
		print_char(*text,color,row,i);
		i++;
		text++;
	}

	if(decorator!='\0'){
		for(i;i<=COLUMNS;i++){
			print_char(decorator,color,row,i);
		}	
	}	
}
