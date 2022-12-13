#include "../inc/assembler.h"

Assembler::Assembler(string inputFile, string outputFile){
  this->inputFile.open(inputFile, fstream::in);
  this->outputFile.open(outputFile, fstream::out | fstream::trunc);

  if(!this->inputFile.is_open()){
    throw AssemblerException("Can not open input file");
  }

  this->currentSectionIndex = START_SECTION_INDEX;
  this->locationCounter = 0;
  this->currentLine = 0;

  this->lexer = new Lexer();
  this->parser = new Parser();
  this->symbolsTable = new SymbolsTable();
  this->symbolsTable->addEmptySection();
  this->relocationsTable = new RelocationsTable();

  this->allInputLines = this->parser->parseTextFile(this->inputFile);
  this->allInputLines = this->parser->deleteAllComments(this->allInputLines);
  this->allInputLines = this->parser->deleteAfterEndDirective(this->allInputLines);
  this->allInputLines = this->parser->removeExtraWhiteSpaces(this->allInputLines);

 
}

void Assembler::assemble(){
  this->pass();
  this->resolveGlobalSymbols();
  this->resolveExternSymbols();
  this->resolveForwardRefferenceLinks();
  this->printToOutputFile();
}

void Assembler::pass(){
  
  bool directiveOrInstructionFoundInOneLine;
  for(string line: allInputLines){
    this->currentLine++;
    vector <string> alltokensFromLine = this->lexer->getAllTokensFromLine(line);
    directiveOrInstructionFoundInOneLine = false;

    for(int i=0; i<alltokensFromLine.size(); i++){
      string currentToken = alltokensFromLine[i];

      if(!regex_match(alltokensFromLine[0],INSTRUCTIONS_REGEX) && !regex_match(alltokensFromLine[0], DIRECTIVE_REGEX) && !regex_match(alltokensFromLine[0], LABEL_REGEX)){
        throw AssemblerException("Wrong syntax, symbol " + currentToken + " is not label/directive/instruction",this->currentLine);
      }
      
      if(regex_match(currentToken, LABEL_REGEX)){
        if(this->currentSectionIndex == 0){
          throw AssemblerException("Labels need to be defined in the section", this->currentLine);
        }
      } 

      if(regex_match(currentToken,INSTRUCTIONS_REGEX) || regex_match(currentToken,DIRECTIVE_REGEX)){
        if(directiveOrInstructionFoundInOneLine == false) directiveOrInstructionFoundInOneLine = true;
        else{
          throw AssemblerException("Can not have more than one directive or instruction in one line", this->currentLine);
        }
      }
      //.GLOBAL AND .EXTERN DIRECTIVES
      if(regex_match(currentToken, DIRECTIVE_GLOBAL_EXTERN_REGEX)){

        if(alltokensFromLine.size()<2){
          throw AssemblerException("Directives .global and .extern need to have at least one symbol", this->currentLine);
        }

        string lineWithoutLabel = this->parser->removeLabelFromLine(line);

        if(currentToken==".global" && !regex_match(lineWithoutLabel, GLOBAL_DIRECTIVE_SYNTAX_CHECK_REGEX)) throw AssemblerException("Wrong syntax for global directive", this->currentLine);
        if(currentToken==".extern" && !regex_match(lineWithoutLabel, EXTERN_DIRECTIVE_SYNTAX_CHECK_REGEX)) throw AssemblerException("Wrong syntax for extern directive", this->currentLine);

        
        for (int j=i+1; j<alltokensFromLine.size(); j++){

          if(currentToken == ".global"){
           
            this->globalDirectiveSymbols.push_back(alltokensFromLine[j]);

          }

          if(currentToken == ".extern"){
          
            this->externDirectiveSymbols.push_back(alltokensFromLine[j]);

          }
        }
      }
      //END

      //LABELS
      if(regex_match(currentToken, LABEL_REGEX)){
        
        string currentLabelName = currentToken;
        int position = line.find(LABEL_SYMBOL);
        if(position != string::npos){
          currentLabelName = currentLabelName.substr(0,position);
        }

        if(currentToken != alltokensFromLine[0]) throw AssemblerException("Labels need to be on the start of the line", this->currentLine);
       
        if(this->currentSectionIndex == 0){
          throw AssemblerException("Label "+ currentLabelName+ " can not be defined outside of any section", this->currentLine);
        }
        
        if(i+1<alltokensFromLine.size()){
          if(!regex_match(alltokensFromLine[i+1],INSTRUCTIONS_REGEX) && !regex_match(alltokensFromLine[i+1],DIRECTIVE_REGEX)) 
          throw AssemblerException("Unknown directive/instruction after label: " + currentLabelName, this->currentLine);
        }

        if (this->symbolsTable->isSymbolDefined(currentLabelName))
            throw AssemblerException("Label " + currentLabelName + " is already defined", this->currentLine);
        
        if(!this->symbolsTable->doesSymbolExist(currentLabelName)){
          SymbolsTableEntry entry(this->locationCounter,0, "NOTYPE","LOCAL", to_string(this->currentSectionIndex), currentLabelName, true);
          this->symbolsTable->addToSymbolsTable(entry);
        }
        else{
          this->symbolsTable->setIsDefinedForSymbol(currentLabelName, true);
          this->symbolsTable->setValueForSymbol(currentLabelName, this->locationCounter);
          this->symbolsTable->setSectionIndexForsymbol(currentLabelName, to_string(this->currentSectionIndex));
        }

      }
      //END

      //.SECTION DIRECTIVE
      if(regex_match(currentToken, DIRECTIVE_SECTION_REGEX)){
        if(this->currentSectionIndex != 0){
          this->symbolsTable->setSectionSize(this->locationCounter, to_string(currentSectionIndex));
        }
        this->locationCounter = 0;
        

        if(alltokensFromLine.size() != 2) throw AssemblerException("Section requires exactly one valid name",this->currentLine);
        string sectionName = alltokensFromLine[i+1];

        SymbolsTableEntry entry(0, 0, "SECTION", "LOCAL", sectionName, true);
        this->currentSectionIndex = stol(entry.getSectionIndexString());
        this->symbolsTable->addToSymbolsTable(entry);
      }
      //END

      //.END DIRECTIVE
      if(regex_match(currentToken, DIRECTIVE_END_REGEX)){
    
        if(this->currentSectionIndex != 0){
          this->symbolsTable->setSectionSize(this->locationCounter, to_string(currentSectionIndex));
        }

      }
      //END

      //.SKIP AND .WORD DIRECTIVES
      if(regex_match(currentToken, DIRECTIVES_SKIP_WORD_REGEX)){
        if(currentToken == ".skip"){
          
          if(this->currentSectionIndex == 0) throw AssemblerException("Skip directive need to be in the section", this->currentLine);

          if(alltokensFromLine.size() != 2){
            throw AssemblerException("Skip directive can have only one parameter", this->currentLine);
          }

          if(!regex_match(alltokensFromLine[1], LITERAL_DECIMAL_REGEX)&& !regex_match(alltokensFromLine[1], LITERAL_HEXADECIMAL_REGEX)){
            throw AssemblerException("Skip directive should be followed by one literal", this->currentLine);
          }

          unsigned long counter = 0;
          
          if(regex_match(alltokensFromLine[1], LITERAL_DECIMAL_REGEX)){ 
            counter = stoul(alltokensFromLine[1]);
            this->locationCounter += stoul(alltokensFromLine[1]);
          }
          if(regex_match(alltokensFromLine[1], LITERAL_HEXADECIMAL_REGEX)) {
            counter = stoul(alltokensFromLine[1],nullptr,16);
            this->locationCounter += stoul(alltokensFromLine[1],nullptr,16);
          }
          
          for(int i=0; i<counter; i++) this->generateBinaryCodeByte(0);

        }

        if(currentToken == ".word"){
          unsigned long value = 0;

          if(this->currentSectionIndex == 0) throw AssemblerException("Word directive need to be in the section", this->currentLine);

          if(alltokensFromLine.size()-i < 2){
            throw AssemblerException("Word directive need to have at least one parameter", this->currentLine);
          }
          
          string lineWithoutLabel = this->parser->removeLabelFromLine(line);
          if(!regex_match(lineWithoutLabel, WORD_DIRECTIVE_SYNTAX_CHECK_REGEX)) throw AssemblerException("Wrong syntax for word directive", this->currentLine);

          for (int j=i+1; j<alltokensFromLine.size(); j++){

            if(!regex_match(alltokensFromLine[j], LITERAL_DECIMAL_REGEX) && !regex_match(alltokensFromLine[j],LITERAL_HEXADECIMAL_REGEX) && !regex_match(alltokensFromLine[j],SYMBOL_REGEX)){
              throw AssemblerException("Word directive need to be followed with one or more literals/symbols", this->currentLine);
            }

            if (regex_match(alltokensFromLine[j], LITERAL_DECIMAL_REGEX))
              value = stoul(alltokensFromLine[j]);
            else if (regex_match(alltokensFromLine[j],LITERAL_HEXADECIMAL_REGEX))	
              value = stoul(alltokensFromLine[j], nullptr, 16);
            //SYMBOL
            else 
            {
              value = 0;
              ForwardRefferenceLink newLink(alltokensFromLine[j], this->currentSectionIndex, this->locationCounter, RELOCATION_ABSOLUTE, 0);
              newLink.setIsUsedInDirective(true);
              allFowardRefferenceLinks.push_back(newLink);
            }

            generateBinaryCodeByte((uint8_t)(value & 0xFF));
            generateBinaryCodeByte((uint8_t)((value >> 8) & 0xFF));

            this->locationCounter += 2;
          }

        }
      }
      //END


      //INSTRUCTIONS
      if(regex_match(currentToken, INSTRUCTIONS_REGEX)){
        
        if(this->currentSectionIndex == 0){
          throw AssemblerException("Instructions need to be defined in the section", this->currentLine);
        }

        Instruction currentInstruction;

        for(int i=0; i<allInstructions.size();i++){
          if(allInstructions[i].getMnemonic() == currentToken){
            currentInstruction.setMnemonic(allInstructions[i].getMnemonic());
            currentInstruction.setInstructionDescriptor(allInstructions[i].getInstructionDescriptor());
            currentInstruction.setOperandsRequired(allInstructions[i].getOperandsRequired());
          }
        }

        if(alltokensFromLine.size()-i-1 != currentInstruction.getOperandsRequired()) throw AssemblerException("Instruction "+currentInstruction.getMnemonic()+" requires " + to_string(currentInstruction.getOperandsRequired())+ " operands",this->currentLine);
        
        vector<string> operands; 
        for(int k = i+1; k<alltokensFromLine.size();k++){
          operands.push_back(alltokensFromLine[k]);
        }
        
        string instructionLineWithoutLabel = this->parser->removeLabelFromLine(line);

        if((!regex_match(instructionLineWithoutLabel, NO_OPERANDS_REGEX) && regex_match(currentInstruction.getMnemonic(),NO_OPERANDS_INSTRUCTIONS_REGEX))
        || (!regex_match(instructionLineWithoutLabel, ONE_OPERAND_REGEX) && regex_match(currentInstruction.getMnemonic(),ONE_OPERAND_INSTRUCTIONS_REGEX))
        || (!regex_match(instructionLineWithoutLabel, TWO_OPERANDS_REGEX) && regex_match(currentInstruction.getMnemonic(),TWO_OPERANDS_INSTRUCTIONS_REGEX)))
        throw AssemblerException("Wrong instruction format for instruction " + currentInstruction.getMnemonic(), this->currentLine);
      
        if((currentToken=="ldr"||currentToken=="str")&&(
        !regex_match(instructionLineWithoutLabel, LDR_STR_DIRECT_LITERAL_REGEX)&&
        !regex_match(instructionLineWithoutLabel, LDR_STR_DIRECT_SYMBOL_REGEX)&&
        !regex_match(instructionLineWithoutLabel, LDR_STR_REGISTER_DIRECT_REGEX)&&
        !regex_match(instructionLineWithoutLabel, LDR_STR_REGISTER_INDIRECT_REGEX)&&
        !regex_match(instructionLineWithoutLabel, LDR_STR_MEMORY_DIRECT_ABOSOLUTE_REGEX)&&
        !regex_match(instructionLineWithoutLabel, LDR_STR_MEMORY_DIRECT_LITERAL_REGEX)&&
        !regex_match(instructionLineWithoutLabel, LDR_STR_PC_RELATIVE_REGEX)&&
        !regex_match(instructionLineWithoutLabel, LDR_STR_DISPLAY_LITERAL_REGEX)&&
        !regex_match(instructionLineWithoutLabel, LDR_STR_DISPLAY_SYMBOL_REGEX)      
        )) throw AssemblerException("Wrong address type for instruction: " + currentToken, this->currentLine);


        if((currentToken=="call" || currentToken=="jmp" || currentToken=="jeq"|| currentToken=="jne"||currentToken=="jgt")&&(
        !regex_match(instructionLineWithoutLabel, JUMP_CALL_DIRECT_REGEX)&&
        !regex_match(instructionLineWithoutLabel, JUMP_CALL_REGISTER_DIRECT_REGEX)&&
        !regex_match(instructionLineWithoutLabel, JUMP_CALL_REGISTER_INDIRECT_REGEX)&&
        !regex_match(instructionLineWithoutLabel, JUMP_CALL_PC_RELATIVE_REGEX)&&
        !regex_match(instructionLineWithoutLabel, JUMP_CALL_ABSOLUTE_REGEX)&&
        !regex_match(instructionLineWithoutLabel, JUMP_CALL_MEMORY_DIRECT_SYMBOL_REGEX)&&
        !regex_match(instructionLineWithoutLabel, JUMP_CALL_MEMORY_DIRECT_LITERAL_REGEX)&&
        !regex_match(instructionLineWithoutLabel, JUMP_CALL_DISPLAY_LITERAL_REGEX)&&
        !regex_match(instructionLineWithoutLabel, JUMP_CALL_DISPLAY_SYMBOL_REGEX)      
        )) throw AssemblerException("Wrong address type for instruction: " + currentToken, this->currentLine);

        
        currentInstruction.setInstructionSize(instructionLineWithoutLabel, operands);
        currentInstruction.resolveDescriptors(instructionLineWithoutLabel, operands);
        
       
        
        //ABSOLUTE RELOCATIONS TYPE
        if(currentInstruction.isAbosluteJumpCall(instructionLineWithoutLabel) 
        || currentInstruction.isDisplaySymbolJumpCall(instructionLineWithoutLabel)
        || (currentInstruction.isMemoryDirectSymbolJumpCall(instructionLineWithoutLabel)&& !currentInstruction.isRegisterDirectJumpCall(instructionLineWithoutLabel))
        || currentInstruction.isDirectSymbolLdrStr(instructionLineWithoutLabel)
        || currentInstruction.isDisplaySymbolLdrStr(instructionLineWithoutLabel)
        || (currentInstruction.isMemoryDirectAbsoluteLdrStr(instructionLineWithoutLabel)&& !currentInstruction.isRegisterDirectLdrStr(instructionLineWithoutLabel))){

          ForwardRefferenceLink newLink(currentInstruction.getSymbolToAddInRelocationsTable(), this->currentSectionIndex, this->locationCounter + 3, RELOCATION_ABSOLUTE, this->locationCounter+currentInstruction.getSize());
          newLink.setIsUsedInDirective(false);
          allFowardRefferenceLinks.push_back(newLink);

        }

        //PCREL RELOCATIONS TYPE
        if(currentInstruction.isPcRelativeJumpCall(instructionLineWithoutLabel)
        || currentInstruction.isPcRelativeLdrStr(instructionLineWithoutLabel)){

          ForwardRefferenceLink newLink(currentInstruction.getSymbolToAddInRelocationsTable(), this->currentSectionIndex, this->locationCounter + 3, RELOCATION_PCRELATIVE, this->locationCounter+currentInstruction.getSize());
          newLink.setIsUsedInDirective(false);
          allFowardRefferenceLinks.push_back(newLink);

        }

        //END OF ADDING TO RELOCATIONS TABLE


        //START OF WRITING DESCRIPTORS TO BINARY CODE
        bitset<8> insDesc = currentInstruction.getInstructionDescriptor();
        bitset<8> regDesc = currentInstruction.getRegistersDescriptor();
        bitset<8> addrMode = currentInstruction.getAddressMode();
        bitset<8> dataHigh = currentInstruction.getDataHigh();
        bitset<8> dataLow = currentInstruction.getDataLow();
       
        generateBinaryCodeByte(static_cast<uint8_t>(insDesc.to_ulong()));
        if(currentInstruction.getSize()>1) generateBinaryCodeByte(static_cast<uint8_t>(regDesc.to_ulong()));
        if(currentInstruction.getSize()>2) generateBinaryCodeByte(static_cast<uint8_t>(addrMode.to_ulong()));
        if(currentInstruction.getSize()>3){
          generateBinaryCodeByte(static_cast<uint8_t>(dataHigh.to_ulong()));
          generateBinaryCodeByte(static_cast<uint8_t>(dataLow.to_ulong()));
        }
        //END 
        
        
        this->locationCounter += currentInstruction.getSize();
      }
      //END
    }
  }

  if(this->currentSectionIndex != 0){
    this->symbolsTable->setSectionSize(this->locationCounter, to_string(currentSectionIndex));
  }
}

