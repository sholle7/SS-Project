#include "../inc/symbolsTable.h"

unsigned long SymbolsTableEntry::ID = 0;
unsigned long SymbolsTableEntry::SECTION_ID = 1;
//---------------------------------------------------------------------------------------------
//                                 SYMBOLS TABLE ENTRY                                        |
//---------------------------------------------------------------------------------------------

SymbolsTableEntry::SymbolsTableEntry(unsigned long value,unsigned long size,string type,string bind, string sectionIndexString,string symbolName, bool isDefined){
  this->id = ID++;
  this->value = value;
  this->size = size;
  this->type = type;
  this->bind = bind;
  this->sectionIndexString = sectionIndexString;
  this->symbolName = symbolName;
  this->isDefined = isDefined;
}

SymbolsTableEntry::SymbolsTableEntry(unsigned long value, unsigned long size, string type, string bind, string symbolName, bool isDefined){
  this->id = ID++;
  this->value = value;
  this->size = size;
  this->type = type;
  this->bind = bind;
  this->sectionIndexString = to_string(SECTION_ID);
  SECTION_ID++;
  this->symbolName = symbolName;
  this->isDefined = isDefined;
}



//---------------------------------------------------------------------------------------------
//                                    SYMBOLS TABLE                                           | 
//---------------------------------------------------------------------------------------------

SymbolsTable::SymbolsTable() {
  
}

void SymbolsTable::addEmptySection(){
  SymbolsTableEntry entry = SymbolsTableEntry(0,0,"NOTYPE", "LOCAL","UNDEFINED","", true);
  addToSymbolsTable(entry);
}

void SymbolsTable::addToSymbolsTable(SymbolsTableEntry entry){
  this->symbolsTable.push_back(entry);
}

void SymbolsTable::printSymbolsTable(ofstream& outputFile){
  
  outputFile<<left;  
  outputFile<<setw(12)<<"id"<<" | "
  <<setw(12)<<"value"<<" | "
  <<setw(12)<< "size"<<" | "
  <<setw(12)<<"type"<<" | "
  <<setw(12)<< "bind"<< " | "
  <<setw(12)<< "sectionIndex"<< " | "
  <<setw(12)<< "symbolName"<<endl;
  
  for(int i=1; i<105;i++) outputFile<<"─";
  outputFile<<endl;

  for(SymbolsTableEntry entry: this->symbolsTable){
      outputFile<<setw(12)<<entry.getId()<< " | " 
      <<setw(12)<<entry.getValue()<<" | "
      <<setw(12)<<entry.getSize()<<" | "
      <<setw(12)<<entry.getType()<< " | "
      <<setw(12)<<entry.getBind()<<" | "
      <<setw(12)<<entry.getSectionIndexString()<<" | "
      <<setw(12)<<entry.getSymbolName()<<endl;
  }
  outputFile<<endl<<endl;

}

bool SymbolsTable::isSymbolDefined(string name){
  for(SymbolsTableEntry entry:this->symbolsTable){
    if(entry.getSymbolName() == name && entry.getIsDefined()==true && entry.getType()=="NOTYPE") return true;
  }
  return false;
}

bool SymbolsTable::doesSymbolExist(string name){
  for(SymbolsTableEntry entry:this->symbolsTable){
    if(entry.getSymbolName() == name && entry.getType()=="NOTYPE") return true;
  }
  return false;
}

void SymbolsTable::setBindingForSymbol(string name, string bind){
   for(int i=0 ; i<this->symbolsTable.size(); i++){
    if(this->symbolsTable[i].getSymbolName() == name && this->symbolsTable[i].getType()=="NOTYPE"){
      this->symbolsTable[i].setBind(bind);
    }
   }
}

void SymbolsTable::setSectionSize(unsigned long value, string sectionIndex){
  for(int i=0 ; i<this->symbolsTable.size(); i++){
    if(this->symbolsTable[i].getSectionIndexString() == sectionIndex && this->symbolsTable[i].getType()=="SECTION"){
      this->symbolsTable[i].setSize(value);
    }
  }
}

string SymbolsTable::getSectionName(long id){
  for(SymbolsTableEntry entry:this->symbolsTable){
    if(entry.getSectionIndexString() == to_string(id) && entry.getType()=="SECTION") return entry.getSymbolName();
  }
  return "";
}

void SymbolsTable::setIsDefinedForSymbol(string name, bool isDefined){
  for(int i=0 ; i<this->symbolsTable.size(); i++){
      if(this->symbolsTable[i].getSymbolName() == name && this->symbolsTable[i].getType()=="NOTYPE"){
        this->symbolsTable[i].setIsDefined(isDefined);
      }
  }
}

void SymbolsTable::setValueForSymbol(string name, unsigned long value){
  for(int i=0 ; i<this->symbolsTable.size(); i++){
        if(this->symbolsTable[i].getSymbolName() == name && this->symbolsTable[i].getType()=="NOTYPE"){
          this->symbolsTable[i].setValue(value);
        }
  }
}

