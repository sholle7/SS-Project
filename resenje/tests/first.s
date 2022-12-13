.global my_start
.global value0
.extern b

.section .text
value0:
.word 0

my_start:

ldr r6, $0xFEFE 
ldr r1, $5
ldr r2, $0

a:
  cmp r1, r2
  jgt b
  halt
  