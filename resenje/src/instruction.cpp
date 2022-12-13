#include "../inc/instruction.h"

Instruction::Instruction(string mnemonic, string instructionDescriptor, unsigned operandsRequired){
  this->mnemonic = mnemonic;
  this->instructionDescriptor = bitset<8>(instructionDescriptor);
  this->operandsRequired = operandsRequired;
  this->size = 0;
  this->registersDescriptor=0;
  this->addressMode=0;
  this->dataHigh=0;
  this->dataLow=0;
}

void Instruction::setInstructionSize(string instructionLineWithoutLabel, vector<string> operandsUnresolved){

  if(operandsUnresolved.size()==0){
    this->size = 1;
    return;
  }
 
  if (this->mnemonic == "int" || this->mnemonic == "not") {
    this->size = 2;
		return;
  }

	if (this->mnemonic == "push" || this->mnemonic == "pop") {
    this->size = 3;
		return;
  }

  if(operandsUnresolved.size()==2 && this->mnemonic!="ldr" && this->mnemonic!="str"){
    this->size = 2;
    return;
  }


  //jumps, call, ldr and str
  if (isRegisterDirectLdrStr(instructionLineWithoutLabel) || isRegisterIndirectLdrStr(instructionLineWithoutLabel) || isRegisterDirectJumpCall(instructionLineWithoutLabel) || isRegisterIndirectJumpCall(instructionLineWithoutLabel)) {
    this->size=3;
		return;
  }
	
  if (isMemoryDirectAbsoluteLdrStr(instructionLineWithoutLabel) || isPcRelativeLdrStr(instructionLineWithoutLabel) || isDirectLiteralLdrStr(instructionLineWithoutLabel) || 
     isDirectSymbolLdrStr(instructionLineWithoutLabel) || isDisplayLiteralLdrStr(instructionLineWithoutLabel) || isDisplaySymbolLdrStr(instructionLineWithoutLabel) ||
     isAbosluteJumpCall(instructionLineWithoutLabel) || isPcRelativeJumpCall(instructionLineWithoutLabel) || isMemoryDirectLiteralJumpCall(instructionLineWithoutLabel) || isMemoryDirectSymbolJumpCall(instructionLineWithoutLabel) || 
     isDisplayLiteralJumpCall(instructionLineWithoutLabel) || isDisplaySymbolJumpCall(instructionLineWithoutLabel)||isMemoryDirectLiteralLdrStr(instructionLineWithoutLabel)||isDirectJumpCall(instructionLineWithoutLabel)){ 
     this->size=5;
  	 return;
  }
  
}



bool Instruction::isRegisterDirectLdrStr(string instructionLineWithoutLabel){
  bool flag = false;
  if(regex_match(instructionLineWithoutLabel, LDR_STR_REGISTER_DIRECT_REGEX)){
    flag = true;
    
  }
  return flag;
}

bool Instruction::isRegisterIndirectLdrStr(string instructionLineWithoutLabel){
  bool flag = false;
  if(regex_match(instructionLineWithoutLabel, LDR_STR_REGISTER_INDIRECT_REGEX)){
    flag = true;
    
  }
  return flag;
}

bool Instruction::isRegisterDirectJumpCall(string instructionLineWithoutLabel){
  bool flag = false;
  if(regex_match(instructionLineWithoutLabel, JUMP_CALL_REGISTER_DIRECT_REGEX)){
    flag = true;
    
  }
  return flag;
}

bool Instruction::isRegisterIndirectJumpCall(string instructionLineWithoutLabel){
  bool flag = false;
  if(regex_match(instructionLineWithoutLabel, JUMP_CALL_REGISTER_INDIRECT_REGEX)){
    flag = true;
    
  }
  return flag;
}

bool Instruction::isMemoryDirectAbsoluteLdrStr(string instructionLineWithoutLabel){
  bool flag = false;
  if(regex_match(instructionLineWithoutLabel, LDR_STR_MEMORY_DIRECT_ABOSOLUTE_REGEX)){
    flag = true;
    
  }
  return flag;
}

