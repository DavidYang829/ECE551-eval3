#include "page.hpp"

#include <fstream>
#include <iostream>

Page::Page(char type, int pageNum, std::string filepath) {
  this->type = type;
  this->pageNum = pageNum;
  this->filepath = filepath;
}

char Page::getType() {
  return type;
}

int Page::getPageNum() {
  return pageNum;
}

/**
 * add choice to list
 */
bool Page::addChoice(Choice c) {
  choices.push_back(c);
  return true;
}

/**
 * load page text from file
 * store loaded text on the first call, return it in subsequent calls
 */
std::string Page::loadText() {
  // if text is empty, load and store
  if (!text.length()) {
    std::ifstream ifs(filepath, std::ios::in);
    if (!ifs.is_open()) {
      std::cerr << filepath << "read page failed." << std::endl;
      exit(EXIT_FAILURE);
    }

    // load all content in file
    std::istreambuf_iterator<char> begin(ifs);
    std::istreambuf_iterator<char> end;
    text = std::string(begin, end);
  }

  return text;
}

std::vector<Choice> Page::getChoices() {
  return choices;
}

Choice Page::getChoice(int index) {
  return choices[index];
}

/**
 * display page
 */
void Page::display(std::map<std::string, int> currentVars) {
  std::cout << loadText() << std::endl;
  if (getType() == 'N') {
    // Normal
    std::cout << "What would you like to do?" << std::endl;
    std::cout << std::endl;
    for (int i = 0; i < getChoices().size(); ++i) {
      Choice choice = getChoice(i);
      // if this page needs some condition
      if (choice.needVar()) {
        if (choice.meetVar(currentVars)) {
          std::cout << " " << i + 1 << ". " << choice.getText() << std::endl;
        }
        else {
          std::cout << " " << i + 1 << ". <UNAVAILABLE>" << std::endl;
        }
      }
      else {
        std::cout << " " << i + 1 << ". " << choice.getText() << std::endl;
      }
    }
  }
  else if (getType() == 'W') {
    // Win
    std::cout << "Congratulations! You have won. Hooray!" << std::endl;
  }
  else if (getType() == 'L') {
    // Lose
    std::cout << "Sorry, you have lost. Better luck next time!" << std::endl;
  }
}

int Page::choiceNum() {
  return choices.size();
}

bool Page::addVar(std::string name, int value) {
  vars[name] = value;
  return true;
}

std::map<std::string, int> Page::getVars() {
  return vars;
}
