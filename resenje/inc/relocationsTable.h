#ifndef _relocationsTable_h_
#define _relocationsTable_h_

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

class RelocationsTableEntry{
public:

  RelocationsTableEntry(unsigned long sectionId, long offset, string type, unsigned long symbolId, string symbolName,long addend);


  long getOffset(){
    return this->offset;
  }

  string getType(){
    return this->type;
  }
  
  unsigned long getSymbolId(){
    return this->symbolId;
  }

  unsigned long getSectionId(){
    return this->sectionId;
  }

  long getAddend(){
    return this->addend;
  }
  string getSymbolName(){
    return this->symbolName;
  }

  void setOffset(long offset){
    this->offset = offset;
  }

  void setSymbolId(unsigned long symbolId){
    this->symbolId = symbolId;
  }

  void setType(string type){
    this->type = type;
  }


  void setAddend(long addend){
    this->addend = addend;
  }
  void setSectionId(unsigned long sectionId){
    this->sectionId = sectionId;
  }
  void setSymbolName(string symbolName){
    this->symbolName = symbolName;
  }
  bool getIsUsedInDirective(){return this->isUsedInDirective;}
  void setIsUsedInDirective(bool isUsedInDirective){this->isUsedInDirective = isUsedInDirective;}

private:
  unsigned long sectionId;
  long offset;
  string type;
  unsigned long symbolId;
  string symbolName;
  long addend;
  bool isUsedInDirective;
};

class RelocationsTable{
public:

  void addToRelocationsTable(RelocationsTableEntry entry);
  void printRelocationsTable(ofstream& outputFile, unsigned long sectionId);
  vector<RelocationsTableEntry>& getRelocationsTableRefference(){return this->relocationsTable;}
  unsigned long getRelocationsTableSectionId();

private:
vector<RelocationsTableEntry> relocationsTable;

};


#endif