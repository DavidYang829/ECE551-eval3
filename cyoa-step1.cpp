#include <iostream>

#include "story.hpp"

/**
 * display all pages and choices without playing
 */
void displayAllStory(Story story) {
  std::map<std::string, int> emptyVars;
  for (Page p : story.getPages()) {
    std::cout << "Page " << p.getPageNum() << std::endl;
    std::cout << "==========" << std::endl;
    p.display(emptyVars);
  }
}

int main(int argc, char ** argv) {
  if (argc != 2) {
    std::cerr << "argument needed." << std::endl;
    exit(EXIT_FAILURE);
  }
  Story story;
  story.load(argv[1]);
  displayAllStory(story);
  return EXIT_SUCCESS;
}
