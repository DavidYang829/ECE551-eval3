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
  story.play();
  return EXIT_SUCCESS;
}