Assembler::~Assembler(){
  delete this->symbolsTable;
  delete this->relocationsTable;
  delete this->parser;
  delete this->lexer;

  this->inputFile.close();
  this->outputFile.close();
 
}


void Assembler::generateBinaryCodeByte(uint8_t byte){
  
  if ((this->binaryCodeMap.find(this->currentSectionIndex) == binaryCodeMap.end()))
        this->binaryCodeMap.insert({this->currentSectionIndex, vector<uint8_t>()});

    binaryCodeMap[this->currentSectionIndex].push_back(byte);

}


void Assembler::resolveGlobalSymbols(){
  
  for(string s: this->globalDirectiveSymbols){
    if(this->symbolsTable->isSection(s)) throw AssemblerException("Symbol "+s+" is 'section' and can not be declared as global");
    if(!this->symbolsTable->isSymbolDefined(s)) throw AssemblerException("Symbol "+s+" is declared as global symbol, but is not defined");    
    this->symbolsTable->setBindingForSymbol(s, "GLOBAL");
  }
}

void Assembler::resolveExternSymbols(){
  for(string s: this->externDirectiveSymbols){
    if(this->symbolsTable->doesSymbolExist(s)) throw AssemblerException("Symbol "+s+" is declared as extern symbol, but is defined");
    if(this->symbolsTable->isSectionGlobal(s)) throw AssemblerException("Symbol "+s+" is 'section' and can not be declared as extern");
    SymbolsTableEntry entry(0, 0, "NOTYPE", "GLOBAL", "UNDEFINED", s, false);
    this->symbolsTable->addToSymbolsTable(entry);
  }
}


