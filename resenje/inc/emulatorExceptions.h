#ifndef _emulatorexceptions_h_
#define _emulatorexceptions_h_

#include <exception>
#include <iostream>
#include <cstring>
#include <sstream>

using namespace std;

class EmulatorException : public exception
{
public:
   EmulatorException(string message) noexcept : exception() {this->message=message; this->line = -1;}
   EmulatorException(string message, unsigned long line) noexcept : exception() {this->message=message; this->line = line;}

   const char *what() const noexcept override
   {

      ostringstream helpErrorMessage;

      helpErrorMessage << "Error: ";

      if (line != -1) helpErrorMessage << " line " << line << ": ";
   
      helpErrorMessage << message;

      string errorMessage = helpErrorMessage.str();

      char *cstr = new char[errorMessage.length() + 1];
      strcpy(cstr, errorMessage.c_str());
      temporary[0] = cstr;

      return cstr;
   }
   ~EmulatorException()
   {
      delete[] temporary[0];
      delete temporary;
   }

private:
   string message;
   unsigned long line;

   char **temporary = new char *[1];
};

#endif