void SymbolsTable::setSectionIndexForsymbol(string name, string sectionIndexString){
  for(int i=0 ; i<this->symbolsTable.size(); i++){
        if(this->symbolsTable[i].getSymbolName() == name && this->symbolsTable[i].getType()=="NOTYPE"){
          this->symbolsTable[i].setSectionIndexString(sectionIndexString);
        }
  }
}

string SymbolsTable::getBindingfOfSymbol(string name){
  for(int i=0 ; i<this->symbolsTable.size(); i++){
        if(this->symbolsTable[i].getSymbolName() == name && this->symbolsTable[i].getType()=="NOTYPE"){
          return this->symbolsTable[i].getBind();
        }
  }
  return "";
}

unsigned long SymbolsTable::getIdOfSymbol(string name){
  for(int i=0 ; i<this->symbolsTable.size(); i++){
        if(this->symbolsTable[i].getSymbolName() == name && this->symbolsTable[i].getType()=="NOTYPE"){
          return this->symbolsTable[i].getId();
        }
  }
  return 0;
}

unsigned long SymbolsTable::getValueOfSymbol(string name){
  for(int i=0 ; i<this->symbolsTable.size(); i++){
        if(this->symbolsTable[i].getSymbolName() == name && this->symbolsTable[i].getType()=="NOTYPE"){
          return this->symbolsTable[i].getValue();
        }
  }
  return 0;
}

unsigned long SymbolsTable::getSectionOfSymbol(string name){
  for(int i=0 ; i<this->symbolsTable.size(); i++){
        if(this->symbolsTable[i].getSymbolName() == name && this->symbolsTable[i].getType()=="NOTYPE"){
          return stoul(this->symbolsTable[i].getSectionIndexString());
        }
  }
  return 0;
}

bool SymbolsTable::isSectionGlobal(string name){
  for(int i=0 ; i<this->symbolsTable.size(); i++){
        if(this->symbolsTable[i].getSymbolName() == name && this->symbolsTable[i].getType()=="SECTION" && this->symbolsTable[i].getBind()=="GLOBAL"){
          return true;
        }
  }
  return false;
}

bool SymbolsTable::isSection(string name){
  for(int i=0 ; i<this->symbolsTable.size(); i++){
        if(this->symbolsTable[i].getSymbolName() == name && this->symbolsTable[i].getType()=="SECTION"){
          return true;
        }
  }
  return false;
}

unsigned long SymbolsTable::getIdOfSection(string name){
  for(int i=0 ; i<this->symbolsTable.size(); i++){
        if(this->symbolsTable[i].getSymbolName() == name && this->symbolsTable[i].getType()=="SECTION"){
          return stoul(this->symbolsTable[i].getSectionIndexString());
        }
  }
  return 0;
}

bool SymbolsTable::doesSectionExists(string name){
  for(SymbolsTableEntry entry:this->symbolsTable){
    if(entry.getSymbolName() == name && entry.getType()=="SECTION") return true;
  }
  return false;
}

unsigned long SymbolsTable::getSectionSize(string name){
    for(int i=0 ; i<this->symbolsTable.size(); i++){
        if(this->symbolsTable[i].getSymbolName() == name && this->symbolsTable[i].getType()=="SECTION"){
          return this->symbolsTable[i].getSize();
        }
      }
  return 0;
}

vector<SymbolsTableEntry> SymbolsTable::getExternSymbols() {
		vector<SymbolsTableEntry> externSyms;
		for (SymbolsTableEntry& entry : this->symbolsTable) {
			if (entry.getSectionIndexString() == "UNDEFINED" && entry.getBind()=="GLOBAL")
				externSyms.push_back(entry);
		}
		return externSyms;
}


 vector<SymbolsTableEntry> SymbolsTable::getGlobalSymbols() {
		vector<SymbolsTableEntry> globalSyms;
		for (SymbolsTableEntry& entry : this->symbolsTable) {
			if (entry.getSectionIndexString() != "UNDEFINED" && entry.getBind()=="GLOBAL")
				globalSyms.push_back(entry);
		}
		return globalSyms;
}

void SymbolsTable::setValueForSection(string name, unsigned long value){
  for(int i=0 ; i<this->symbolsTable.size(); i++){
        if(this->symbolsTable[i].getSymbolName() == name && this->symbolsTable[i].getType()=="SECTION"){
          this->symbolsTable[i].setValue(value);
        }
  }
}

unsigned long SymbolsTable::getSectionValue(unsigned long id){
 for(int i=0 ; i<this->symbolsTable.size(); i++){
        if(stoul(this->symbolsTable[i].getSectionIndexString()) == id && this->symbolsTable[i].getType()=="SECTION"){
          return this->symbolsTable[i].getValue();
        }
  }
  return 0;
}