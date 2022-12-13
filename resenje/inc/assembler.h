#ifndef _assembler_h_
#define _assembler_h_

#include <iostream>
#include <fstream>
#include "symbolsTable.h"
#include "relocationsTable.h"
#include "assemblerExceptions.h"
#include "parser.h"
#include "lexer.h"
#include <string>
#include <vector>
#include <regex>
#include "constants.h"
#include "instruction.h"
#include <bitset>

using namespace std;

class ForwardRefferenceLink {
    public:
      ForwardRefferenceLink(string symbolName, unsigned long sectionIndex, unsigned long patch, string relocationType, unsigned long nextInstructionLocationCounter){
        this->symbolName = symbolName;
        this->sectionIndex = sectionIndex;
        this->patch = patch;
        this->relocationType = relocationType;
        this->nextInstructionLocationCounter = nextInstructionLocationCounter;
      }

      string getSymbolName(){return this->symbolName;}
      unsigned long getSectionIndex(){return this->sectionIndex;}
      unsigned long getPatch(){return this->patch;}
      bool getModifyOneByte(){return this->modifyOneByte;}
      string getRelocationType(){return this->relocationType;}
      unsigned long getNextInstructionLocationCounter(){return this->nextInstructionLocationCounter;}
      bool getIsUsedInDirective(){return this->isUsedInDirective;}
      void setIsUsedInDirective(bool isUsedInDirective){this->isUsedInDirective = isUsedInDirective;} 

    private:
      string symbolName;
      unsigned long sectionIndex;
      unsigned long patch; //address
      bool modifyOneByte;
      string relocationType;
      unsigned long nextInstructionLocationCounter;
      bool isUsedInDirective;
};


static vector <Instruction> allInstructions = {
    Instruction("halt", "00000000", 0),
    Instruction("int", "00010000", 1),
    Instruction("iret", "00100000", 0),
    Instruction("call", "00110000", 1),
    Instruction("ret", "01000000", 0),
   
   
    Instruction("jmp", "01010000", 1),
    Instruction("jeq", "01010001", 1),
    Instruction("jne", "01010010", 1),
    Instruction("jgt", "01010011", 1),

    Instruction("push", "10110000", 1), //push implemented with str instruction with predecrement - 2B
    Instruction("pop", "10100000", 1), //pop implemented with ldr instruction with postincrement - 2B

    Instruction("xchg", "01100000", 2),
    
    Instruction("add", "01110000", 2),
    Instruction("sub", "01110001", 2),
    Instruction("mul", "01110010", 2),
    Instruction("div", "01110011", 2),
    Instruction("cmp", "01110100", 2),

    Instruction("not", "10000000", 1),
    Instruction("and", "10000001", 2),
    Instruction("or", "10000010", 2),
    Instruction("xor", "10000011", 2),
    Instruction("test", "10000100", 2),

    Instruction("shl", "10010000", 2),
    Instruction("shr", "10010001", 2),

    Instruction("ldr", "10100000", 2),
    Instruction("str", "10110000", 2)
};


class Assembler{

  public:

    Assembler(string inputFile, string outputFile);
    void pass();
    void assemble();
    void generateBinaryCodeByte(uint8_t byte);
    void resolveGlobalSymbols();
    void resolveExternSymbols();
    void resolveForwardRefferenceLinks();
    void printToOutputFile();
    ~Assembler();

  private:

    ifstream inputFile;
    ofstream outputFile;
    Parser* parser;
    Lexer* lexer;
    SymbolsTable* symbolsTable;
    RelocationsTable* relocationsTable;
    vector<string> allInputLines; //represent every line in .as file parsed with comments and text after end directive deleted 
    unsigned long locationCounter;
    long currentSectionIndex;
    unsigned long currentLine;
    vector<string> globalDirectiveSymbols;
    vector<string> externDirectiveSymbols;
    map<long, vector<uint8_t>> binaryCodeMap; 
    vector<ForwardRefferenceLink> allFowardRefferenceLinks;
};

#endif