bool Instruction::isPcRelativeLdrStr(string instructionLineWithoutLabel){
  bool flag = false;
  if(regex_match(instructionLineWithoutLabel, LDR_STR_PC_RELATIVE_REGEX)){
    flag = true;
    
  }
  return flag;
}

bool Instruction::isDirectLiteralLdrStr(string instructionLineWithoutLabel){
  bool flag = false;
  if(regex_match(instructionLineWithoutLabel, LDR_STR_DIRECT_LITERAL_REGEX)){
    flag = true;
    
  }
  return flag;
}

bool Instruction::isDirectSymbolLdrStr(string instructionLineWithoutLabel){
  bool flag = false;
  if(regex_match(instructionLineWithoutLabel, LDR_STR_DIRECT_SYMBOL_REGEX)){
    flag = true;
    
  }
  return flag;
}

bool Instruction::isDisplayLiteralLdrStr(string instructionLineWithoutLabel){
  bool flag = false;
  if(regex_match(instructionLineWithoutLabel, LDR_STR_DISPLAY_LITERAL_REGEX)){
    flag = true;
    
  }
  return flag;
}

bool Instruction::isDisplaySymbolLdrStr(string instructionLineWithoutLabel){
  bool flag = false;
  if(regex_match(instructionLineWithoutLabel, LDR_STR_DISPLAY_SYMBOL_REGEX)){
    flag = true;
   
  }
  return flag;
}

bool Instruction::isAbosluteJumpCall(string instructionLineWithoutLabel){
  bool flag = false;
  if(regex_match(instructionLineWithoutLabel, JUMP_CALL_ABSOLUTE_REGEX)){
    flag = true;
    
  }
  return flag;
}

bool Instruction::isPcRelativeJumpCall(string instructionLineWithoutLabel){
  bool flag = false;
  if(regex_match(instructionLineWithoutLabel, JUMP_CALL_PC_RELATIVE_REGEX)){
    flag = true;
    
  }
  return flag;
}

bool Instruction::isMemoryDirectLiteralJumpCall(string instructionLineWithoutLabel){
  bool flag = false;
  if(regex_match(instructionLineWithoutLabel, JUMP_CALL_MEMORY_DIRECT_LITERAL_REGEX)){
    flag = true;
    
  }
  return flag;
} 

bool Instruction::isMemoryDirectSymbolJumpCall(string instructionLineWithoutLabel){
  bool flag = false;
  if(regex_match(instructionLineWithoutLabel, JUMP_CALL_MEMORY_DIRECT_SYMBOL_REGEX)){
    flag = true;
    
  }
  return flag;
}

bool Instruction::isDisplayLiteralJumpCall(string instructionLineWithoutLabel){
  bool flag = false;
  if(regex_match(instructionLineWithoutLabel, JUMP_CALL_DISPLAY_LITERAL_REGEX)){
    flag = true;
    
  }
  return flag;
}

bool Instruction::isDisplaySymbolJumpCall(string instructionLineWithoutLabel){
  bool flag = false;
  if(regex_match(instructionLineWithoutLabel, JUMP_CALL_DISPLAY_SYMBOL_REGEX)){
    flag = true;
    
  }
  return flag;
}

bool Instruction::isMemoryDirectLiteralLdrStr(string instructionLineWithoutLabel){
  bool flag = false;
  if(regex_match(instructionLineWithoutLabel, LDR_STR_MEMORY_DIRECT_LITERAL_REGEX)){
    flag = true;
    
  }
  return flag;
}

bool Instruction::isDirectJumpCall(string instructionLineWithoutLabel){
  bool flag = false;
  if(regex_match(instructionLineWithoutLabel, JUMP_CALL_DIRECT_REGEX)){
    flag = true;
    
  }
  return flag;
}

void Instruction::resolveDescriptors(string instructionLineWithoutLabel, vector<string> operandsUnresolved){
  resolveRegistersDesctiptor(instructionLineWithoutLabel, operandsUnresolved);
  resolveAddressMode(instructionLineWithoutLabel, operandsUnresolved);
  resolveDataHighAndDataLow(instructionLineWithoutLabel, operandsUnresolved);
}



