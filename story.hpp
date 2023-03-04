#ifndef STORY_H
#define STORY_H

#include <map>
#include <vector>

#include "page.hpp"

class Story {
 private:
  std::vector<Page> pages;
  std::map<std::string, int> vars;
  int currentPageNum = 0;
  void findCycleFreeWinPaths(int currentPageNum,
                             std::map<int, bool> & visited,
                             std::vector<std::pair<int, int> > & history,
                             std::vector<std::vector<std::pair<int, int> > > & paths);
  void move(int pageNum);
  Page & getPage(int pageNum);
  bool addPage(Page p);
  Page getCurrentPage();
  bool end();
  void updateVars();

 public:
  Story();
  void load(std::string folder);
  void play();
  bool verify();
  std::vector<std::vector<std::pair<int, int> > > findCycleFreeWinPaths();
  std::vector<Page> getPages();
};

#endif  //STORY_H
