#include <iostream>
#include "../inc/emulatorExceptions.h"
#include "../inc/emulator.h"

using namespace std;

int main(int argc, char** argv) {

  try{

    if (argc != 2) throw EmulatorException("Invalid parameters - you need to pass only one file to emulator");

    string fileName = argv[1];
    Emulator emulator (fileName);

    emulator.emulate();


  } catch(exception& e){
    cout<<e.what() <<endl;
    return -1;
  }

  return 0;
}