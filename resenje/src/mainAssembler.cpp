#include <iostream>
#include "../inc/assemblerExceptions.h"
#include <string.h>
#include <stdio.h>
#include "../inc/assembler.h"

using namespace std;

int main(int argc, char** argv) {
  
  try{

    if (argc < 4){
        throw AssemblerException("Invalid parameters");
    }

    string options = argv[1];
    string outputFile = argv[2];
    string inputFile = argv[3];
    
    if(options=="-o"){
      Assembler* assembler = new Assembler(inputFile, outputFile);

      assembler->assemble();
      
      delete assembler;
    }
    else{

      throw AssemblerException("Wrong argument options");
      
    }

  }

  catch(exception& e){
    cout<<e.what() <<endl;
    return -1;
  }


  return 0;
}