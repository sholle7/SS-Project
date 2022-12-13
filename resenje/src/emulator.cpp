#include "../inc/emulator.h"

Emulator::Emulator(string fileName){
    this->inputFile.open(fileName, fstream::in);
    
    if(!this->inputFile.is_open()){
      throw EmulatorException("Can not open input file");
    }
    int i = 0;
    while(i++ < MEMORY_SIZE) this->memory.push_back("00");

    i = 0;
    while(i++ < 9) this->registers.push_back(0);
    rpc = &registers[pc];
    rsp = &registers[sp];
    rpsw = &registers[psw];   
}

Emulator::~Emulator(){
  this->inputFile.close();
}

void Emulator::emulate(){
  readFromInputFile();
  execute();
  printFinalStateOfEmulatedProcessor();
}

void Emulator::readFromInputFile(){
  string line;
  int index = 0;

  while(getline(this->inputFile, line)){
 
    vector<string> lineValues;
    tokenizeLine(line, ':', lineValues);
    string allBytesFromLine = lineValues[1];

    vector<string> currentLineValues;
    tokenizeLine(allBytesFromLine, ' ', currentLineValues);

    for(int i=0; i<currentLineValues.size(); i++){
      this-> memory[index] = currentLineValues[i];
      index++;
      if(index>DEFAULT_STACK_START_ADDRESS) throw EmulatorException("Program tried to overwrite data on the reserved places");
    }
  }
}

void Emulator::resetFlagPSW(short flag)
{
    *rpsw = *rpsw & ~flag;
}

void Emulator::setFlagPSW(short flag)
{
   *rpsw = *rpsw | flag;
}

bool Emulator::getFlagPSW(short flag)
{
    return *rpsw & flag;
}

void Emulator::tokenizeLine(string &str, char delim, vector<string> &out)
{
  size_t start;
  size_t end = 0;

  while ((start = str.find_first_not_of(delim, end)) != string::npos)
  {
    end = str.find(delim, start);
    out.push_back(str.substr(start, end - start));
  }
}

void Emulator::printFinalStateOfEmulatedProcessor(){
  cout << "------------------------------------------------" << endl;
  cout << "Emulated processor executed halt instruction" << endl;
  cout << "Emulated processor state: psw=0b";

  short flag;

  flag = 1 << 15;
  if (*rpsw & flag) cout << "1";
  else cout << "0";

  flag = 1 << 14;
  while (flag > 0) {
    if((*rpsw & flag) != 0) cout << "1";
    else cout << "0";
    flag = flag / 2;
  }

  cout << endl;

  cout<< "r0=0x" << hex << setfill('0') << setw(4) << registers[r0] << "    " 
      << "r1=0x" << hex << setfill('0') << setw(4) << registers[r1] << "    " 
      << "r2=0x" << hex << setfill('0') << setw(4) << registers[r2] << "    "
      << "r3=0x" << hex << setfill('0') << setw(4) << registers[r3] << endl 
      << "r4=0x" << hex << setfill('0') << setw(4) << registers[r4] << "    " 
      << "r5=0x" << hex << setfill('0') << setw(4) << registers[r5] << "    "
      << "r6=0x" << hex << setfill('0') << setw(4) << registers[r6] << "    " 
      << "r7=0x" << hex << setfill('0') << setw(4) << registers[r7] << endl;
}

void Emulator::execute(){

  this->isRunning = true;

  string lowerBits = readByteFromMemory(0);
  string higherBits = readByteFromMemory(1);
  string address = "0x" + higherBits + lowerBits; 
  short addr = stoi(address, nullptr, 16);
  
  *rpc = addr;

  resetFlagPSW(I);
  resetFlagPSW(Tl);
  resetFlagPSW(Tr);

  while(this->isRunning){
    this->previousPc = *rpc;
    string firstByte = readByteFromMemory(*rpc);
    string instructionMnemonic = getInstructionMnemonic(firstByte);
    decodeAndExecuteInstruction(instructionMnemonic);
  }
}

