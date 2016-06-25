;kernel.asm
;Michael Black, 2007

;kernel.asm contains assembly functions that you can use in your kernel

;Alex Omar Fernandez Lorenzo
;Github: https://github.com/lexfernandez
;Email: olorenzo@outlook.com

	.global _readSector
	.global _readChar
	.global _printChar
	.global _putInMemory
	.global _interrupt
	.global _makeInterrupt21
	.global _loadProgram
;	.extern _handleInterrupt21
	.extern _printString
	.extern _readString


_readSector:
	push bp
	mov bp,sp
	sub sp,#6

	mov bx,[bp+4]   ;buffer
	mov ax,[bp+6]	;sector
	mov cl,#36		
	div cl
	xor ah,ah 		;set higher bits = 0
	mov [bp-2],ax  	;guardar track

	mov ax,[bp+6]	;sector
	mov cl,#18
	div cl
	and al,#0x1
	xor dx,dx 		;set higher bits = 0
	mov dl,al
	mov [bp-4],dx   ;head

	add ah,#1
	xor dx,dx
	mov dl,ah
	mov [bp-6], dx  ;relative sector

	mov al,#0x1 	;sectors to read
	mov dl, #0x0    ;device number
	mov ch,[bp-2]  	;track number
	mov cl, [bp-6]	;relative number
	mov ah, #0x2 	;read a sector
	mov dh,[bp-4]  	;head number
	int #0x13
	add sp,#6
	pop bp
	ret


;int readChar()
_readChar:
	push bp
	mov bp,sp
	mov ah,#0x00
	int #0x16
	pop bp
	ret

;void printChar(char ch)
_printChar:
	push bp
	mov bp,sp
	mov al, [bp+4]
	mov ah, #0x0e
	int #0x10
	pop bp
	ret	
	
;void putInMemory (int segment, int address, char character)
_putInMemory:
	push bp
	mov bp,sp
	push ds
	mov ax,[bp+4]
	mov si,[bp+6]
	mov cl,[bp+8]
	mov ds,ax
	mov [si],cl
	pop ds
	pop bp
	ret

;int interrupt (int number, int AX, int BX, int CX, int DX)
_interrupt:
	push bp
	mov bp,sp
	mov ax,[bp+4]	;get the interrupt number in AL
	push ds		;use self-modifying code to call the right interrupt
	mov bx,cs
	mov ds,bx
	mov si,#intr
	mov [si+1],al	;change the 00 below to the contents of AL
	pop ds
	mov ax,[bp+6]	;get the other parameters AX, BX, CX, and DX
	mov bx,[bp+8]
	mov cx,[bp+10]
	mov dx,[bp+12]

intr:	int #0x00	;call the interrupt (00 will be changed above)

	mov ah,#0	;we only want AL returned
	pop bp
	ret

;void makeInterrupt21()
;this sets up the interrupt 0x21 vector
;when an interrupt 0x21 is called in the future, 
;_interrupt21ServiceRoutine will run

_makeInterrupt21:
	;get the address of the service routine
	mov dx,#_interrupt21ServiceRoutine
	push ds
	mov ax, #0	;interrupts are in lowest memory
	mov ds,ax
	mov si,#0x84	;interrupt 0x21 vector (21 * 4 = 84)
	mov ax,cs	;have interrupt go to the current segment
	mov [si+2],ax
	mov [si],dx	;set up our vector
	pop ds
	ret

;this is called when interrupt 21 happens
;it will call your function:
;void handleInterrupt21 (int AX, int BX, int CX, int DX)
_interrupt21ServiceRoutine:
	push dx
	push cx
	push bx
	push ax
	call _handleInterrupt21
	pop ax
	pop bx
	pop cx
	pop dx

	iret

_handleInterrupt21:
	push bp
	mov bp,sp
	mov ax,[bp+4]
	cmp ax,#0
	je CallPrintString
	cmp ax,#1
	je CallReadString
	cmp ax,#2
	je CallReadSector

end21Handle:
	pop bp
	ret

CallPrintString:
	mov ax,[bp+6]
	push ax
	call _printString
	add sp,#2
	jmp end21Handle


CallReadString:
	mov ax,[bp+6]
	push ax
	call _readString
	add sp,#2
	jmp end21Handle

CallReadSector:
	mov ax,[bp+6]
	mov bx,[bp+8]
	push bx
	push ax
	call _readSector
	add sp,#4
	jmp end21Handle

; Load a program from sector 11 into segment 0x20000
_loadProgram:
	mov ax, #0x2000
	mov ds, ax
	mov ss, ax
	mov es, ax
	
	;let's have the stack start at 0x2000:fff0
	mov ax, #0xfff0
	mov sp, ax
	mov bp, ax
	
	; Read the program from the floppy
	mov cl, #12	;cl holds sector number
	mov dh, #0 ;dh holds head number - 0
	mov ch, #0 ;ch holds track number - 0
	mov ah, #2 ;absolute disk read
	mov al, #1 ;read 1 sector
	mov dl, #0 ;read from floppy disk A
	mov bx, #0 ;read into offset 0 (in the segment)
	int #0x13 ;call BIOS disk read function

	; Switch to program
	jmp #0x2000:#0