//backpatching method
void Assembler::resolveForwardRefferenceLinks(){

  for(int i=0; i<this->allFowardRefferenceLinks.size(); i++){
    int addend = 0;
    bool isExternFlag = false;


    if(!this->symbolsTable->doesSymbolExist(this->allFowardRefferenceLinks[i].getSymbolName())){
      throw AssemblerException("Assembling unsuccessful, symbol " + this->allFowardRefferenceLinks[i].getSymbolName() + " is not defined");
    }
     if(!this->symbolsTable->isSymbolDefined(this->allFowardRefferenceLinks[i].getSymbolName())){
      for(int j=0; j<this->externDirectiveSymbols.size();j++)
       if(this->externDirectiveSymbols[j] == this->allFowardRefferenceLinks[i].getSymbolName()) isExternFlag = true;
      if(isExternFlag == false) throw AssemblerException("Assembling unsuccessful, symbol " + allFowardRefferenceLinks[i].getSymbolName() + " is not defined");
    }
   
    if(this->allFowardRefferenceLinks[i].getRelocationType() == RELOCATION_ABSOLUTE){
     
        string currentBind = this->symbolsTable->getBindingfOfSymbol(this->allFowardRefferenceLinks[i].getSymbolName());
        unsigned long currentId = this->symbolsTable->getIdOfSymbol(this->allFowardRefferenceLinks[i].getSymbolName());
        unsigned long currentValue = this->symbolsTable->getValueOfSymbol(this->allFowardRefferenceLinks[i].getSymbolName());
        
        if (currentBind == "LOCAL") {
              //addend = currentValue;
              addend = 0;
              RelocationsTableEntry entry(this->allFowardRefferenceLinks[i].getSectionIndex(), this->allFowardRefferenceLinks[i].getPatch(), RELOCATION_ABSOLUTE, currentId, this->allFowardRefferenceLinks[i].getSymbolName() , addend);
              entry.setIsUsedInDirective(this->allFowardRefferenceLinks[i].getIsUsedInDirective());
              this->relocationsTable->addToRelocationsTable(entry);
        } 
        if (currentBind == "GLOBAL"){
              addend = 0;
              RelocationsTableEntry entry(this->allFowardRefferenceLinks[i].getSectionIndex(), this->allFowardRefferenceLinks[i].getPatch(), RELOCATION_ABSOLUTE, currentId, this->allFowardRefferenceLinks[i].getSymbolName() , addend);
              entry.setIsUsedInDirective(this->allFowardRefferenceLinks[i].getIsUsedInDirective());
              this->relocationsTable->addToRelocationsTable(entry);
        }
    }

    if(this->allFowardRefferenceLinks[i].getRelocationType() == RELOCATION_PCRELATIVE){
        unsigned long currentSection;
        if(isExternFlag == false) currentSection = this->symbolsTable->getSectionOfSymbol(this->allFowardRefferenceLinks[i].getSymbolName());
        
        string currentBind = this->symbolsTable->getBindingfOfSymbol(this->allFowardRefferenceLinks[i].getSymbolName());
        unsigned long currentId = this->symbolsTable->getIdOfSymbol(this->allFowardRefferenceLinks[i].getSymbolName());
        unsigned long currentValue = this->symbolsTable->getValueOfSymbol(this->allFowardRefferenceLinks[i].getSymbolName());
        
        // if (currentSection == this->allFowardRefferenceLinks[i].getSectionIndex()) addend = currentValue - this->allFowardRefferenceLinks[i].getNextInstructionLocationCounter();
        // else if (currentBind == "LOCAL") {
        //       addend = currentValue - 2;
        //       RelocationsTableEntry entry(this->allFowardRefferenceLinks[i].getSectionIndex(), this->allFowardRefferenceLinks[i].getPatch(), RELOCATION_PCRELATIVE, currentId, this->allFowardRefferenceLinks[i].getSymbolName() , addend);
        //       this->relocationsTable->addToRelocationsTable(entry);
        // } 
        // else { 
        //       addend = -2;
        //       RelocationsTableEntry entry(this->allFowardRefferenceLinks[i].getSectionIndex(), this->allFowardRefferenceLinks[i].getPatch(), RELOCATION_PCRELATIVE, currentId,this->allFowardRefferenceLinks[i].getSymbolName() ,addend);
        //       this->relocationsTable->addToRelocationsTable(entry);
        // }

        //EDITED
              addend = -2;
              RelocationsTableEntry entry(this->allFowardRefferenceLinks[i].getSectionIndex(), this->allFowardRefferenceLinks[i].getPatch(), RELOCATION_PCRELATIVE, currentId,this->allFowardRefferenceLinks[i].getSymbolName() ,addend);
              entry.setIsUsedInDirective(this->allFowardRefferenceLinks[i].getIsUsedInDirective());
              this->relocationsTable->addToRelocationsTable(entry);

        //END
    }
  }
}

