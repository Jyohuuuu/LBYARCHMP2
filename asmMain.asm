section .data
var1 dd 1000.0
var2 dd 3600.0
section .text
bits 64
default rel
global asmMain

asmMain:
    mov R11, 0
    mov R12, 0
    L1:
    cmp RCX, 0
    je END
    MOVSS XMM0, [RDX+R11*4]
    inc R11
    MOVSS XMM1, [RDX+R11*4]
    inc R11
    MOVSS XMM2, [RDX+R11*4]
    inc R11
    VSUBSS XMM3, XMM1, XMM0
    DIVSS XMM3,XMM2
    MULSS XMM3, [var1]
    DIVSS XMM3, [var2]
    CVTSS2SI R9, XMM3
    mov [R8+R12*4], R9
    inc R12
    dec RCX
    jmp L1
  
 
    END:
    ret