#include "../inc/linker.h"

Linker::Linker(vector<string> allInputFilesNames, string outputFile)
{

  this->allInputFilesNames = allInputFilesNames;
  this->outputFile.open(outputFile, fstream::out | fstream::trunc);
}

Linker::~Linker()
{
  this->outputFile.close();
}

void Linker::linkAllInputFiles()
{

  //get symbols table, relocations table and binary codes for every file and store it
  readInputFiles();

  //merge all sections from all files into one symbol table
  mergeSectionsIntoSymbolsTable();

  //check if all extern symbols are defined somewhere in other files
  checkExternSymbolsDefinitionAndMultipleSymbolDefinitions();

  //give every section his starting address 
  resolveStartingAddressesForEverySection();

  //divide binary codes from all files by sections
  mergeBinaryCodesBySections();

  //resolve all symbols in all relocations tables
  resolveRelocations();

  //write to output file binary codes of all sections combined in format: address,values (because -hex option need to be included)
  writeToOutputFile();

}

void Linker::readInputFiles()
{

  for (int i = 0; i < allInputFilesNames.size(); i++)
  {

    int j = 0;

    ifstream inputFile(allInputFilesNames[i], fstream::in);

    if (!inputFile){
      throw LinkerException("Can not open input file");
    }

    FileData filedata;
    readSymbolsTable(inputFile, filedata);
  
    while (j < this->currentNumberOfSections)
    {
      readRelocationsTable(inputFile, filedata);     
      readBinaryCode(inputFile, filedata); 
      j++;
    }


    this->allFilesData.push_back(filedata);
    inputFile.close();
  }

  // printAllFileDatasToOutputFile();
}

void Linker::readSymbolsTable(ifstream &inputFile, FileData &fileData)
{
  string line;
  SymbolsTable s;
  this->currentNumberOfSections = 0;
  for (int i = 0; i < 4; i++)
    getline(inputFile, line);
  int currentId = 0;

  while (getline(inputFile, line))
  {
    if (line.empty())
      break;
    else
    {
      vector<string> lineValues;
      tokenize(line, '|', lineValues);
      for (int i = 0; i < lineValues.size(); i++)
      {

        lineValues[i].erase(std::remove_if(lineValues[i].begin(), lineValues[i].end(), ::isspace), lineValues[i].end());
      }
      bool flag = false;
      if (lineValues[5] != "UNDEFINED")
        flag = true;
      if (lineValues[3] == "SECTION")
        this->currentNumberOfSections++;
      SymbolsTableEntry entry = SymbolsTableEntry(stoul(lineValues[1]), stoul(lineValues[2]), lineValues[3], lineValues[4], lineValues[5], lineValues[6], flag);
      entry.setId(currentId++);
      s.addToSymbolsTable(entry);
    }
  }

  fileData.setSymbolsTable(s);
}

void Linker::readRelocationsTable(ifstream &inputFile, FileData &fileData)
{
  string line;
  getline(inputFile, line);

  RelocationsTable r;

  getline(inputFile, line);
  string sectionName = line;
  sectionName.erase(0, 6);
  
  getline(inputFile, line);
  getline(inputFile, line);
  getline(inputFile, line);

 this->currentSectionName = sectionName;
  unsigned long sectionId = fileData.getSymbolsTable().getIdOfSection(sectionName);

  while (getline(inputFile, line))
  {
    if (line.empty())
      break;

    else
    {
      vector<string> lineValues;
      tokenize(line, '|', lineValues);
      for (int i = 0; i < lineValues.size(); i++)
      {
        lineValues[i].erase(std::remove_if(lineValues[i].begin(), lineValues[i].end(), ::isspace), lineValues[i].end());
      }

      vector<string> idName;
      tokenize(lineValues[2], '(', idName);
      int position = idName[1].find(')');
      if (position != string::npos)
      {
        idName[1] = idName[1].substr(0, position);
      }
      RelocationsTableEntry entry = RelocationsTableEntry(sectionId, stol(lineValues[0]), lineValues[1], stoul(idName[0]), idName[1], stol(lineValues[3]));
      bool dirFlag;
      if(lineValues[4] == "dir")dirFlag = true;
      else dirFlag = false;

      entry.setIsUsedInDirective(dirFlag);
      r.addToRelocationsTable(entry);
    }
  }
  fileData.addRelocationsTable(r);
}

