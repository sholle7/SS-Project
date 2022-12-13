.global my_start
.global value0

.section .text
value0:
.word 0

my_start:

ldr r6, $0xFEFE 

ldr r0, $2
ldr r1, $5
ldr r2, $1

not r2

ldr r3, $4
ldr r4, $1 

test r4, r3
jgt endSymbol

ldr r3, $1
ldr r4, $0xEFFF
shl r4, r3
jgt endSymbol


xchg r0, r1
cmp r0, r1

jgt endSymbol
ldr r1, $7

endSymbol:
ldr r5, $func2
ldr r4, $0
str r5, [r4+0x1000]

call *[r4+0x1000]
halt

func2:
  ldr r4, $2
  ret
  
func:
  ldr r4, $1
  ret
  