string Emulator::getInstructionMnemonic(string firstByte){

  if(firstByte == "00"){
    (*rpc)++;
    return "halt";
  }
  else if(firstByte == "10"){
     (*rpc)++;
    return "int";
  }
  else if(firstByte == "20"){
    (*rpc)++;
    return "iret";
  }
  else if(firstByte == "30"){
    (*rpc)++;
    return "call";
  }
  else if(firstByte == "40"){
    (*rpc)++;
    return "ret";
  }
  else if(firstByte == "50"){
     (*rpc)++;
    return "jmp";
  }
  else if(firstByte == "51"){
    (*rpc)++;
    return "jeq";
  }
  else if(firstByte == "52"){
    (*rpc)++;
    return "jne";
  }
  else if(firstByte == "53"){
    (*rpc)++;
    return "jgt";
  }
  else if(firstByte == "b0"){
    (*rpc)++;
    return "str";
  }
  else if(firstByte == "a0"){
    (*rpc)++;
    return "ldr";
  }
  else if(firstByte == "60"){
    (*rpc)++;
    return "xchg";
  }
  else if(firstByte == "70"){
    (*rpc)++;
    return "add";
  }
  else if(firstByte == "71"){
    (*rpc)++;
    return "sub";
  }
  else if(firstByte == "72"){
    (*rpc)++;
    return "mul";
  }
  else if(firstByte == "73"){
    (*rpc)++;
    return "div";
  }
  else if(firstByte == "74"){
    (*rpc)++;
    return "cmp";
  }
  else if(firstByte == "80"){
    (*rpc)++;
    return "not";
  }
  else if(firstByte == "81"){
    (*rpc)++;
    return "and";
  }
  else if(firstByte == "82"){
    (*rpc)++;
    return "or";
  }
  else if(firstByte == "83"){
    (*rpc)++;
    return "xor";
  }
  else if(firstByte == "84"){
    (*rpc)++;
    return "test";
  }
  else if(firstByte == "90"){
    (*rpc)++;
    return "shl";
  }
  else if(firstByte == "91"){
    (*rpc)++;
    return "shr";
  }
  else{
    // throw EmulatorException("Wrong instruction operation code and/or instruction modificator");
    *rpc = previousPc;
    jumpOnInterruptRoutineOfEntry(1);
    return "";
  }
  
}

int Emulator::getDestinationRegisterIndex(string secondByte){
  string byteString = "0x" + secondByte; 
  short byte = stoi(byteString, nullptr, 16);
  int destinationRegisterIndex = (byte >> 4) & 0xF;

  return destinationRegisterIndex;
}

int Emulator::getSourceRegisterIndex(string secondByte){
  string byteString = "0x" + secondByte; 
  short byte = stoi(byteString, nullptr, 16);
  int sourceRegisterIndex = byte & 0xF;

  return sourceRegisterIndex;
}

string Emulator::getUpdateTypeForSourceRegister(string thirdByte){

  string byteString = "0x" + thirdByte; 
  short byte = stoi(byteString, nullptr, 16);
  int upMode = (byte >> 4) & 0xF;
  switch(upMode){
    case 0:return "noChange"; break;
    case 1:return "subtractTwoBefore"; break;
    case 2:return "addTwoBefore"; break;
    case 3:return "subtractTwoAfter"; break;
    case 4:return "addTwoAfter"; break;
    default: return "";
  }
}

string Emulator::getAddressingType(string thirdByte){
  string byteString = "0x" + thirdByte; 
  short byte = stoi(byteString, nullptr, 16);
  int addrType = byte & 0xF;
  switch(addrType){
    case 0: return "immediate"; break;
    case 1: return "registerDirect"; break;
    case 2: return "registerIndirect"; break;
    case 3: return "registerIndirectOffset"; break;
    case 4: return "memory"; break;
    case 5: return "registerDirectAddend"; break;  
    default: return "";
  }
}

string Emulator::getOperandBasedOnAddressingType(){
  string operand = "";
  if(this->addressingType == "immediate"){
    operand = this->dataHigh + this->dataLow;
  }
  else if(this->addressingType == "registerDirect"){
    stringstream ss;
    ss<<hex<<this->registers[this->sourceRegisterIndex];
    ss>>operand;
  }
  else if(this->addressingType == "registerIndirect"){
    string lowByte = readByteFromMemory(this->registers[this->sourceRegisterIndex]);
    string highByte = readByteFromMemory(this->registers[this->sourceRegisterIndex] + 1);
    operand = highByte + lowByte;
  }
  else if(this->addressingType == "registerIndirectOffset"){
    string data = this->dataHigh + this->dataLow;
    short dataPayload = stoi(data, nullptr, 16);
    string lowByte = readByteFromMemory(this->registers[this->sourceRegisterIndex] + dataPayload);
    string highByte = readByteFromMemory(this->registers[this->sourceRegisterIndex] + dataPayload + 1);
    operand = highByte + lowByte;
  }
  else if(this->addressingType == "memory"){
    string data = this->dataHigh + this->dataLow;
    short dataPayload = stoi(data, nullptr, 16);
    string lowByte = readByteFromMemory(dataPayload);
    string highByte = readByteFromMemory(dataPayload + 1);
    operand = highByte + lowByte;
  }
  else if(this->addressingType == "registerDirectAddend"){
    short bytes = this->registers[this->sourceRegisterIndex];
    string data = this->dataHigh + this->dataLow;
    short dataPayload = stoi(data, nullptr, 16);

    stringstream ss;
    ss<< hex << (bytes + dataPayload);
    operand = ss.str();
  }

  return operand;
}

