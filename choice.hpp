#ifndef CHOICE_H
#define CHOICE_H

#include <map>
#include <string>

class Choice {
 private:
  int toPageNum;
  std::string text;
  std::string varName;
  int varValue;

 public:
  Choice(int toPageNum, std::string text);
  int getToPageNum();
  std::string getText();
  bool setVarName(std::string name);
  bool setVarValue(int value);
  std::string getVarName();
  int getVarValue();
  bool needVar();
  bool meetVar(std::map<std::string, int> currentVars);
};

#endif  //CHOICE_H
