#ifndef _linker_h_
#define _linker_h_

#include <iostream>
#include <vector>
#include <fstream>
#include "linkerExceptions.h"
#include "symbolsTable.h"
#include "relocationsTable.h"
#include "constants.h"
#include <algorithm>
#include <iterator> 
#include <map>

using namespace std;

class FileData{
  public:

    void setSymbolsTable(SymbolsTable s){this->symbolsTable = s;}
    void addRelocationsTable(RelocationsTable r){ this->relocationsTables.push_back(r);}
    void addBinaryCode(string name, string code){this->binaryCodes[name] = code;}

    SymbolsTable getSymbolsTable(){return this->symbolsTable;}
    SymbolsTable& getSymbolsTableRefference(){return this->symbolsTable;}

    vector<RelocationsTable> getRelocationsTables(){return this->relocationsTables;}
    
    bool doesFileHaveSection(string name){
      auto it = this->binaryCodes.find(name);
      if (it==this->binaryCodes.end()) return false;
      else return true;
    }

    string getBinaryCodesOfSection(string name){
      auto it = this->binaryCodes.find(name);  
      return it->second;
    }

    void printToOutputFile(ofstream& output){
      for (auto i = this->binaryCodes.begin(); i != this->binaryCodes.end(); ++i) {
        output << i->first << ": " << i->second << endl;
      }
    }
   map<string, long> addends;
  private:
    SymbolsTable symbolsTable;
    vector<RelocationsTable> relocationsTables;
    map <string,string> binaryCodes;
   
};


class SectionInfo{

public:
  int getAddress(){return this->address;}
  void setAddress(int address){this->address=address;}
  string getSectionName(){return this->sectionName;}
  void setSectionName(string name){this->sectionName = name;}
  
  
private:
  string sectionName;
  int address;
};



class Linker{

public:

  Linker(vector<string>allInputFilesNames, string outputFile);
  ~Linker();
  void linkAllInputFiles();
  void readInputFiles();
  
  void readSymbolsTable(ifstream& inputFile, FileData& fileData);
  void readRelocationsTable(ifstream& inputFile, FileData& fileData);
  void readBinaryCode(ifstream& inputFile, FileData& fileData);
  void tokenize(string &str, char delim, vector<string> &out);
  void printAllFileDatasToOutputFile(); //helper function to check are intern structures correct

  void mergeSectionsIntoSymbolsTable();
  void checkExternSymbolsDefinitionAndMultipleSymbolDefinitions();
  void resolveStartingAddressesForEverySection();
  void mergeBinaryCodesBySections();
  void resolveRelocations();
  void writeToOutputFile();

  void getSectionInfoOfName(string name, SectionInfo& sectInfo);
  void editBinaryCode(long addendFromSection, int value, string sectionName, bool isLH); //isLH is true when symbol is used in directive

  int getValueToEdit(string symbolName);
  int getIndexOfFileWhereSymbolIsDefined(string symbolName);
  
  unsigned long getIdForSymbol(string name);
  bool isLocalSymbol(string name, int index);
  int getLocalSymbolValueToEdit(string symbolName, int file);
  

private:
  string currentSectionName;
  int currentNumberOfSections;

  ofstream outputFile;

  vector<string> allInputFilesNames;
  vector<SectionInfo> allSectionsInfo;
  vector<FileData> allFilesData;

  vector<SymbolsTableEntry> allGlobalSymbols;

  SymbolsTable allSections;

  
  map <string, string> allBytesBySections;//all byte codes grouped by sections
  
};

#endif