void Instruction::resolveRegistersDesctiptor(string instructionLineWithoutLabel, vector<string> operandsUnresolved){
  
  
  if(this->getSize() < 2) return; //halt iret ret

  if(regex_match(instructionLineWithoutLabel, ONE_OPERAND_REGEX)){
    
    string regD = operandsUnresolved[0];
    unsigned long regDIndex;

    if(regD!="pc" && regD!="sp" && regD!="psw"){
      int position = regD.find("r");
      regD = regD.substr(position+1);
      regDIndex = stoul(regD);
    }
    else{
      if(regD=="sp") regDIndex=6;
      if(regD=="pc") regDIndex=7;
      if(regD=="psw") regDIndex=8;
    }

    string regDesc;

    if(this->mnemonic == "push"){
      string lowBits = "0110"; //register sp is regS
      bitset<4> highBits = bitset<4>(regDIndex);
      regDesc = highBits.to_string() + lowBits;
      this->registersDescriptor = bitset<8>(regDesc);
    }

    if(this->mnemonic == "pop"){
      string lowBits = "0110"; //register sp is regS
      bitset<4> highBits = bitset<4>(regDIndex);
      regDesc = highBits.to_string() + lowBits;
      this->registersDescriptor = bitset<8>(regDesc);
    }

    if(this->mnemonic == "int"){  
      string lowBits = "1111";
      bitset<4> highBits = bitset<4>(regDIndex);
      regDesc = highBits.to_string() + lowBits;
      this->registersDescriptor = bitset<8>(regDesc);
    }

    if(this->mnemonic == "not"){
      string lowBits = "1111";
      bitset<4> highBits = bitset<4>(regDIndex);
      regDesc = highBits.to_string() + lowBits;
      this->registersDescriptor = bitset<8>(regDesc);
    }
  }

  if(regex_match(instructionLineWithoutLabel, TWO_OPERANDS_REGEX)){
    string regD = operandsUnresolved[0];
    string regS = operandsUnresolved[1];

    unsigned long regDIndex;
    unsigned long regSIndex;

    if(regD!="pc" && regD!="sp" && regD!="psw"){
      int position = regD.find("r");
      regD = regD.substr(position+1);
      regDIndex = stoul(regD);
    }
    else{
      if(regD=="sp") regDIndex=6;
      if(regD=="pc") regDIndex=7;
      if(regD=="psw") regDIndex=8;
    }
    
    if(regS!="pc" && regS!="sp" && regS!="psw"){
      int positionS= regS.find("r");
      regS = regS.substr(positionS+1);
      regSIndex = stoul(regS);
    }
    else{
      if(regS=="sp") regSIndex=6;
      if(regS=="pc") regSIndex=7;
      if(regS=="psw") regSIndex=8;
    }

    string regDesc;
    bitset<4> lowBits = bitset<4>(regSIndex);
    bitset<4> highBits = bitset<4>(regDIndex);
    regDesc = highBits.to_string() + lowBits.to_string();

    if(this->mnemonic == "xchg"){
      this->registersDescriptor = bitset<8>(regDesc);
    }

    if(this->mnemonic == "add"){
      this->registersDescriptor = bitset<8>(regDesc);
    }

    if(this->mnemonic == "sub"){
      this->registersDescriptor = bitset<8>(regDesc);
    }

    if(this->mnemonic == "mul"){
      this->registersDescriptor = bitset<8>(regDesc);
    }

    if(this->mnemonic == "div"){
      this->registersDescriptor = bitset<8>(regDesc);
    }

    if(this->mnemonic == "cmp"){
      this->registersDescriptor = bitset<8>(regDesc);
    }

    if(this->mnemonic == "and"){
      this->registersDescriptor = bitset<8>(regDesc);
    }

    if(this->mnemonic == "or"){
      this->registersDescriptor = bitset<8>(regDesc);
    }

    if(this->mnemonic == "xor"){
      this->registersDescriptor = bitset<8>(regDesc);
    }

    if(this->mnemonic == "test"){
      this->registersDescriptor = bitset<8>(regDesc);
    }

    if(this->mnemonic == "shl"){
      this->registersDescriptor = bitset<8>(regDesc);
    }

    if(this->mnemonic == "shr"){
      this->registersDescriptor = bitset<8>(regDesc);
    }
  }

  if(regex_match(this->getMnemonic(), JUMP_CALL_INSTRUCTION_REGEX)){
    string regS = operandsUnresolved[0];
    unsigned long regSIndex;
    string regDesc;
    bitset<4> lowBits;
    string highBits = "1111";

    if(!isRegisterDirectJumpCall(instructionLineWithoutLabel) && !isRegisterIndirectJumpCall(instructionLineWithoutLabel) && 
      !isDisplayLiteralJumpCall(instructionLineWithoutLabel) && !isDisplaySymbolJumpCall(instructionLineWithoutLabel)) {
        lowBits = bitset<4>("1111");
    }
    else{
        string regS = operandsUnresolved[0];
        string newRegS="";
        unsigned long regSIndex;
        
        if(isRegisterDirectJumpCall(instructionLineWithoutLabel)){
          
          if(regS!="pc" && regS!="sp" && regS!="psw"){
            int positionS= regS.find("r");
            regS = regS.substr(positionS+1);
            regSIndex = stoul(regS);
          }
          else{
            if(regS=="sp") regSIndex=6;
            if(regS=="pc") regSIndex=7;
            if(regS=="psw") regSIndex=8;
          }

        }
        else{

          int positionS= regS.find("[");
            for(int i=positionS+1; i<regS.size(); i++){
                if(regS[i]!=' ' && regS[i]!=']' && regS[i]!= '+') newRegS += regS[i];
                else break;
            }

          if(newRegS!="pc" && newRegS!="sp" && newRegS!="psw"){
            int position = newRegS.find("r");
            newRegS = newRegS.substr(position+1);
            regSIndex = stoul(newRegS);
          }
          else{
            if(newRegS=="sp") regSIndex=6;
            if(newRegS=="pc") regSIndex=7;
            if(newRegS=="psw") regSIndex=8;
          }     
        }
        lowBits = bitset<4>(regSIndex);
    }

     
    if(isPcRelativeJumpCall(instructionLineWithoutLabel)){
      lowBits = bitset<4>("0111"); //PC register 
    }

    
    regDesc = highBits + lowBits.to_string();

    if(this->mnemonic == "jmp"){
      this->registersDescriptor = bitset<8>(regDesc);
    }
    if(this->mnemonic == "jeq"){
      this->registersDescriptor = bitset<8>(regDesc);
    }
    if(this->mnemonic == "jne"){
      this->registersDescriptor = bitset<8>(regDesc);
    }
    if(this->mnemonic == "jgt"){
      this->registersDescriptor = bitset<8>(regDesc);
    }
    if(this->mnemonic == "call"){
      this->registersDescriptor = bitset<8>(regDesc);
    }

  }

  if(regex_match(this->getMnemonic(), LDR_STR_INSTRUCTION_REGEX)){
    string regD = operandsUnresolved[0];
    unsigned long regDIndex;

    if(regD!="pc" && regD!="sp" && regD!="psw"){
      int position = regD.find("r");
      regD = regD.substr(position+1);
      regDIndex = stoul(regD);
    }
    else{
      if(regD=="sp") regDIndex=6;
      if(regD=="pc") regDIndex=7;
      if(regD=="psw") regDIndex=8;
    }

    string regDesc;

    if(this->mnemonic == "ldr" || this->mnemonic=="str"){
      bitset<4> lowBits;
      bitset<4> highBits;

      if(!isRegisterDirectLdrStr(instructionLineWithoutLabel) && !isRegisterIndirectLdrStr(instructionLineWithoutLabel) && 
      !isDisplayLiteralLdrStr(instructionLineWithoutLabel) && !isDisplaySymbolLdrStr(instructionLineWithoutLabel)) {
        lowBits = bitset<4>("1111");
      }
      else{
        string regS = operandsUnresolved[1];
        string newRegS="";
        unsigned long regSIndex;
        
        if(isRegisterDirectLdrStr(instructionLineWithoutLabel)){
          
          if(regS!="pc" && regS!="sp" && regS!="psw"){
            int positionS= regS.find("r");
            regS = regS.substr(positionS+1);
            regSIndex = stoul(regS);
          }
          else{
            if(regS=="sp") regSIndex=6;
            if(regS=="pc") regSIndex=7;
            if(regS=="psw") regSIndex=8;
          }

        }
        else{

          int positionS= regS.find("[");
            for(int i=positionS+1; i<regS.size(); i++){
                if(regS[i]!=' ' && regS[i]!=']' && regS[i]!= '+') newRegS += regS[i];
                else break;
            }

          if(newRegS!="pc" && newRegS!="sp" && newRegS!="psw"){
            int position = newRegS.find("r");
            newRegS = newRegS.substr(position+1);
            regSIndex = stoul(newRegS);
          }
          else{
            if(newRegS=="sp") regSIndex=6;
            if(newRegS=="pc") regSIndex=7;
            if(newRegS=="psw") regSIndex=8;
          }     
        }
        lowBits = bitset<4>(regSIndex);
      }
    
      if(isPcRelativeLdrStr(instructionLineWithoutLabel)){
        lowBits = bitset<4>("0111"); //PC register 
      }

      highBits = bitset<4>(regDIndex);
      regDesc = highBits.to_string() + lowBits.to_string();
      
      this->registersDescriptor = bitset<8>(regDesc);
    }
  }
}

