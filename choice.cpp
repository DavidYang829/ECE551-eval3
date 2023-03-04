#include "choice.hpp"

Choice::Choice(int toPageNum, std::string text) {
  this->toPageNum = toPageNum;
  this->text = text;
}

int Choice::getToPageNum() {
  return toPageNum;
}

std::string Choice::getText() {
  return text;
}

bool Choice::setVarName(std::string name) {
  varName = name;
  return true;
}

std::string Choice::getVarName() {
  return varName;
}

bool Choice::setVarValue(int value) {
  varValue = value;
  return true;
}

int Choice::getVarValue() {
  return varValue;
}

bool Choice::needVar() {
  return !getVarName().empty();
}

bool Choice::meetVar(std::map<std::string, int> currentVars) {
  if (currentVars[varName] == varValue)
    return true;
  return false;
}