void Assembler::printToOutputFile(){

  //Symbols Table
  this->outputFile<<"#.symtab"<<endl<<endl;
  this->symbolsTable->printSymbolsTable(this->outputFile);

  int currentByte;
  int currentOffset;

  map<long, vector<uint8_t>>::iterator it;

  //iterate through every section and print relocations table and binary representation of that section
  for (it = this->binaryCodeMap.begin(); it != binaryCodeMap.end(); it++) {

        //relocations table
        
        this->outputFile << "#.rela" <<  this->symbolsTable->getSectionName(it->first)<<endl<<endl;
        this->relocationsTable->printRelocationsTable(this->outputFile, it->first);

        //binary representation of the section
        this->outputFile <<"#"<<this->symbolsTable->getSectionName(it->first)<<endl<<endl;

        currentByte = 0;
        currentOffset = 0;

        this->outputFile<<left;  
        this->outputFile<<setw(8)<<"offset"<<" | "
        <<setw(30)<< "values"<<endl;
        for(int i=1; i<36;i++) this->outputFile<<"─";
        this->outputFile<<endl;


        for (int i = 0; i < it->second.size(); i++) {
          if(currentByte == 0){
            outputFile<<right<<setw(8)<<hex<<setfill('0')<<currentOffset<< " | ";
          }

          this->outputFile << hex << ((it->second[i]  >> 4) & 0xF); //first hexadecimal number
          this->outputFile << hex << (it->second[i]  & 0xF); //second hexadecimal number
        
          if (++currentByte == 8)
          {
              currentByte = 0;
              currentOffset +=8;
              if(i != it->second.size()-1)this->outputFile<<endl;
          }
          else this->outputFile<<" ";   
        }
         
         this->outputFile<<endl<<endl<<endl;
  }
}
