//Alex Omar Fernandez Lorenzo
//Github: https://github.com/lexfernandez
//Email: olorenzo@outlook.com


Step 1: Printing to the Screen ­ Interrupt 0x10

Utilizando las llamadas al sistema que nos provee la BIOS podemos utilizar interrupciones
para imprimir en la pantalla, sin necesidad de manejar la posicion del cursor por cuenta
propia.

utilizamos la interrupcion 0x10 para poder imprimir un caracter en la posicion actual del
cursor en la pantalla de la siguiente manera:
	
	interrupt(0x10, 0xe*256+*str, 0, 0, 0);

como queremos imprimir una cadena recorremos la cadena hasta encontrar el caracter \0 que
nos indica el fin de la cadena y para cada caracter de la cadena utilizamos la interrupcion
mencionada anteriormente, ejemplo:

	printString("Hello World\0");

Step 2:  Reading from the keyboard ­ Interrupt 0x16

En este casos utilizamos la interrupcion 0x16 la cual nos permite leer 

	char line[80];
	printString("Enter a line: \0");
	readString(line);
	printString(line);

Step 3 ­ Read a sector from the disk ­ Interrupt 0x13

	char buffer[512];
	readSector(buffer, 30);
	printString(buffer);

Step 4 ­ Create your own interrupt

	makeInterrupt21();
	interrupt(0x21,0,0,0,0);

Step 5 ­ Make printString, readString, and readSector interrupt calls

	char line[80];
	makeInterrupt21();
	interrupt(0x21,1,line,0,0);
	interrupt(0x21,0,line,0,0);
	interrupt(0x21,4,line,0,0);
