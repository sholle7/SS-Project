#include "../inc/parser.h"

vector<string> Parser::parseTextFile(ifstream& inputFileForParsing){
  vector<string> lines;
  string line;

  while (!inputFileForParsing.eof()) {
    getline(inputFileForParsing, line);
    lines.push_back(line);
  }
  return lines;
}

vector<string> Parser::deleteAllComments(vector<string> allLines){
  vector<string> returnLines;
   for (string line: allLines){
       int position = line.find(COMMENT_SYMBOL);
       if(position != string::npos){
          line = line.substr(0,position);
       }
       returnLines.push_back(line);
   }

   return returnLines;
}

vector<string> Parser::deleteAfterEndDirective(vector<string> allLines){
  vector<string> returnLines;
  
  for (int i=0; i<allLines.size(); i++){
      int position = allLines[i].find(DIRECTIVE_END);
      if(position == string::npos){
        returnLines.push_back(allLines[i]);
      }
      else{
        returnLines.push_back(allLines[i]);
        break;
      }
      
  }

  return returnLines;
}

vector<string> Parser::removeExtraWhiteSpaces(vector<string> allLines){
  vector<string> returnLines;
  string line;
  for(int i=0; i<allLines.size(); i++){
    line = "";
    unique_copy (allLines[i].begin(), allLines[i].end(), back_insert_iterator<string>(line), [](char a,char b){ return isspace(a) && isspace(b);});  
    returnLines.push_back(line);
  }

  return returnLines;
}

string Parser::removeLabelFromLine(string line){
  string currentLine = line;
  int position = line.find(LABEL_SYMBOL);
  if(position != string::npos){
    while(true){
      if(currentLine[position+1] ==' ') position++;
      else{
        break;
      }
    }
    currentLine = currentLine.substr(position+1);
  }
  return currentLine;
}