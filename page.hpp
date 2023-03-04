#ifndef PAGE_H
#define PAGE_H

#include <map>
#include <string>
#include <vector>

#include "choice.hpp"

class Page {
 private:
  char type;
  int pageNum;
  std::string filepath;
  std::string text;
  std::vector<Choice> choices;
  std::map<std::string, int> vars;

  std::string loadText();

 public:
  Page(char type, int pageNum, std::string filepath);
  char getType();
  int getPageNum();
  std::map<std::string, int> getVars();
  bool addChoice(Choice c);
  bool addVar(std::string name, int value);
  std::vector<Choice> getChoices();
  Choice getChoice(int index);
  void display(std::map<std::string, int> currentVars);
  int choiceNum();
};

#endif  //PAGE_H
