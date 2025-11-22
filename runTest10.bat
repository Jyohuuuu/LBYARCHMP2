nasm -f win64 asmMain.asm
gcc -c test.c -o test.obj -m64
gcc test.obj asmMain.obj -o test.exe -m64
test.exe < sampleInput10.txt