void Linker::readBinaryCode(ifstream &inputFile, FileData &fileData)
{
  string line;
  string binCode = "";
  for (int i = 0; i < 5; i++)
    getline(inputFile, line);

  while (getline(inputFile, line))
  {
    if (line.empty())
      break;

    else
    {

      vector<string> lineValues;

      tokenize(line, '|', lineValues);
      lineValues[1].erase(std::remove_if(lineValues[1].begin(), lineValues[1].end(), ::isspace), lineValues[1].end());

      binCode += lineValues[1];
    }
  }
  fileData.addBinaryCode(this->currentSectionName , binCode);
}

void Linker::tokenize(string &str, char delim, vector<string> &out)
{
  size_t start;
  size_t end = 0;

  while ((start = str.find_first_not_of(delim, end)) != string::npos)
  {
    end = str.find(delim, start);
    out.push_back(str.substr(start, end - start));
  }
}

void Linker::printAllFileDatasToOutputFile()
{
  // you can change string directly, like this - strs[0] [0]= 'a';

  for (int i = 0; i < allFilesData.size(); i++)
  {
    this->allFilesData[i].getSymbolsTable().printSymbolsTable(this->outputFile);
    this->outputFile << endl;

    // i - relocation table section have i+1 index
    vector<RelocationsTable> rels = this->allFilesData[i].getRelocationsTables();
    
    this->allFilesData[i].printToOutputFile(this->outputFile);

    for (int j = 0; j < rels.size(); j++)
    {
      rels[j].printRelocationsTable(outputFile, j + 1);
      outputFile << endl;
      outputFile << endl;
      outputFile << endl;
    }
  }
}

void Linker::mergeSectionsIntoSymbolsTable()
{

  int id = 1;

  for (int j=0; j<this->allFilesData.size(); j++) {
    SymbolsTable symTab = this->allFilesData[j].getSymbolsTable();
    SymbolsTable& symTabRefference = this->allFilesData[j].getSymbolsTableRefference();

    vector<SymbolsTableEntry> table = symTab.getSymbolsTable();
    vector<SymbolsTableEntry>& tableRefference = this->allFilesData[j].getSymbolsTableRefference().getSymbolsTableRefference();

    vector<SymbolsTableEntry> sections;

    for (SymbolsTableEntry entry : table) {
      if (entry.getType() == "SECTION" && entry.getSectionIndexString()!="UNDEFINED"){
        sections.push_back(entry);
      }
    }

    for (SymbolsTableEntry section : sections) {
      if(section.getSectionIndexString()=="UNDEFINED")continue;
      if (this->allSections.doesSectionExists(section.getSymbolName())){
        unsigned long currentSize = section.getSize();

        for (int i = 0; i < this->allSections.getSymbolsTable().size(); i++){
          
          if (this->allSections.getSymbolsTableRefference().at(i).getSymbolName() == section.getSymbolName()) {
            this->allFilesData[j].addends[section.getSymbolName()] = this->allSections.getSymbolsTableRefference().at(i).getSize();
            this->allSections.getSymbolsTableRefference().at(i).setSize(this->allSections.getSymbolsTableRefference().at(i).getSize() + currentSize);
          }
        }
      }
      else
      {
        section.setId(id);
        section.setSectionIndexString(to_string(id));
        id++;
        this->allSections.addToSymbolsTable(section);
        this->allFilesData[j].addends[section.getSymbolName()] = 0;
        //this->allSectionsStartingAddresses[section.getSymbolName()] = DEFAULT_START_ADDRESS;
        SectionInfo sectionInfo;
        sectionInfo.setAddress(DEFAULT_SECTION_START_ADDRESS);
        sectionInfo.setSectionName(section.getSymbolName());
        this->allSectionsInfo.push_back(sectionInfo);
      }
    }
  }
}

