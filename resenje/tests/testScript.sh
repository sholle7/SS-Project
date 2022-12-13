ASSEMBLER=../assembler
LINKER=../linker
EMULATOR=../emulator

retVal=0

${ASSEMBLER} -o allInstructions.o allInstructions.s
retVal1=$?
if [ $retVal1 -ne 0 ]
then
   retVal=$((retVal+1))
fi

${ASSEMBLER} -o ivt.o ivt.s
retVal1=$?
if [ $retVal1 -ne 0 ]
then
   retVal=$((retVal+1))
fi

${ASSEMBLER} -o isr_reset.o isr_reset.s
retVal1=$?
if [ $retVal1 -ne 0 ]
then
   retVal=$((retVal+1))
fi

${ASSEMBLER} -o isr_terminal.o isr_terminal.s
retVal1=$?
if [ $retVal1 -ne 0 ]
then
   retVal=$((retVal+1))
fi

${ASSEMBLER} -o isr_timer.o isr_timer.s
retVal1=$?
if [ $retVal1 -ne 0 ]
then
   retVal=$((retVal+1))
fi

${ASSEMBLER} -o isr_user0.o isr_user0.s
retVal1=$?
if [ $retVal1 -ne 0 ]
then
   retVal=$((retVal+1))
fi

${LINKER} -hex -o test1.hex ivt.o allInstructions.o isr_reset.o isr_terminal.o isr_timer.o isr_user0.o
retVal1=$?
if [ $retVal1 -ne 0 ]
then
   retVal=$((retVal+1))
fi

if [ $retVal -eq 0 ]
then
   ${EMULATOR} test1.hex
fi
