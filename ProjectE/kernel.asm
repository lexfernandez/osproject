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
	.global _moveToSegment
	.global _launchProgram
	.global _printhex
	.global _setCursor
	.global _clear
	;.global _terminate

	.extern _handleInterrupt21
	.extern _printString
	.extern _readString

	.global _irqInstallHandler
	.global _setTimerPhase
	.global _timerISR
	.extern _handleTimerInterrupt
	.global _setKernelDataSegment
	.global _restoreDataSegment
	.extern _saveSPToCPB
	.extern _scheduleProcess
	.global _copyToSeg

	.data
		.extern _currentProcess

	.text

;_terminate:
;	retf

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

;_handleInterrupt21:
;	push bp
;	mov bp,sp
;	mov ax,[bp+4]
;	cmp ax,#0
;	je CallPrintString
;	cmp ax,#1
;	je CallReadString
;	cmp ax,#2
;	je CallReadSector

;end21Handle:
;	pop bp
;	ret

;CallPrintString:
;	mov ax,[bp+6]
;	push ax
;	call _printString
;	add sp,#2
;	jmp end21Handle


;CallReadString:
;	mov ax,[bp+6]
;	push ax
;	call _readString
;	add sp,#2
;	jmp end21Handle

;CallReadSector:
;	mov ax,[bp+6]
;	mov bx,[bp+8]
;	push bx
;	push ax
;	call _readSector
;	add sp,#4
;	jmp end21Handle


;this is called to start a program that is loaded into memory
;void launchProgram(int segment)
_launchProgram:
	mov bp,sp
	mov bx,[bp+2]	;get the segment into bx

	mov ax,cs	;modify the jmp below to jump to our segment
	mov ds,ax	;this is self-modifying code
	mov si,#jump
	mov [si+3],bx	;change the first 0000 to the segment

	mov ds,bx	;set up the segment registers
	mov ss,bx
	mov es,bx

	mov sp,#0xfff0	;set up the stack pointer
	mov bp,#0xfff0

jump:	jmp #0x0000:0x0000	;and start running (the first 0000 is changed above)

;printhex is used for debugging only
;it prints out the contents of ax in hexadecimal
_printhex:
        push bx
        push ax
        push ax
        push ax
        push ax
        mov al,ah
        mov ah,#0xe
        mov bx,#7
        shr al,#4
        and al,#0xf
        cmp al,#0xa
        jb ph1
        add al,#0x7
ph1:    add al,#0x30
        int 0x10

        pop ax
        mov al,ah
        mov ah,#0xe
        and al,#0xf
        cmp al,#0xa
        jb ph2
        add al,#0x7
ph2:    add al,#0x30
        int 0x10

        pop ax
        mov ah,#0xe
        shr al,#4
        and al,#0xf
        cmp al,#0xa
        jb ph3
        add al,#0x7
ph3:    add al,#0x30
        int 0x10

        pop ax
        mov ah,#0xe
        and al,#0xf
        cmp al,#0xa
        jb ph4
        add al,#0x7
ph4:    add al,#0x30
        int 0x10

        pop ax
        pop bx
        ret


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

;void moveToSegment (int segment, int address, char character)
_moveToSegment:
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

_clear:
	push bp
	mov bp,sp
	push ds

	mov ah, #0x07
	mov al, #0x00 ; (00 = clear entire windows )
	mov bh, #0x07 ; set font white bg black
	mov ch, #0x00 ; row = 0
	mov cl, #0x00 ; col =0
	mov dh, #0x18 ; row = 24 (0x18)h
	mov dl, #0x4f ; col = 79 (0x4f)h
	int 0x10      ; call interruption

	call _setCursor

	pop ds
	pop bp
	ret


_setCursor:
	mov ah, #2
	mov bh, #0
	mov dh, #0
	mov dl, #0
	int #0x10
	ret

;void irqInstallHandler(int irq_number, void (*fn)())
_irqInstallHandler:
	cli
	push bp
	mov bp, sp
	push si
	push ds
	mov dx, [bp+6] ; Function pointer
	xor ax, ax
	mov ds, ax ; Interrupt vector is at lowest memory
	mov si, [bp+4]
	shl si, 2 ; ax = irq_handler * 4
	mov ax, cs
	mov [si + 2], ax
	mov [si], dx
	pop ds
	pop si
	pop bp
	sti
	ret

; void setTimerPhase(int hz)
_setTimerPhase:
	push bp
	mov bp, sp
	mov dx, #0x0012 ; Default frequency of the timer is 1,193,180 Hz
	mov ax, #0x34DC
	mov bx, [bp+4]
	div bx
	mov bx, ax ; Save quotient
	mov dx, #0x43
	mov al, #0x36
	out dx, al ; Set our command byte 0x36
	mov dx, #0x40
	mov al, bl
	out dx, al ; Set low byte of divisor
	mov al, bh
	out dx, al ; Set high byte of divisor
	pop bp
	ret

_timerISR:
	;Disable interrupts
	cli
	;Push all the registers on the stack
	push bx
	push cx
	push dx
	push si
	push di
	push bp
	push ax
	push ds
	push es

	mov bx,sp ;save the stack pointer (sp register) in the current PCB
	push bx
	call _saveSPToCPB

	;Reset the interrupt controller
	mov al,#0x20
	out #0x20,al
	;Switch to kernel data segment
	mov ax,#0x1000
	mov ds,ax

	mov ss,ax
	mov sp,#0xfff0
	push bx
	;call _handleTimerInterrupt
	call _scheduleProcess
	mov bx, [_currentProcess]

	mov sp,[bx+2]
	mov ss,[bx+4]

	;Restore the registers.
	pop es
	pop ds
	pop ax
	pop bp
	pop di
	pop si
	pop dx
	pop cx
	pop bx
	sti
	;Return from interrupt
	iret


;void setKernelDataSegment()
;sets the data segment to the kernel, saving the current ds on the stack
_setKernelDataSegment:
  pop bx
  push ds
  push bx
  mov ax,#0x1000
  mov ds,ax
  ret

;void restoreDataSegment()
;restores the data segment
_restoreDataSegment:
  pop bx
  pop ds
  push bx
  ret


_copyToSeg:
	push bp
	mov bp,sp
	push ds
	push si
	push di
	push es

	mov si,[bp+4]
	mov es,[bp+6]
	mov di,[bp+8]
	mov cx,[bp+10]
	rep
		movsb
	pop es
	pop di
	pop si
	pop ds
	pop bp
	ret