void Linker::checkExternSymbolsDefinitionAndMultipleSymbolDefinitions() {
  vector<SymbolsTableEntry> allExternSymbols;
  vector<SymbolsTableEntry> allGlobalSymbols;

  for (int i = 0; i < this->allFilesData.size(); i++) {
    vector<SymbolsTableEntry> externSymbols = this->allFilesData[i].getSymbolsTable().getExternSymbols();
    for (SymbolsTableEntry e : externSymbols)
      allExternSymbols.push_back(e);

    vector<SymbolsTableEntry> globalSymbols = this->allFilesData[i].getSymbolsTable().getGlobalSymbols();
    for (SymbolsTableEntry e : globalSymbols)
      allGlobalSymbols.push_back(e);
  }

  this->allGlobalSymbols = allGlobalSymbols;

  for (int i = 0; i < allExternSymbols.size(); i++) {
    bool isExternSymbolDefinedFlag = false;
    for (int j = 0; j < allGlobalSymbols.size(); j++)
      if (allExternSymbols[i].getSymbolName() == allGlobalSymbols[j].getSymbolName())
        isExternSymbolDefinedFlag = true;
    if (isExternSymbolDefinedFlag == false)
      throw LinkerException("Symbol " + allExternSymbols[i].getSymbolName() + " is not defined.");
  }

  for (int i = 0; i < allGlobalSymbols.size(); i++)
  {
    for (int j = 0; j < allGlobalSymbols.size(); j++)
    {
      if (i != j)
      {
        if (allGlobalSymbols[i].getSymbolName() == allGlobalSymbols[j].getSymbolName())
          throw LinkerException("Multiple symbol definition for symbol " + allGlobalSymbols[i].getSymbolName());
      }
    }
  }
}

void Linker::mergeBinaryCodesBySections(){
  for(int i=0; i<allFilesData.size(); i++){
    for(int j=0; j<allSections.getSymbolsTableRefference().size(); j++){
      if(allFilesData[i].doesFileHaveSection(this->allSections.getSymbolsTableRefference().at(j).getSymbolName()))
      // this->allBytesBySections[this->allSections.getSymbolsTableRefference().at(j).getSymbolName()].push_back(allFilesData[i].getBinaryCodesOFSection(this->allSections.getSymbolsTableRefference().at(j).getSymbolName()));  
      this->allBytesBySections[this->allSections.getSymbolsTableRefference().at(j).getSymbolName()] += (allFilesData[i].getBinaryCodesOfSection(this->allSections.getSymbolsTableRefference().at(j).getSymbolName())); 
    }
  }
}

void Linker::resolveStartingAddressesForEverySection(){
  unsigned long startingAddress = 0;

  for(auto& section: this->allSectionsInfo){

    if(section.getAddress() == DEFAULT_SECTION_START_ADDRESS){
        section.setAddress(startingAddress);
        startingAddress += this->allSections.getSectionSize(section.getSectionName());
    }
    this->allSections.setValueForSection(section.getSectionName(), section.getAddress());
  }
}

