#include "../inc/lexer.h"

vector<string> Lexer::getAllTokensFromLine(string line)
{

  vector<string> returnTokens;
  string currentToken = "";
  bool isSquareBracketOpenedFlag = false;
  // replace sp, pc and psw with general purpose registers
  // line = regex_replace(line, SP_REGEX, "%r6");
  // line = regex_replace(line, PC_REGEX, "%r7");
  // line = regex_replace(line, PSW_REGEX, "%r8");
  for (char character : line) {
    
    if ((isSquareBracketOpenedFlag == true) || (character != ' ' && character != ',' && character != ':' && character != '\n' && character != '\r' && character != '\b' && character != '\t' && character != '\f' && character != '\v')) {
      currentToken += character;
    }

    if (character=='['){
        isSquareBracketOpenedFlag = true;
    }

    if (character==']'){
        isSquareBracketOpenedFlag = false;
    }

    if (character == ':') {
      
      if (currentToken != "") {
        currentToken = currentToken + character;
        returnTokens.push_back(currentToken);
      }
      else {
        if (returnTokens.size() > 0) returnTokens[returnTokens.size() - 1] += ":";
      }
      currentToken = "";
    }
    
    if(!((isSquareBracketOpenedFlag == true) || (character != ' ' && character != ',' && character != ':' && character != '\n' && character != '\r' && character != '\b' && character != '\t' && character != '\f' && character != '\v'))){
      if (currentToken == "+" || currentToken == "-" || isSquareBracketOpenedFlag == true) continue;
      if (currentToken != "") returnTokens.push_back(currentToken);
       currentToken = "";
    }

  }

  if (currentToken != "") returnTokens.push_back(currentToken);

  return returnTokens;
}