void Instruction::resolveAddressMode(string instructionLineWithoutLabel, vector<string> operandsUnresolved){
  
  if(this->getSize() < 3) return;

  string addrMode;

  if(regex_match(this->getMnemonic(), JUMP_CALL_INSTRUCTION_REGEX)){
    string lowBits;
    string highBits = "0000";

    //immediate
    if(isDirectJumpCall(instructionLineWithoutLabel) || isAbosluteJumpCall(instructionLineWithoutLabel)){
      lowBits="0000";
    }

    //regdir
    if(isRegisterDirectJumpCall(instructionLineWithoutLabel)){
      lowBits="0001";
    }

    //regdir with 16-bit addend
    if(isPcRelativeJumpCall(instructionLineWithoutLabel)){
      lowBits="0101";
    }

    //regind
    if(isRegisterIndirectJumpCall(instructionLineWithoutLabel)){
      lowBits="0010";
    }

    //regind with 16-bit offset
    if(isDisplaySymbolJumpCall(instructionLineWithoutLabel)
    || isDisplayLiteralJumpCall(instructionLineWithoutLabel)){
      lowBits="0011";
    }

    //mem
    if((isMemoryDirectSymbolJumpCall(instructionLineWithoutLabel) && !isRegisterDirectJumpCall(instructionLineWithoutLabel))
    || isMemoryDirectLiteralJumpCall(instructionLineWithoutLabel)){
      lowBits="0100";
    }

    addrMode = highBits + lowBits;
    this->addressMode = bitset<8>(addrMode);
  }

  if(regex_match(this->getMnemonic(), LDR_STR_INSTRUCTION_REGEX)){
    string lowBits;
    string highBits = "0000";

    //immediate
    if(isDirectLiteralLdrStr(instructionLineWithoutLabel) 
    || isDirectSymbolLdrStr(instructionLineWithoutLabel)){
      lowBits="0000";
    }

    //regdir
    if(isRegisterDirectLdrStr(instructionLineWithoutLabel)){
      lowBits="0001";
    }

    //regind
    if(isRegisterIndirectLdrStr(instructionLineWithoutLabel)){
      lowBits="0010";
    }

    //regind with 16-bit offset
    if(isPcRelativeLdrStr(instructionLineWithoutLabel)
    ||isDisplayLiteralLdrStr(instructionLineWithoutLabel)
    ||isDisplaySymbolLdrStr(instructionLineWithoutLabel)){
      lowBits="0011";
    }

    //mem
    if((isMemoryDirectAbsoluteLdrStr(instructionLineWithoutLabel)&& !isRegisterDirectLdrStr(instructionLineWithoutLabel))
      ||isMemoryDirectLiteralLdrStr(instructionLineWithoutLabel)){
      lowBits="0100";
    }
    
    addrMode = highBits + lowBits;
    this->addressMode = bitset<8>(addrMode);
  }

  if(this->getMnemonic() == "push"){
    string lowBits ="0010";
    string highBits = "0001";
    addrMode = highBits + lowBits;
    this->addressMode = bitset<8>(addrMode);
  }

  if(this->getMnemonic() == "pop"){
    string lowBits = "0010";
    string highBits="0100";
    addrMode = highBits + lowBits;
    this->addressMode = bitset<8>(addrMode);
  }

}