void Linker::resolveRelocations()
{
  
  for(int i=0; i<this->allFilesData.size(); i++){
    
    vector<RelocationsTable> currentRelocationsTables = this->allFilesData[i].getRelocationsTables();
    SymbolsTable currentSymbolsTable = this->allFilesData[i].getSymbolsTable();
    
    for(int j=0; j<currentRelocationsTables.size(); j++){

      vector<RelocationsTableEntry>&  currentRelocationsTableEntries = currentRelocationsTables[j].getRelocationsTableRefference();

      for(int k=0; k<currentRelocationsTableEntries.size(); k++){
        unsigned long sectionId = getIdForSymbol(currentRelocationsTableEntries[k].getSymbolName());
        unsigned long currentSectionIdForRelocationsTable = currentRelocationsTableEntries[k].getSectionId();

        if(isLocalSymbol(currentRelocationsTableEntries[k].getSymbolName(), i)){

          if(currentRelocationsTableEntries[k].getType()==RELOCATION_PCRELATIVE){
                string symbolName = currentRelocationsTableEntries[k].getSymbolName();
                
                int index = i;

                string sectionName = this->allFilesData[index].getSymbolsTable().getSectionName(sectionId);
                SectionInfo sectionInfo;
                getSectionInfoOfName(sectionName, sectionInfo);
                
                string sectionNameForRelocationsTable = this->allFilesData[i].getSymbolsTable().getSectionName(currentSectionIdForRelocationsTable);
                SectionInfo sectionInfoForRelocationsTable;
                getSectionInfoOfName(sectionNameForRelocationsTable, sectionInfoForRelocationsTable);

                int startingAddress = sectionInfo.getAddress();
                long addendFromSection = this->allFilesData[index].addends[sectionName];
                long addendFromCurrentSection = this->allFilesData[i].addends[sectionNameForRelocationsTable];
                long offset = currentRelocationsTableEntries[k].getOffset();

                int idSec = currentRelocationsTableEntries[k].getSectionId();
                string relocationsSectionName = currentSymbolsTable.getSectionName(idSec);

                int finalLocationAddress = offset + addendFromCurrentSection;
                int value = getLocalSymbolValueToEdit(symbolName, i);
              
                int finalValue = -(offset + addendFromCurrentSection + 2 + sectionInfoForRelocationsTable.getAddress()) + (startingAddress +value + addendFromSection);
                editBinaryCode(finalLocationAddress, finalValue, relocationsSectionName, currentRelocationsTableEntries[k].getIsUsedInDirective());
              
          }
          
          if(currentRelocationsTableEntries[k].getType()==RELOCATION_ABSOLUTE){
                string symbolName = currentRelocationsTableEntries[k].getSymbolName();
                
                int index = i;
                
                string sectionName = this->allFilesData[index].getSymbolsTable().getSectionName(sectionId);
                SectionInfo sectionInfo;
                getSectionInfoOfName(sectionName, sectionInfo);

                string sectionNameForRelocationsTable = this->allFilesData[i].getSymbolsTable().getSectionName(currentSectionIdForRelocationsTable);
                SectionInfo sectionInfoForRelocationsTable;
                getSectionInfoOfName(sectionNameForRelocationsTable, sectionInfoForRelocationsTable);

                int startingAddress = sectionInfo.getAddress();
                long addendFromSection = this->allFilesData[index].addends[sectionName];
                long addendFromCurrentSection = this->allFilesData[i].addends[sectionNameForRelocationsTable];
                long offset = currentRelocationsTableEntries[k].getOffset();

                int idSec = currentRelocationsTableEntries[k].getSectionId();
                string relocationsSectionName = currentSymbolsTable.getSectionName(idSec);

                int finalLocationAddress = offset + addendFromCurrentSection;
                int value = getLocalSymbolValueToEdit(symbolName, i);
                int finalValue = value + addendFromSection + startingAddress;

                editBinaryCode(finalLocationAddress, finalValue, relocationsSectionName, currentRelocationsTableEntries[k].getIsUsedInDirective());
         }

        }

        else{

          if(currentRelocationsTableEntries[k].getType()==RELOCATION_PCRELATIVE){
           
            string symbolName = currentRelocationsTableEntries[k].getSymbolName();
            int index = getIndexOfFileWhereSymbolIsDefined(symbolName);

            string sectionName = this->allFilesData[index].getSymbolsTable().getSectionName(sectionId);
            SectionInfo sectionInfo;
            getSectionInfoOfName(sectionName, sectionInfo);
        
            string sectionNameForRelocationsTable = this->allFilesData[i].getSymbolsTable().getSectionName(currentSectionIdForRelocationsTable);
            SectionInfo sectionInfoForRelocationsTable;
            getSectionInfoOfName(sectionNameForRelocationsTable, sectionInfoForRelocationsTable);
            
            int startingAddress = sectionInfo.getAddress();
            long addendFromSection = this->allFilesData[index].addends[sectionName];
            long addendFromCurrentSection = this->allFilesData[i].addends[sectionNameForRelocationsTable];
            long offset = currentRelocationsTableEntries[k].getOffset();

            int idSec = currentRelocationsTableEntries[k].getSectionId();
            string relocationsSectionName = currentSymbolsTable.getSectionName(idSec);

            int finalLocationAddress = offset + addendFromCurrentSection;
            int value = getValueToEdit(symbolName);
        
            int finalValue = -(offset + addendFromCurrentSection + 2 + sectionInfoForRelocationsTable.getAddress()) + (startingAddress +value + addendFromSection);
            editBinaryCode(finalLocationAddress, finalValue, relocationsSectionName, currentRelocationsTableEntries[k].getIsUsedInDirective());
          }

          if(currentRelocationsTableEntries[k].getType()==RELOCATION_ABSOLUTE){
              string symbolName = currentRelocationsTableEntries[k].getSymbolName();
              int index = getIndexOfFileWhereSymbolIsDefined(symbolName);
             
              string sectionName = this->allFilesData[index].getSymbolsTable().getSectionName(sectionId);
              SectionInfo sectionInfo;
              getSectionInfoOfName(sectionName, sectionInfo);

              string sectionNameForRelocationsTable = this->allFilesData[i].getSymbolsTable().getSectionName(currentSectionIdForRelocationsTable);
              SectionInfo sectionInfoForRelocationsTable;
              getSectionInfoOfName(sectionNameForRelocationsTable, sectionInfoForRelocationsTable);

              int startingAddress = sectionInfo.getAddress();
              long addendFromSection = this->allFilesData[index].addends[sectionName];
              long addendFromCurrentSection = this->allFilesData[i].addends[sectionNameForRelocationsTable];
              long offset = currentRelocationsTableEntries[k].getOffset();

              int idSec = currentRelocationsTableEntries[k].getSectionId();
              string relocationsSectionName = currentSymbolsTable.getSectionName(idSec);

              int finalLocationAddress = offset + addendFromCurrentSection;
              int value = getValueToEdit(symbolName);
              int finalValue = value + addendFromSection + startingAddress;

              editBinaryCode(finalLocationAddress, finalValue, relocationsSectionName, currentRelocationsTableEntries[k].getIsUsedInDirective());
          }
        }
      }
    }
  }
}

