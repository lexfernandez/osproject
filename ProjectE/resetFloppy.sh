rm ./bootload
rm ./kernel.o
rm ./kernel
nasm ./bootload.asm
dd if=/dev/zero of=floppya.img bs=512 count=2880
dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc
bcc -ansi -c -o kernel.o kernel.c
as86 kernel.asm -o kernel_asm.o
as86 start.asm -o start_asm.o
ld86 -o kernel -d start_asm.o kernel.o kernel_asm.o
dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3

#dd if=message.txt of=floppya.img bs=512 count=1 seek=30 conv=notrunc

#bcc -ansi -c -o test.o test.c
#as86 os_api.asm -o os_api_asm.o
#ld86 -o syscall_test -d test.o os_api_asm.o
#dd if=syscall_test of=floppya.img bs=512 count=1 seek=11 conv=notrunc

dd if=map.img of=floppya.img bs=512 count=1 seek=1 conv=notrunc
dd if=dir.img of=floppya.img bs=512 count=1 seek=2 conv=notrunc

#gcc loadFile.c -o loadFile
#./loadFile message.txt

#bochs -f opsys.bxrc
bcc -ansi -c -o lex.o lex.c
ld86 -o lex -d lex.o os_api_asm.o
cp lex ./sfs_root/
cp tstprg ./sfs_root/
cp refere ./sfs_root/
./cshell.sh
