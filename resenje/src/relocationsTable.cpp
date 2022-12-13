#include "../inc/relocationsTable.h"

//---------------------------------------------------------------------------------------------
//                                RELOCATIONS TABLE ENTRY                                     |
//---------------------------------------------------------------------------------------------

RelocationsTableEntry::RelocationsTableEntry(unsigned long sectionId, long offset, string type, unsigned long symbolId,string symbolName,long addend){
  this->sectionId = sectionId;
  this->offset = offset;
  this->type = type;
  this->symbolId = symbolId;
  this->symbolName = symbolName;
  this->addend = addend;
}



//---------------------------------------------------------------------------------------------
//                                   RELOCATIONS TABLE                                        | 
//---------------------------------------------------------------------------------------------


void RelocationsTable::addToRelocationsTable(RelocationsTableEntry entry){
  this->relocationsTable.push_back(entry);
}

void RelocationsTable::printRelocationsTable(ofstream& outputFile, unsigned long sectionId){

  outputFile<<left;
  outputFile<<setw(20)<<setfill(' ')<<"offset"<<" | "
  <<setw(20)<<"type" << " | "
  <<setw(20)<< "symbolName"<<" | "
  <<setw(20)<<"addend"<<" | "
  <<setw(20)<<"ins/dir"<<endl;

  for(int i=1; i<106;i++) outputFile<<"─";
  outputFile<<endl;

  for(RelocationsTableEntry entry: this->relocationsTable){
    if(entry.getSectionId() == sectionId){
      string symName = to_string(entry.getSymbolId()) + "(" + entry.getSymbolName() + ")";
      string insDir = entry.getIsUsedInDirective()==true?"dir":"ins";
      outputFile<<dec<<setw(20)<< entry.getOffset()<<" | "
      <<setw(20)<< entry.getType() << " | "
      <<setw(20)<< symName<< " | "
      <<setw(20)<<dec<<entry.getAddend()<< " | "
      <<setw(20)<<insDir<<endl;
    }
  }
  outputFile<<endl<<endl;
}

unsigned long RelocationsTable::getRelocationsTableSectionId(){
  return this->relocationsTable[0].getSectionId();
}