void Linker::writeToOutputFile(){
  //iterate through allSectionsInfo, get the section name, find byte codes of that section in allBytesBySections and write it in output file
  
  int currentByte = 0;
  int currentOffset = 0;
  
  for(int i=0; i<this->allSectionsInfo.size(); i++){
    string sectionName = this->allSectionsInfo[i].getSectionName();
    string allBytesOfSection = this->allBytesBySections[sectionName];

    for(int j=0; j<allBytesOfSection.size(); j+=2){

      if(currentByte == 0){
        outputFile<<right<<setw(8)<<hex<<setfill('0')<<currentOffset<< ": ";
      }

      this->outputFile << allBytesOfSection[j]; //first hexadecimal number
      this->outputFile << allBytesOfSection[j+1];//second hexadecimal number

      if (++currentByte == 8){
        currentByte = 0;
        currentOffset +=8;
        this->outputFile<<endl;
      }

      else this->outputFile<<" ";   

    }
    
  }
}

void Linker::getSectionInfoOfName(string name, SectionInfo& sectInfo){
  for(int i=0; i<this->allSectionsInfo.size(); i++){
    if(this->allSectionsInfo[i].getSectionName() == name) sectInfo = this->allSectionsInfo[i];
  }
}

void Linker::editBinaryCode(long addendFromSection, int value, string sectionName, bool isLH){
  
  string& bytes = this->allBytesBySections[sectionName];
  
  int positionStart = 2*addendFromSection;

  stringstream ss;

  if(isLH){

  //L H 
  //directives
  
    ss<< hex <<((value >>4 ) & 0x0F); 
    string stringValue = ss.str();
    bytes[positionStart] = stringValue[0];
    ss.str("");

    ss<< hex <<((value) & 0x0F); 
    stringValue = ss.str();
    bytes[positionStart+1] = stringValue[0];
    ss.str("");

    ss<< hex <<((value  >> 12) & 0x0F); 
    stringValue = ss.str();
    bytes[positionStart+2] = stringValue[0];
    ss.str("");
    
    ss<< hex <<((value  >> 8) & 0x0F); 
    stringValue = ss.str();
    bytes[positionStart+3] = stringValue[0];
    ss.str("");
  //END
  }

  else{

  //H L
  //instructions

    ss<< hex <<((value >> 12) & 0x0F); 
    string stringValue = ss.str();
    bytes[positionStart] = stringValue[0];
    ss.str("");

    ss<< hex <<((value >> 8) & 0x0F); 
    stringValue = ss.str();
    bytes[positionStart+1] = stringValue[0];
    ss.str("");

    ss<< hex <<((value  >> 4) & 0x0F); 
    stringValue = ss.str();
    bytes[positionStart+2] = stringValue[0];
    ss.str("");
    
    ss<< hex <<((value) & 0x0F); 
    stringValue = ss.str();
    bytes[positionStart+3] = stringValue[0];
    ss.str("");
  //END
  }

}