void Instruction::resolveDataHighAndDataLow(string instructionLineWithoutLabel, vector<string> operandsUnresolved){
  
  if(this->getSize() < 5) return;

  if(regex_match(this->getMnemonic(), JUMP_CALL_INSTRUCTION_REGEX)){

    if(isDirectJumpCall(instructionLineWithoutLabel)){
      string literal = operandsUnresolved[0];
      int value;

      if(regex_match(literal, LITERAL_DECIMAL_REGEX)){ 
        value = stoi(literal);
      }

      if(regex_match(literal, LITERAL_HEXADECIMAL_REGEX)) {
        value = stoi(literal,nullptr,16);
      }
      
      bitset<16> data = bitset<16>(value); 
      dataHigh = bitset<8>(data.to_ulong()>>8 & 0x00ff);
      
      dataLow = bitset<8>(data.to_ulong() & 0x00ff);
      
    }

    if(isAbosluteJumpCall(instructionLineWithoutLabel)){
      
      string symbol = operandsUnresolved[0];
      this->symbolToAddInRelocationsTable = symbol;

      
      dataHigh=bitset<8>(0);
      dataLow=bitset<8>(0);
    }

    if(isPcRelativeJumpCall(instructionLineWithoutLabel)){
      
      string symbol = operandsUnresolved[0];
      int position = symbol.find("%");
      symbol = symbol.substr(position+1);
      this->symbolToAddInRelocationsTable = symbol;

      
      dataHigh=bitset<8>(0);
      dataLow=bitset<8>(0);
    }

    if(isDisplaySymbolJumpCall(instructionLineWithoutLabel)){
      
      string symbol = operandsUnresolved[0];
      int position = symbol.find("+");
      while(true){
        if(symbol[position+1] ==' ') position++;
        else{
          break;
        }
      }
      symbol = symbol.substr(position+1);
      symbol = symbol.erase(symbol.size()-1);
      this->symbolToAddInRelocationsTable = symbol;

      
      dataHigh=bitset<8>(0);
      dataLow=bitset<8>(0);
    }

    if(isDisplayLiteralJumpCall(instructionLineWithoutLabel)){

      string literal = operandsUnresolved[0];
      int position = literal.find("+");
      while(true){
        if(literal[position+1] ==' ') position++;
        else{
          break;
        }
      }
      literal = literal.substr(position+1);
      literal = literal.erase(literal.size()-1);

      int value;

      if(regex_match(literal, LITERAL_DECIMAL_REGEX)){ 
        value = stoi(literal);
      }

      if(regex_match(literal, LITERAL_HEXADECIMAL_REGEX)) {
        value = stoi(literal,nullptr,16);
      }
      
      bitset<16> data = bitset<16>(value); 
      dataHigh = bitset<8>(data.to_ulong()>>8 & 0x00ff);
      dataLow = bitset<8>(data.to_ulong() & 0x00ff);
    }
    
    if(isMemoryDirectSymbolJumpCall(instructionLineWithoutLabel)){
      
      string symbol = operandsUnresolved[0];
      int position = symbol.find("*");
      symbol = symbol.substr(position+1);
      this->symbolToAddInRelocationsTable = symbol;

      
      dataHigh=bitset<8>(0);
      dataLow=bitset<8>(0);
    }
    
    if(isMemoryDirectLiteralJumpCall(instructionLineWithoutLabel)){
      string literal = operandsUnresolved[0];
      int position = literal.find("*");
      literal = literal.substr(position+1);
      
      int value;

      if(regex_match(literal, LITERAL_DECIMAL_REGEX)){ 
        value = stoi(literal);
      }

      if(regex_match(literal, LITERAL_HEXADECIMAL_REGEX)) {
        value = stoi(literal,nullptr,16);
      }
      
      bitset<16> data = bitset<16>(value); 
      dataHigh = bitset<8>(data.to_ulong()>>8 & 0x00ff);
      dataLow = bitset<8>(data.to_ulong() & 0x00ff);
      
    }
 
  }

  if(regex_match(this->getMnemonic(), LDR_STR_INSTRUCTION_REGEX)){
    
    if(isDirectLiteralLdrStr(instructionLineWithoutLabel)){
      string literal = operandsUnresolved[1];
      int position = literal.find("$");
      literal = literal.substr(position+1);
      int value;

      if(regex_match(literal, LITERAL_DECIMAL_REGEX)){ 
        value = stoi(literal);
      }

      if(regex_match(literal, LITERAL_HEXADECIMAL_REGEX)) {
        value = stoi(literal,nullptr,16);
      }
      
      bitset<16> data = bitset<16>(value); 
      dataHigh = bitset<8>(data.to_ulong()>>8 & 0x00ff);
      dataLow = bitset<8>(data.to_ulong() & 0x00ff);
    }
    
    if(isDirectSymbolLdrStr(instructionLineWithoutLabel)){
      
      string symbol = operandsUnresolved[1];
      int position = symbol.find("$");
      symbol = symbol.substr(position+1);
      this->symbolToAddInRelocationsTable = symbol;

      
      dataHigh=bitset<8>(0);
      dataLow=bitset<8>(0);
    }

    if(isPcRelativeLdrStr(instructionLineWithoutLabel)){
      
      string symbol = operandsUnresolved[1];
      int position = symbol.find("%");
      symbol = symbol.substr(position+1);
      this->symbolToAddInRelocationsTable = symbol;

      
      dataHigh=bitset<8>(0);
      dataLow=bitset<8>(0);
    }

    if(isDisplayLiteralLdrStr(instructionLineWithoutLabel)){

      string literal = operandsUnresolved[1];
      int position = literal.find("+");
      while(true){
        if(literal[position+1] ==' ') position++;
        else{
          break;
        }
      }

      literal = literal.substr(position+1);
      literal = literal.erase(literal.size()-1);

      int value;

      if(regex_match(literal, LITERAL_DECIMAL_REGEX)){ 
        value = stoi(literal);
      }

      if(regex_match(literal, LITERAL_HEXADECIMAL_REGEX)) {
        value = stoi(literal,nullptr,16);
      }

      
      bitset<16> data = bitset<16>(value); 
      dataHigh = bitset<8>(data.to_ulong()>>8 & 0x00ff);
      dataLow = bitset<8>(data.to_ulong() & 0x00ff);
    
    }

    if(isDisplaySymbolLdrStr(instructionLineWithoutLabel)){
      
      string symbol = operandsUnresolved[1];
      int position = symbol.find("+");
      while(true){
        if(symbol[position+1] ==' ') position++;
        else{
          break;
        }
      }
      symbol = symbol.substr(position+1);
      symbol = symbol.erase(symbol.size()-1);
      this->symbolToAddInRelocationsTable = symbol;

      
      dataHigh=bitset<8>(0);
      dataLow=bitset<8>(0);
    }
  
    if(isMemoryDirectAbsoluteLdrStr(instructionLineWithoutLabel)){
      
      string symbol = operandsUnresolved[1];
      this->symbolToAddInRelocationsTable = symbol;

      
      dataHigh=bitset<8>(0);
      dataLow=bitset<8>(0);
    }

    if(isMemoryDirectLiteralLdrStr(instructionLineWithoutLabel)){
      string literal = operandsUnresolved[1];
      int value;

      if(regex_match(literal, LITERAL_DECIMAL_REGEX)){ 
        value = stoi(literal);
      }

      if(regex_match(literal, LITERAL_HEXADECIMAL_REGEX)) {
        value = stoi(literal,nullptr,16);
      }
     
      bitset<16> data = bitset<16>(value); 
      dataHigh = bitset<8>(data.to_ulong()>>8 & 0x00ff);
      dataLow = bitset<8>(data.to_ulong() & 0x00ff);
    }
  
  }
}
