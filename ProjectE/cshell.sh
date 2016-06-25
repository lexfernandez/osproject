rm ./sfs_root/shell
rm shell
bcc -ansi -c -o shell.o shell.c
as86 os_api.asm -o os_api_asm.o
ld86 -o shell -d shell.o os_api_asm.o
cp shell ./sfs_root/

bcc -ansi -c -o test_mp.o test.c
ld86 -o test_mp -d test_mp.o os_api_asm.o
cp test_mp ./sfs_root/
