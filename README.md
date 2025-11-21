# LBYARCHMP2
nasm -f win64 asmMain.asm

gcc -c cmain.c -o cmain.obj -m64

gcc cmain.obj asmMain.obj -o cmain.exe -m64

cmain.exe < sampleInput.txt