void Emulator::decodeAndExecuteInstruction(string instructionMnemonic){

  if(instructionMnemonic == "halt"){
    //SIZE - 1B

    //INSTRUCTION EXECUTE 
    this->isRunning = false;
    //END
  }
  else if(instructionMnemonic == "int"){
    //SIZE - 2B

    string secondByte = readByteFromMemory(*rpc);
    (*rpc)++;

    //INSTRUCTION DECODE
    this->destinationRegisterIndex = getDestinationRegisterIndex(secondByte);
    this->sourceRegisterIndex = getSourceRegisterIndex(secondByte);
    //END
     
    if (this->sourceRegisterIndex != 0xf) {
      *rpc = this->previousPc;
      jumpOnInterruptRoutineOfEntry(1);
      return;
    }
    
    //INSTRUCTION EXECUTE 
    jumpOnInterruptRoutineOfEntry(this->registers[destinationRegisterIndex]);
    //END
  }
  else if(instructionMnemonic == "iret"){
    //SIZE - 2B

    //INSTRUCTION EXECUTE 
    string helpOPPSW="0x" + readWordFromStack();
    *rpsw = stoi(helpOPPSW, nullptr, 16);

    string helpOPPC="0x" + readWordFromStack();
    *rpc = stoi(helpOPPC, nullptr, 16);

    //END
  }
  else if(instructionMnemonic == "call"){
    //SIZE - (3 or 5)B

    string secondByte = readByteFromMemory(*rpc);
    (*rpc)++;
    string thirdByte = readByteFromMemory(*rpc);
    (*rpc)++;

    //INSTRUCTION DECODE
      this->destinationRegisterIndex = getDestinationRegisterIndex(secondByte);
      this->sourceRegisterIndex = getSourceRegisterIndex(secondByte);

      this->updateType = getUpdateTypeForSourceRegister(thirdByte);
      this->addressingType = getAddressingType(thirdByte);
      

      if (this->addressingType == "immediate" || this->addressingType == "registerIndirectOffset" || this->addressingType == "memory"|| this->addressingType == "registerDirectAddend"){
          this->dataHigh = readByteFromMemory(*rpc);
          this->dataLow = readByteFromMemory((*rpc) + 1);
          (*rpc)++;
          (*rpc)++;
      }
    //END

    if (this->destinationRegisterIndex != 0xf) {
      *rpc = this->previousPc;
      jumpOnInterruptRoutineOfEntry(1);
      return;
    }

    //INSTRUCTION EXECUTE 
      if(this->updateType == "subtractTwoBefore"){
        this->registers[this->sourceRegisterIndex] -= 2;
      }
      if(this->updateType == "addTwoBefore"){
        this->registers[this->sourceRegisterIndex] += 2;
      }

      
      size_t n_zero = 2;

      stringstream ss;

      ss<< hex << (((*rpc)>>8) & 0x00ff);
      string hPc = ss.str();
      hPc = string(n_zero - min(n_zero, hPc.size()), '0') + hPc;
      ss.str("");

      ss<< hex << (((*rpc)) & 0x00ff);
      string lPc = ss.str();
      lPc = string(n_zero - min(n_zero, lPc.size()), '0') + lPc;
      ss.str("");


      writeWordToStack(hPc, lPc);

      string helpOP="0x" + getOperandBasedOnAddressingType();
      short operand = stoi(helpOP, nullptr, 16);

      *rpc = operand;

      if(this->updateType == "subtractTwoAfter"){
         this->registers[this->sourceRegisterIndex] -= 2;
      }
      if(this->updateType == "addTwoAfter"){
         this->registers[this->sourceRegisterIndex] += 2;
      }
    //END 

  }
  else if(instructionMnemonic == "ret"){
    //SIZE - 2B

    //INSTRUCTION EXECUTE 
    string helpOP="0x" + readWordFromStack();
    *rpc = stoi(helpOP, nullptr, 16);
    //END
  }
  else if(instructionMnemonic == "jmp"){
    //SIZE - (3 or 5)B
    
    string secondByte = readByteFromMemory(*rpc);
    (*rpc)++;
    
    string thirdByte = readByteFromMemory(*rpc);
    (*rpc)++;
     

    //INSTRUCTION DECODE
      this->destinationRegisterIndex = getDestinationRegisterIndex(secondByte);
      this->sourceRegisterIndex = getSourceRegisterIndex(secondByte);

      this->updateType = getUpdateTypeForSourceRegister(thirdByte);
      this->addressingType = getAddressingType(thirdByte);

      if(this->addressingType == "immediate" || this->addressingType == "registerIndirectOffset" || this->addressingType == "memory"|| this->addressingType == "registerDirectAddend"){
          this->dataHigh = readByteFromMemory(*rpc);
          this->dataLow = readByteFromMemory((*rpc) + 1);
          (*rpc)++;
          (*rpc)++;
      }
    //END

    if (this->destinationRegisterIndex != 0xf) {
      *rpc = this->previousPc;
      jumpOnInterruptRoutineOfEntry(1);
      return;
    }

    //INSTRUCTION EXECUTE 
      if(this->updateType == "subtractTwoBefore"){
        this->registers[this->sourceRegisterIndex] -= 2;
      }
      if(this->updateType == "addTwoBefore"){
        this->registers[this->sourceRegisterIndex] += 2;
      }

      string helpOP="0x" + getOperandBasedOnAddressingType();
      short operand = stoi(helpOP, nullptr, 16);
      
      *rpc = operand;
    
      if(this->updateType == "subtractTwoAfter"){
         this->registers[this->sourceRegisterIndex] -= 2;
      }
      if(this->updateType == "addTwoAfter"){
         this->registers[this->sourceRegisterIndex] += 2;
      }
    //END
  }
  else if(instructionMnemonic == "jeq"){
    //SIZE - (3 or 5)B

    string secondByte = readByteFromMemory(*rpc);
    (*rpc)++;
    string thirdByte = readByteFromMemory(*rpc);
    (*rpc)++;

    //INSTRUCTION DECODE
      this->destinationRegisterIndex = getDestinationRegisterIndex(secondByte);
      this->sourceRegisterIndex = getSourceRegisterIndex(secondByte);

      this->updateType = getUpdateTypeForSourceRegister(thirdByte);
      this->addressingType = getAddressingType(thirdByte);
      

      if(this->addressingType == "immediate" || this->addressingType == "registerIndirectOffset" || this->addressingType == "memory"|| this->addressingType == "registerDirectAddend"){
          this->dataHigh = readByteFromMemory(*rpc);
          this->dataLow = readByteFromMemory((*rpc) + 1);
          (*rpc)++;
          (*rpc)++;
      }
    //END

    if (this->destinationRegisterIndex != 0xf) {
      *rpc = this->previousPc;
      jumpOnInterruptRoutineOfEntry(1);
      return;
    }

    //INSTRUCTION EXECUTE 
      if(this->updateType == "subtractTwoBefore"){
        this->registers[this->sourceRegisterIndex] -= 2;
      }
      if(this->updateType == "addTwoBefore"){
        this->registers[this->sourceRegisterIndex] += 2;
      }

      string helpOP="0x" + getOperandBasedOnAddressingType();
      short operand = stoi(helpOP, nullptr, 16);

      
      if(getFlagPSW(Z)){
        this->jumpCondition = true;
      }
      else this->jumpCondition = false;


      if (this->jumpCondition) {
          *rpc = operand;
      }

      if(this->updateType == "subtractTwoAfter"){
         this->registers[this->sourceRegisterIndex] -= 2;
      }
      if(this->updateType == "addTwoAfter"){
         this->registers[this->sourceRegisterIndex] += 2;
      }
    //END
  }
  else if(instructionMnemonic == "jne"){
    //SIZE - (3 or 5)B

    string secondByte = readByteFromMemory(*rpc);
    (*rpc)++;
    string thirdByte = readByteFromMemory(*rpc);
    (*rpc)++;

    //INSTRUCTION DECODE
      this->destinationRegisterIndex = getDestinationRegisterIndex(secondByte);
      this->sourceRegisterIndex = getSourceRegisterIndex(secondByte);

      this->updateType = getUpdateTypeForSourceRegister(thirdByte);
      this->addressingType = getAddressingType(thirdByte);
      

      if(this->addressingType == "immediate" || this->addressingType == "registerIndirectOffset" || this->addressingType == "memory"|| this->addressingType == "registerDirectAddend"){
          this->dataHigh = readByteFromMemory(*rpc);
          this->dataLow = readByteFromMemory((*rpc) + 1);
          (*rpc)++;
          (*rpc)++;
      }
    //END

    if (this->destinationRegisterIndex != 0xf) {
      *rpc = this->previousPc;
      jumpOnInterruptRoutineOfEntry(1);
      return;
    }

    //INSTRUCTION EXECUTE 
      if(this->updateType == "subtractTwoBefore"){
        this->registers[this->sourceRegisterIndex] -= 2;
      }
      if(this->updateType == "addTwoBefore"){
        this->registers[this->sourceRegisterIndex] += 2;
      }

      string helpOP="0x" + getOperandBasedOnAddressingType();
      short operand = stoi(helpOP, nullptr, 16);
  
      if(!getFlagPSW(Z)){
        this->jumpCondition = true;
      }
      else this->jumpCondition = false;


      if (this->jumpCondition) {
          *rpc = operand;
      }

      if(this->updateType == "subtractTwoAfter"){
         this->registers[this->sourceRegisterIndex] -= 2;
      }
      if(this->updateType == "addTwoAfter"){
         this->registers[this->sourceRegisterIndex] += 2;
      }
    //END
  }
  else if(instructionMnemonic == "jgt"){
    //SIZE - (3 or 5)B

    string secondByte = readByteFromMemory(*rpc);
    (*rpc)++;
    string thirdByte = readByteFromMemory(*rpc);
    (*rpc)++;

    //INSTRUCTION DECODE
      this->destinationRegisterIndex = getDestinationRegisterIndex(secondByte);
      this->sourceRegisterIndex = getSourceRegisterIndex(secondByte);

      this->updateType = getUpdateTypeForSourceRegister(thirdByte);
      this->addressingType = getAddressingType(thirdByte);
      

      if(this->addressingType == "immediate" || this->addressingType == "registerIndirectOffset" || this->addressingType == "memory"|| this->addressingType == "registerDirectAddend"){
          this->dataHigh = readByteFromMemory(*rpc);
          this->dataLow = readByteFromMemory((*rpc) + 1);
          (*rpc)++;
          (*rpc)++;
      }
    //END

    if (this->destinationRegisterIndex != 0xf) {
      *rpc = this->previousPc;
      jumpOnInterruptRoutineOfEntry(1);
      return;
    }

    //INSTRUCTION EXECUTE 
      if(this->updateType == "subtractTwoBefore"){
        this->registers[this->sourceRegisterIndex] -= 2;
      }
      if(this->updateType == "addTwoBefore"){
        this->registers[this->sourceRegisterIndex] += 2;
      }

      string helpOP="0x" + getOperandBasedOnAddressingType();
      short operand = stoi(helpOP, nullptr, 16);

      if(!(getFlagPSW(O) ^ getFlagPSW(N)) & !getFlagPSW(Z)){
        this->jumpCondition = true;
      }
      else this->jumpCondition = false;


      if (this->jumpCondition) {
          *rpc = operand;
      }

      if(this->updateType == "subtractTwoAfter"){
         this->registers[this->sourceRegisterIndex] -= 2;
      }
      if(this->updateType == "addTwoAfter"){
         this->registers[this->sourceRegisterIndex] += 2;
      }
    //END
  }
  else if(instructionMnemonic == "str"){
    //str or push instructions

    //SIZE - (3 or 5)B

    string secondByte = readByteFromMemory(*rpc);
    (*rpc)++;
    string thirdByte = readByteFromMemory(*rpc);
    (*rpc)++;

    //INSTRUCTION DECODE
      this->destinationRegisterIndex = getDestinationRegisterIndex(secondByte);
      this->sourceRegisterIndex = getSourceRegisterIndex(secondByte);

      this->updateType = getUpdateTypeForSourceRegister(thirdByte);
      this->addressingType = getAddressingType(thirdByte);
      
      if (this->addressingType == "immediate" || this->addressingType == "registerIndirectOffset" || this->addressingType == "memory"|| this->addressingType == "registerDirectAddend"){
          this->dataHigh = readByteFromMemory(*rpc);
          this->dataLow = readByteFromMemory((*rpc) + 1);
          (*rpc)++;
          (*rpc)++;
      }
    //END

    //INSTRUCTION EXECUTE 
      if(this->updateType == "subtractTwoBefore"){
        this->registers[this->sourceRegisterIndex] -= 2;
      }
      if(this->updateType == "addTwoBefore"){
        this->registers[this->sourceRegisterIndex] += 2;
      }

      //--------------------------------setOperand------------------------------------------------------
        if(this->addressingType == "immediate" || this->addressingType == "registerDirectAddend"){
          *rpc = this->previousPc;
          jumpOnInterruptRoutineOfEntry(1);
          return;
        }
        else if(this->addressingType == "registerDirect"){
          this->registers[this->sourceRegisterIndex] = this->registers[this->destinationRegisterIndex];
        }
        else if(this->addressingType == "registerIndirect"){
          short value = this->registers[this->destinationRegisterIndex];

          size_t n_zero = 2;

          stringstream ss;

          ss<< hex << (((value)>>8) & 0x00ff);
          string hValue = ss.str();
          hValue = string(n_zero - min(n_zero, hValue.size()), '0') + hValue;
          ss.str("");

          ss<< hex << (((value)) & 0x00ff);
          string lValue = ss.str();
          lValue = string(n_zero - min(n_zero, lValue.size()), '0') + lValue;
          ss.str("");
           
          writeByteToMemory(this->registers[this->sourceRegisterIndex], lValue);
          writeByteToMemory(this->registers[this->sourceRegisterIndex] + 1, hValue);
        }
        else if(this->addressingType == "registerIndirectOffset"){
          short value = this->registers[this->destinationRegisterIndex];
          
          size_t n_zero = 2;

          stringstream ss;

          ss<< hex << (((value)>>8) & 0x00ff);
          string hValue = ss.str();
          hValue = string(n_zero - min(n_zero, hValue.size()), '0') + hValue;
          ss.str("");

          ss<< hex << (((value)) & 0x00ff);
          string lValue = ss.str();
          lValue = string(n_zero - min(n_zero, lValue.size()), '0') + lValue;
          ss.str("");

          string data = this->dataHigh + this->dataLow;
          short dataPayload = stoi(data, nullptr, 16);

          int address = this->registers[this->sourceRegisterIndex] + dataPayload;
          
          writeByteToMemory(address, lValue);
          writeByteToMemory(address + 1, hValue);
        }
        else if(this->addressingType == "memory"){
          short value = this->registers[this->destinationRegisterIndex];
          
          size_t n_zero = 2;

          stringstream ss;

          ss<< hex << (((value)>>8) & 0x00ff);
          string hValue = ss.str();
          hValue = string(n_zero - min(n_zero, hValue.size()), '0') + hValue;
          ss.str("");

          ss<< hex << (((value)) & 0x00ff);
          string lValue = ss.str();
          lValue = string(n_zero - min(n_zero, lValue.size()), '0') + lValue;
          ss.str("");

          string data = this->dataHigh + this->dataLow;
          short address = stoi(data, nullptr, 16);
          

          writeByteToMemory(address, lValue);
          writeByteToMemory(address + 1, hValue);    
        }

      //--------------------------------end-------------------------------------------------------------
    
      if(this->updateType == "subtractTwoAfter"){
         this->registers[this->sourceRegisterIndex] -= 2;
      }
      if(this->updateType == "addTwoAfter"){
         this->registers[this->sourceRegisterIndex] += 2;
      }

    //END
  }
  else if(instructionMnemonic == "ldr"){
    //ldr or pop instructions
    
    //SIZE - (3 or 5)B

    string secondByte = readByteFromMemory(*rpc);
    (*rpc)++;
    string thirdByte = readByteFromMemory(*rpc);
    (*rpc)++;

    //INSTRUCTION DECODE
      this->destinationRegisterIndex = getDestinationRegisterIndex(secondByte);
      this->sourceRegisterIndex = getSourceRegisterIndex(secondByte);

      this->updateType = getUpdateTypeForSourceRegister(thirdByte);
      this->addressingType = getAddressingType(thirdByte);

      if (this->addressingType == "immediate" || this->addressingType == "registerIndirectOffset" || this->addressingType == "memory"|| this->addressingType == "registerDirectAddend"){
          this->dataHigh = readByteFromMemory(*rpc);
          this->dataLow = readByteFromMemory((*rpc) + 1);
          (*rpc)++;
          (*rpc)++;
      }
    //END

    //INSTRUCTION EXECUTE 
      if(this->updateType == "subtractTwoBefore"){
        this->registers[this->sourceRegisterIndex] -= 2;
      }
      if(this->updateType == "addTwoBefore"){
        this->registers[this->sourceRegisterIndex] += 2;
      }

      if(this->addressingType == "registerDirectAddend"){
        *rpc = this->previousPc;
        jumpOnInterruptRoutineOfEntry(1);
        return;
      }

      string helpOP="0x" + getOperandBasedOnAddressingType();
      short operand = stoi(helpOP, nullptr, 16);

      this->registers[this->destinationRegisterIndex] = operand;

      if(this->updateType == "subtractTwoAfter"){
         this->registers[this->sourceRegisterIndex] -= 2;
      }
      if(this->updateType == "addTwoAfter"){
         this->registers[this->sourceRegisterIndex] += 2;
      }
    //END 

    return;
  }
  else if(instructionMnemonic == "xchg"){
    //SIZE - 2B

    string secondByte = readByteFromMemory(*rpc);
    (*rpc)++;

    //INSTRUCTION DECODE
    this->destinationRegisterIndex = getDestinationRegisterIndex(secondByte);
    this->sourceRegisterIndex = getSourceRegisterIndex(secondByte);
    //END

    //INSTRUCTION EXECUTE 
      short temporaryValue = registers[this->destinationRegisterIndex];
      registers[this->destinationRegisterIndex] = registers[this->sourceRegisterIndex];
      registers[this->sourceRegisterIndex] = temporaryValue;
    //END
  }
  else if(instructionMnemonic == "add"){
   //SIZE - 2B

    string secondByte = readByteFromMemory(*rpc);
    (*rpc)++;

    //INSTRUCTION DECODE
    this->destinationRegisterIndex = getDestinationRegisterIndex(secondByte);
    this->sourceRegisterIndex = getSourceRegisterIndex(secondByte);
    //END

    //INSTRUCTION EXECUTE
    this->registers[this->destinationRegisterIndex] = registers[this->destinationRegisterIndex] + registers[this->sourceRegisterIndex];
    //END 
  }
  else if(instructionMnemonic == "sub"){
   //SIZE - 2B

    string secondByte = readByteFromMemory(*rpc);
    (*rpc)++;

    //INSTRUCTION DECODE
    this->destinationRegisterIndex = getDestinationRegisterIndex(secondByte);
    this->sourceRegisterIndex = getSourceRegisterIndex(secondByte);
    //END

    //INSTRUCTION EXECUTE
    this->registers[this->destinationRegisterIndex] = registers[this->destinationRegisterIndex] - registers[this->sourceRegisterIndex];
    //END 
  }
  else if(instructionMnemonic == "mul"){
    //SIZE - 2B

    string secondByte = readByteFromMemory(*rpc);
    (*rpc)++;

    //INSTRUCTION DECODE
    this->destinationRegisterIndex = getDestinationRegisterIndex(secondByte);
    this->sourceRegisterIndex = getSourceRegisterIndex(secondByte);
    //END

    //INSTRUCTION EXECUTE
    this->registers[this->destinationRegisterIndex] = registers[this->destinationRegisterIndex] * registers[this->sourceRegisterIndex];
    //END 
  }
  else if(instructionMnemonic == "div"){
    //SIZE - 2B

    string secondByte = readByteFromMemory(*rpc);
    (*rpc)++;

    //INSTRUCTION DECODE
    this->destinationRegisterIndex = getDestinationRegisterIndex(secondByte);
    this->sourceRegisterIndex = getSourceRegisterIndex(secondByte);
    //END

    if (registers[this->sourceRegisterIndex] == 0){
        *rpc = this->previousPc;
        jumpOnInterruptRoutineOfEntry(1);
        return;
    }

    //INSTRUCTION EXECUTE
    this->registers[this->destinationRegisterIndex] = registers[this->destinationRegisterIndex] / registers[this->sourceRegisterIndex];
    //END 
  }
  else if(instructionMnemonic == "cmp"){
    //SIZE - 2B

    string secondByte = readByteFromMemory(*rpc);
    (*rpc)++;

    //INSTRUCTION DECODE
    this->destinationRegisterIndex = getDestinationRegisterIndex(secondByte);
    this->sourceRegisterIndex = getSourceRegisterIndex(secondByte);
    //END

    //INSTRUCTION EXECUTE
    short operandA = this->registers[this->destinationRegisterIndex];
    short operandB = this->registers[this->sourceRegisterIndex];
    short temporaryValue = operandA - operandB;
    
    if(temporaryValue<0) setFlagPSW(N);
    else resetFlagPSW(N);

    if(temporaryValue==0) setFlagPSW(Z);
    else resetFlagPSW(Z);

    if(operandA < operandB) setFlagPSW(C);
    else resetFlagPSW(C);

    if ((operandA < 0 && operandB > 0 && (operandA - operandB) > 0) || (operandA > 0 && operandB < 0 && (operandA - operandB) < 0)){
      setFlagPSW(O);
    }
    else{
      resetFlagPSW(O);
    }
    //END 
  }
  else if(instructionMnemonic == "not"){
    //SIZE - 2B

    string secondByte = readByteFromMemory(*rpc);
    (*rpc)++;

    //INSTRUCTION DECODE
    this->destinationRegisterIndex = getDestinationRegisterIndex(secondByte);
    this->sourceRegisterIndex = getSourceRegisterIndex(secondByte);
    //END

    //INSTRUCTION EXECUTE
    this->registers[this->destinationRegisterIndex] = ~registers[this->destinationRegisterIndex];
    //END 
  }
  else if(instructionMnemonic == "and"){
    //SIZE - 2B

    string secondByte = readByteFromMemory(*rpc);
    (*rpc)++;

    //INSTRUCTION DECODE
    this->destinationRegisterIndex = getDestinationRegisterIndex(secondByte);
    this->sourceRegisterIndex = getSourceRegisterIndex(secondByte);
    //END

    //INSTRUCTION EXECUTE
    this->registers[this->destinationRegisterIndex] = registers[this->destinationRegisterIndex] & registers[this->sourceRegisterIndex];
    //END 
  }
  else if(instructionMnemonic == "or"){
    //SIZE - 2B

    string secondByte = readByteFromMemory(*rpc);
    (*rpc)++;

    //INSTRUCTION DECODE
    this->destinationRegisterIndex = getDestinationRegisterIndex(secondByte);
    this->sourceRegisterIndex = getSourceRegisterIndex(secondByte);
    //END

    //INSTRUCTION EXECUTE
    this->registers[this->destinationRegisterIndex] = registers[this->destinationRegisterIndex] | registers[this->sourceRegisterIndex];
    //END 
  }
  else if(instructionMnemonic == "xor"){
    //SIZE - 2B

    string secondByte = readByteFromMemory(*rpc);
    (*rpc)++;

    //INSTRUCTION DECODE
    this->destinationRegisterIndex = getDestinationRegisterIndex(secondByte);
    this->sourceRegisterIndex = getSourceRegisterIndex(secondByte);
    //END

    //INSTRUCTION EXECUTE
    this->registers[this->destinationRegisterIndex] = registers[this->destinationRegisterIndex] ^ registers[this->sourceRegisterIndex];
    //END 
  }
  else if(instructionMnemonic == "test"){
    //SIZE - 2B

    string secondByte = readByteFromMemory(*rpc);
    (*rpc)++;

    //INSTRUCTION DECODE
    this->destinationRegisterIndex = getDestinationRegisterIndex(secondByte);
    this->sourceRegisterIndex = getSourceRegisterIndex(secondByte);
    //END

    //INSTRUCTION EXECUTE
    short operandA = this->registers[this->destinationRegisterIndex];
    short operandB = this->registers[this->sourceRegisterIndex];
    short temporaryValue = operandA & operandB;
    
    if(temporaryValue<0) setFlagPSW(N);
    else resetFlagPSW(N);

    if(temporaryValue==0) setFlagPSW(Z);
    else resetFlagPSW(Z);
  
    //END 
  }
  else if(instructionMnemonic == "shl"){
    //SIZE - 2B

    string secondByte = readByteFromMemory(*rpc);
    (*rpc)++;

    //INSTRUCTION DECODE
    this->destinationRegisterIndex = getDestinationRegisterIndex(secondByte);
    this->sourceRegisterIndex = getSourceRegisterIndex(secondByte);
    //END

    //INSTRUCTION EXECUTE

    short destinationRegValue = this->registers[this->destinationRegisterIndex];
    short sourceRegValue = this->registers[this->sourceRegisterIndex];
    this->registers[this->destinationRegisterIndex] = this->registers[this->destinationRegisterIndex] << this->registers[this->sourceRegisterIndex];
    
    if(this->registers[this->destinationRegisterIndex]<0) setFlagPSW(N);
    else resetFlagPSW(N);

    if(this->registers[this->destinationRegisterIndex]==0) setFlagPSW(Z);
    else resetFlagPSW(Z);

    if (sourceRegValue < 16 && ((destinationRegValue >> (16 - sourceRegValue)) & 1)){
      setFlagPSW(C);
    }
    else{
      resetFlagPSW(C);
    }

    //END 
  }
  else if(instructionMnemonic == "shr"){
    //SIZE - 2B

    string secondByte = readByteFromMemory(*rpc);
    (*rpc)++;

    //INSTRUCTION DECODE
    this->destinationRegisterIndex = getDestinationRegisterIndex(secondByte);
    this->sourceRegisterIndex = getSourceRegisterIndex(secondByte);
    //END

    //INSTRUCTION EXECUTE

    short destinationRegValue = this->registers[this->destinationRegisterIndex];
    short sourceRegValue = this->registers[this->sourceRegisterIndex];

    this->registers[this->destinationRegisterIndex] = this->registers[this->destinationRegisterIndex] >> this->registers[this->sourceRegisterIndex];
    
    if(this->registers[this->destinationRegisterIndex]<0) setFlagPSW(N);
    else resetFlagPSW(N);

    if(this->registers[this->destinationRegisterIndex]==0) setFlagPSW(Z);
    else resetFlagPSW(Z);

    if ((destinationRegValue >> (sourceRegValue - 1)) & 1){
      setFlagPSW(C);
    }
    else{
      resetFlagPSW(C);
    }

    //END 
  }
}

