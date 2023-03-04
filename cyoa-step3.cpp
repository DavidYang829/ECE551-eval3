#include <iostream>

#include "story.hpp"

int main(int argc, char ** argv) {
  if (argc != 2) {
    std::cerr << "argument needed." << std::endl;
    exit(EXIT_FAILURE);
  }
  Story story;
  story.load(argv[1]);
  story.verify();
  std::vector<std::vector<std::pair<int, int> > > res = story.findCycleFreeWinPaths();
  for (auto path : res) {
    for (int i = 0; i < path.size(); i++) {
      std::pair<int, int> node = path[i];
      if (i)
        std::cout << ",";
      if (node.second == -1)
        std::cout << node.first << "(win)" << std::endl;
      else
        std::cout << node.first << "(" << node.second << ")";
    }
  }
  return EXIT_SUCCESS;
}
