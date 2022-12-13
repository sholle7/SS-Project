#ifndef _lexer_h_
#define _lexer_h_

#include <iostream>
#include <vector>
#include <regex>
#include "ctype.h"
#include "constants.h"

using namespace std;

class Lexer{
  
public:
  vector<string> getAllTokensFromLine(string line);
};


#endif