void Emulator::jumpOnInterruptRoutineOfEntry(int entryNumber){
  string lowerByte = readByteFromMemory(2 * (entryNumber % 8));
  string higherByte= readByteFromMemory(2 * (entryNumber % 8) + 1);

  string address = "0x" + higherByte + lowerByte;

  size_t n_zero = 2;

  stringstream ss;

  ss<< hex << (((*rpc)>>8) & 0x00ff);
  string hPc = ss.str();
  hPc = string(n_zero - min(n_zero, hPc.size()), '0') + hPc;
  ss.str("");

  ss<< hex << (((*rpc)) & 0x00ff);
  string lPc = ss.str();
  lPc = string(n_zero - min(n_zero, lPc.size()), '0') + lPc;
  ss.str("");

  ss<< hex << (((*rpsw)>>8) & 0x00ff);
  string hPsw = ss.str();
  hPsw = string(n_zero - min(n_zero, hPsw.size()), '0') + hPsw;
  ss.str("");

  ss<< hex << (((*rpsw)) & 0x00ff);
  string lPsw = ss.str();
  lPsw = string(n_zero - min(n_zero, lPsw.size()), '0') + lPsw;
  ss.str("");

  writeWordToStack(hPc, lPc);
  writeWordToStack(hPsw, lPsw);
 
  short addr = stoi(address, nullptr, 16);

  *rpc = addr;  

  setFlagPSW(I);
  setFlagPSW(Tl);
  setFlagPSW(Tr);  
}

string Emulator::readByteFromMemory(int address){
  address = address & 0xFFFF;
  if(address >= this->memory.size()) throw EmulatorException("Program tries to read data from undefined part of the memory");
  return this->memory.at(address);
}

void Emulator::writeByteToMemory(int address, string byte){
  size_t n_zero = 2;
  byte = string(n_zero - min(n_zero, byte.size()), '0') + byte;
  address = address & 0xFFFF;
  this->memory.at(address) = byte;
}

string Emulator::readWordFromStack(){
    string returnValue="";
    string lowerByte = readByteFromMemory(*rsp);
    string higherByte = readByteFromMemory((*rsp)+1);
    returnValue = higherByte + lowerByte;
    (*rsp)++;
    (*rsp)++;
    return returnValue;
}

void Emulator::writeWordToStack(string higherByte, string lowerByte){
    (*rsp)--;
    (*rsp)--;
    writeByteToMemory((*rsp) + 1, higherByte);
    writeByteToMemory(*rsp, lowerByte);
}