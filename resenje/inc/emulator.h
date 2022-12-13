#ifndef _emulator_h_
#define _emulator_h_
#include <iostream>
#include <fstream>
#include "constants.h"
#include "emulatorExceptions.h"
#include "vector"
#include <algorithm>
#include <iterator> 
#include <iomanip>
#include <bitset>

using namespace std;

enum Register{
    r0 = 0,
    r1 = 1,
    r2 = 2,
    r3 = 3,
    r4 = 4,
    r5 = 5,
    r6 = 6,
    sp = 6,
    r7 = 7,
    pc = 7,
    psw = 8,
    unusedRegister = 0xF
};

enum Flag{
  Z = 1,
  O = 1 << 1,
  C = 1 << 2,
  N = 1 << 3,
  Tr = 1 << 13,
  Tl = 1 << 14,
  I = 1 << 15
};

class Emulator{

public:

  Emulator(string fileName);
  ~Emulator();

  void emulate();
  void readFromInputFile();
  void printFinalStateOfEmulatedProcessor();
  void tokenizeLine(string &str, char delim, vector<string> &out);
  void resetFlagPSW(short flag);
  void setFlagPSW(short flag);
  bool getFlagPSW(short flag);
  void execute();
  
  string readByteFromMemory(int address);
  void writeByteToMemory(int address, string byte);
 
  string readWordFromStack();
  void writeWordToStack(string higherByte, string lowerByte);
 
  string getInstructionMnemonic(string firstByte);

  int getDestinationRegisterIndex(string secondByte);
  int getSourceRegisterIndex(string secondByte);
  string getUpdateTypeForSourceRegister(string thirdByte);
  string getAddressingType(string thirdByte);
  string getOperandBasedOnAddressingType(); //for jump, call and ldr (pop) instructions

  void decodeAndExecuteInstruction(string instructionMnemonic);
  void jumpOnInterruptRoutineOfEntry(int entryNumber);
 
private:
  
  ifstream inputFile;
  
  vector<short> registers;
  short* rpc;
  short* rsp;
  short* rpsw;

  short previousPc;
  bool isRunning;

  vector<string> memory; //each string in vector represents one byte of binary code

  int destinationRegisterIndex;
  int sourceRegisterIndex;
  string updateType;
  string addressingType;
  string dataHigh;
  string dataLow;
  bool jumpCondition;
};



#endif