#ifndef _parser_h_
#define _parser_h_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <algorithm>
#include <iterator>

#include "constants.h"

using namespace std;

class Parser{

  public:
  vector<string> removeExtraWhiteSpaces(vector<string> allLines);
  vector<string> parseTextFile(ifstream& inputFileForParsing);
  vector<string> deleteAllComments(vector<string> allLines);
  vector<string> deleteAfterEndDirective(vector<string> allLines);
  string removeLabelFromLine(string line);
};

#endif
