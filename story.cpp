#include "story.hpp"

#include <fstream>
#include <iostream>
#include <regex>
#include <string>

/**
 * add page to list
 */
bool Story::addPage(Page p) {
  pages.push_back(p);
  return true;
}

Story::Story() {
}

/**
 * return the reference of page whose page number is pageNum
 */
Page & Story::getPage(int pageNum) {
  for (auto & page : pages) {
    if (page.getPageNum() == pageNum)
      return page;
  }
  // will not happen after verified
  std::cerr << "Page not exists" << std::endl;
  exit(EXIT_FAILURE);
}

std::vector<Page> Story::getPages() {
  return pages;
}

/**
 * read file from story.txt in given folder, and convert each line to either Page class or Choice class
 */
void Story::load(std::string folder) {
  std::string storyFilepath = folder + "/story.txt";
  std::ifstream ifs(storyFilepath, std::ios::in);

  if (!ifs.is_open()) {
    std::cerr << "read fail." << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string line;
  while (getline(ifs, line)) {
    if (!line.length()) {
      // blank line
      continue;
    }
    else if (line.find('@') != -1) {
      // page line
      int pageNum = strtol(line.substr(0, line.find('@')).c_str(), NULL, 10);
      char type = line.substr(line.find('@') + 1, 1)[0];
      std::string filename = line.substr(line.find(':') + 1);

      std::string filepath = folder + '/' + filename;
      Page page(type, pageNum, filepath);
      addPage(page);
    }
    else if (line.find('$') != -1) {
      // page var
      int pageNum = strtol(line.substr(0, line.find('$')).c_str(), NULL, 10);
      line = line.substr(line.find('$') + 1);

      std::string varName = line.substr(0, line.find('='));
      int varValue = strtol(line.substr(line.find('=') + 1).c_str(), NULL, 10);
      getPage(pageNum).addVar(varName, varValue);
    }
    else if (line.find('[') != -1) {
      // choice var
      int pageNum = strtol(line.substr(0, line.find('[')).c_str(), NULL, 10);
      line = line.substr(line.find('[') + 1);

      std::string varName = line.substr(0, line.find('='));
      line = line.substr(line.find('=') + 1);

      int varValue = strtol(line.substr(0, line.find(']')).c_str(), NULL, 10);
      line = line.substr(line.find(':') + 1);

      int toPageNum = strtol(line.substr(0, line.find(':')).c_str(), NULL, 10);
      std::string text = line.substr(line.find(':') + 1);

      Choice choice(toPageNum, text);
      choice.setVarName(varName);
      choice.setVarValue(varValue);
      getPage(pageNum).addChoice(choice);
    }
    else {
      // choice line
      int pageNum = strtol(line.substr(0, line.find(':')).c_str(), NULL, 10);

      line = line.substr(line.find(':') + 1);
      int toPageNum = strtol(line.substr(0, line.find(':')).c_str(), NULL, 10);
      std::string text = line.substr(line.find(':') + 1);

      Choice choice(toPageNum, text);
      getPage(pageNum).addChoice(choice);
    }
  }
}

/**
 * return the current page
 */
Page Story::getCurrentPage() {
  for (auto page : pages) {
    if (page.getPageNum() == currentPageNum)
      return page;
  }
  // will not happen after verified.
  std::cerr << "Page not exists" << std::endl;
  exit(EXIT_FAILURE);
}

/**
 * tell whether the story meets WIN or LOSE page
 */
bool Story::end() {
  Page currentPage = getCurrentPage();
  if (currentPage.getType() == 'W' || currentPage.getType() == 'L')
    return true;
  else
    return false;
}

/**
 * move to next page
 */
void Story::move(int pageNum) {
  currentPageNum = pageNum;
}

/**
 * play the story
 */
void Story::play() {
  updateVars();
  while (true) {
    Page current = getCurrentPage();
    current.display(vars);
    if (end())
      break;

    int choiceIndex;
    while (true) {
      std::string input;
      char * left;
      std::cin >> input;
      choiceIndex = strtol(input.c_str(), &left, 10);
      if (strlen(left) != 0 || choiceIndex < 1 || choiceIndex > current.choiceNum())
        std::cout << "That is not a valid choice, please try again" << std::endl;
      else if (current.getChoice(choiceIndex - 1).needVar()) {
        if (current.getChoice(choiceIndex - 1).meetVar(vars))
          break;
        else
          std::cout << "That choice is not available at this time, please try again"
                    << std::endl;
      }
      else {
        break;
      }
    }

    int toPageNum = current.getChoice(choiceIndex - 1).getToPageNum();
    move(toPageNum);
    updateVars();
  }
}

/**
 * find cycle-free paths to arrive WIN page
 */
std::vector<std::vector<std::pair<int, int> > > Story::findCycleFreeWinPaths() {
  std::vector<std::vector<std::pair<int, int> > > paths;
  std::vector<std::pair<int, int> > history;
  std::map<int, bool> visited;
  findCycleFreeWinPaths(0, visited, history, paths);
  return paths;
}

/**
 *
 * find cycle-free paths to arrive WIN page
 * use deep-first search and record current path while searching
 */
void Story::findCycleFreeWinPaths(
    int currentPageNum,
    std::map<int, bool> & visited,
    std::vector<std::pair<int, int> > & history,
    std::vector<std::vector<std::pair<int, int> > > & paths) {
  visited[currentPageNum] = true;
  Page currentPage = getPage(currentPageNum);
  if (currentPage.getType() == 'W') {
    // WIN page, add history path to answer

    history.push_back(std::make_pair(currentPageNum, -1));
    std::vector<std::pair<int, int> > copiedHistory(history);
    paths.push_back(copiedHistory);
    history.pop_back();
    return;
  }
  else if (currentPage.getType() == 'L') {
    return;
  }

  for (int i = 0; i < currentPage.getChoices().size(); i++) {
    int choiceNum = i + 1;
    int toPageNum = currentPage.getChoice(i).getToPageNum();
    if (!visited[toPageNum]) {
      // cycle-free
      visited[toPageNum] = true;
      history.push_back(std::make_pair(currentPageNum, choiceNum));
      findCycleFreeWinPaths(toPageNum, visited, history, paths);
      history.pop_back();
      visited[toPageNum] = false;
    }
  }
}

/**
 * update current variables and their value
 */
void Story::updateVars() {
  Page page = getPage(currentPageNum);
  for (auto p : page.getVars()) {
    vars[p.first] = p.second;
  }
}

/**
 * verify the story
 */
bool Story::verify() {
  std::map<int, bool> referenced;
  bool haveWin = false;
  bool haveLose = false;

  for (Page p : getPages()) {
    if (p.getType() == 'W')
      haveWin = true;
    if (p.getType() == 'L')
      haveLose = true;
    for (Choice c : p.getChoices()) {
      bool find = false;
      referenced[c.getToPageNum()] = true;
      for (Page np : getPages()) {
        if (np.getPageNum() == p.getPageNum())
          continue;
        if (np.getPageNum() == c.getToPageNum()) {
          find = true;
          break;
        }
      }
      if (!find) {
        std::cerr << "referenced page is not exists" << std::endl;
        exit(EXIT_FAILURE);
      }
    }
  }

  if (referenced.size() != getPages().size() - 1) {
    std::cerr << "page is never referenced" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (!haveWin) {
    std::cerr << "No WIN page" << std::endl;
    exit(EXIT_FAILURE);
  }
  if (!haveLose) {
    std::cerr << "No LOSE page" << std::endl;
    exit(EXIT_FAILURE);
  }
  return true;
}
