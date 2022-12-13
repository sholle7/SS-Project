#ifndef _instruction_h_
#define _instruction_h_

#include <iostream>
#include <vector>
#include <bitset>
#include <regex>
#include "constants.h"

using namespace std;


class Instruction{
public:
  Instruction(){this->mnemonic=""; this->instructionDescriptor=0; this->registersDescriptor=0;
  this->addressMode=0;this->dataHigh=0;this->dataLow=0;
  this->operandsRequired=0; this->size=0;}
  Instruction(string mnemonic, string instructionDescriptor, unsigned operandsRequired);

  string getMnemonic() { return this->mnemonic; }
  unsigned getOperandsRequired() { return this->operandsRequired; }
  unsigned getSize(){return this->size;}
  vector<string>getOperands(){return this->operands;}


  void resolveDescriptors(string instructionLineWithoutLabel,vector<string> operandsUnresolved);
  void resolveRegistersDesctiptor(string instructionLineWithoutLabel, vector<string> operandsUnresolved);
  void resolveAddressMode(string instructionLineWithoutLabel, vector<string> operandsUnresolved);
  void resolveDataHighAndDataLow(string instructionLineWithoutLabel, vector<string> operandsUnresolved);


  bitset<8> getInstructionDescriptor() { return this->instructionDescriptor; }
  bitset<8> getRegistersDescriptor() { return this->registersDescriptor; }
  bitset<8> getAddressMode() { return this->addressMode; };
  bitset<8> getDataHigh() { return this->dataHigh; };
  bitset<8> getDataLow() { return this->dataLow; };




  void setMnemonic(string mnemonic){this->mnemonic = mnemonic;}
  void setInstructionDescriptor(bitset<8> instructionDescriptor){this->instructionDescriptor = instructionDescriptor;}
  void setOperandsRequired(unsigned operandsRequired){this->operandsRequired = operandsRequired;}
  void setInstructionSize(string instructionLineWithoutLabel, vector<string> operandsUnresolved);





  bool isRegisterDirectLdrStr(string instructionLine);
  bool isRegisterIndirectLdrStr(string instructionLine);
  bool isRegisterDirectJumpCall(string instructionLine);
  bool isRegisterIndirectJumpCall(string instructionLine);


  bool isMemoryDirectAbsoluteLdrStr(string instructionLine);
  bool isMemoryDirectLiteralLdrStr(string instructionLine);
  bool isPcRelativeLdrStr(string instructionLine);
  bool isDirectLiteralLdrStr(string instructionLine);
  bool isDirectSymbolLdrStr(string instructionLine);
  bool isDisplayLiteralLdrStr(string instructionLine) ;
  bool isDisplaySymbolLdrStr(string instructionLine);


  bool isDirectJumpCall(string instructionLine); 
  bool isAbosluteJumpCall(string instructionLine);
  bool isPcRelativeJumpCall(string instructionLine);
  bool isMemoryDirectLiteralJumpCall(string instructionLine);
  bool isMemoryDirectSymbolJumpCall(string instructionLine);
  bool isDisplayLiteralJumpCall(string instructionLine);
  bool isDisplaySymbolJumpCall(string instructionLine);
  
  string getSymbolToAddInRelocationsTable(){return this->symbolToAddInRelocationsTable;}


private:
  string mnemonic;
  unsigned operandsRequired;
  unsigned size;
  vector<string> operands;

  bitset<8> instructionDescriptor; 
  bitset<8> registersDescriptor;
  bitset<8> addressMode;
  bitset<8> dataHigh;
  bitset<8> dataLow;

  string symbolToAddInRelocationsTable;
};

#endif