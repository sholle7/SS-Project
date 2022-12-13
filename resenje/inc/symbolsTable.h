#ifndef _symbolsTable_h_
#define _symbolsTable_h_

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

class SymbolsTableEntry
{
public:
  SymbolsTableEntry(unsigned long value, unsigned long size, string type, string bind, string sectionIndexString, string symbolName, bool isDefined);
  SymbolsTableEntry(unsigned long value, unsigned long size, string type, string bind, string symbolName, bool isDefined);
  

  unsigned long getId(){return this->id;}
  unsigned long getValue(){return this->value;}
  unsigned long getSize(){return this->size;}
  string getType(){return this->type;}
  string getBind(){return this->bind;}
  string getSectionIndexString(){return this->sectionIndexString;}
  string getSymbolName(){return this->symbolName;}
  bool getIsDefined(){return this->isDefined;}

  void setValue(unsigned long value){this->value = value;}
  void setSize(unsigned long size){this->size = size;}
  void setType(string type){this->type = type;}
  void setBind(string bind){this->bind = bind;}
  void setSectionIndexString(string sectionIndexString){this->sectionIndexString = sectionIndexString;}
  void setSymbolName(string symbolName){this->symbolName = symbolName;}
  void setIsDefined(bool isDefined){this->isDefined=isDefined;}
  void setId(unsigned long id ){this->id = id;}

private:
  static unsigned long ID;
  static unsigned long SECTION_ID;

  unsigned long id;
  unsigned long value;
  unsigned long size;
  string type;
  string bind;
  string sectionIndexString;
  string symbolName;
  bool isDefined;
};

class SymbolsTable
{
public:
  SymbolsTable();
  void addEmptySection();
  void addToSymbolsTable(SymbolsTableEntry entry);
  void printSymbolsTable(ofstream& outputFile);
  
  vector<SymbolsTableEntry> getSymbolsTable(){return this->symbolsTable;}
  vector<SymbolsTableEntry>& getSymbolsTableRefference(){return this->symbolsTable;}

  bool isSymbolDefined(string name);
  bool doesSymbolExist(string name);
  bool isSectionGlobal(string name);
  bool isSection(string name);

  void setBindingForSymbol(string name, string bind);
  void setIsDefinedForSymbol(string name, bool isDefined);
  void setValueForSymbol(string name, unsigned long value);
  void setSectionIndexForsymbol(string name, string sectionIndex);

  string getBindingfOfSymbol(string name);
  unsigned long getIdOfSymbol(string name);
  unsigned long getValueOfSymbol(string name);
  unsigned long getSectionOfSymbol(string name);
  unsigned long getIdOfSection(string name);
  void setSectionSize(unsigned long value, string sectionIndex);

  bool doesSectionExists(string name);  

  string getSectionName(long id);

  unsigned long getSectionSize(string name);
  vector<SymbolsTableEntry> getExternSymbols();
  vector<SymbolsTableEntry> getGlobalSymbols();

  unsigned long getSectionValue(unsigned long id);
  void setValueForSection(string name, unsigned long value);

private:
  vector<SymbolsTableEntry> symbolsTable;
};

#endif