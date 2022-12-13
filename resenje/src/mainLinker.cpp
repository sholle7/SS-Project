#include <iostream>
#include "../inc/linkerExceptions.h"
#include "vector"
#include "../inc/linker.h"

using namespace std;

int main(int argc, char** argv) {
  
    bool isHex = false;
    bool isOutput = false;
    string argument;
    string outputFile;
    vector<string> allInputFilesNames;
  
  try{

    for (int i = 1; i < argc; i++){

      argument = argv[i];

      if (argument == "-hex") { 
        isHex = true;
        continue;
	  	}

      else if (argument == "-o") { 
        i++;
        outputFile = argv[i];
        isOutput = true;
	  	}

      else{
        allInputFilesNames.push_back(argv[i]);
      }
  
    }

    if(isHex == false) throw LinkerException("Invalid parameters - you need to include '-hex' option");
    if(isOutput == false) throw LinkerException("Invalid parameters - you need to specifiy output file with '-o' option");
    if(allInputFilesNames.size() == 0) throw LinkerException("Invalid parameters - no input files included");

    Linker* linker = new Linker(allInputFilesNames, outputFile);

    linker->linkAllInputFiles();

    delete linker;

  } catch(exception& e){
    cout<<e.what() <<endl;
    return -1;
  }

  return 0;
}