int Linker::getValueToEdit(string symbolName){
  for(int i=0; i<this->allGlobalSymbols.size(); i++){
    if(this->allGlobalSymbols[i].getSymbolName() == symbolName){
      return this->allGlobalSymbols[i].getValue();
    }
  }
  return 0;
}

int Linker::getIndexOfFileWhereSymbolIsDefined(string symbolName){
  
  for(int i=0; i<this->allFilesData.size(); i++){
    SymbolsTable symTab = this->allFilesData[i].getSymbolsTable();
    if(symTab.isSymbolDefined(symbolName) && symTab.getBindingfOfSymbol(symbolName)=="GLOBAL"){
      return i;
    }
  }
  return 0;
}

unsigned long Linker::getIdForSymbol(string name){
  for(int i=0; i<this->allFilesData.size(); i++){
    vector<SymbolsTableEntry> entries = allFilesData[i].getSymbolsTable().getSymbolsTable();
    for(int j=0; j<entries.size(); j++){
      if(entries[j].getSymbolName() == name && entries[j].getIsDefined()==true){
        return stoul(entries[j].getSectionIndexString());
      }
    }
  }
  return 0;
}

bool Linker::isLocalSymbol(string name, int index){
  vector<SymbolsTableEntry> entries = this->allFilesData[index].getSymbolsTable().getSymbolsTable();
  for(int j=0; j<entries.size(); j++){
      if(entries[j].getSymbolName() == name && entries[j].getBind()=="LOCAL"){
        return true;
      }
  }
  return false;
}

int Linker::getLocalSymbolValueToEdit(string symbolName, int file){
  vector<SymbolsTableEntry> entries = this->allFilesData[file].getSymbolsTable().getSymbolsTable();
  for(int i=0; i<entries.size(); i++){
    if(entries[i].getSymbolName() == symbolName){
      return entries[i].getValue();
    }
  }
  return 0;
}