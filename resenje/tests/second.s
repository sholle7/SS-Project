.global b
.global a
.section .text

a:
 ldr r3, $4
 halt

b:
  ldr r2, $5
  jmp a

