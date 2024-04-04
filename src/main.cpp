#include <cstdlib>
#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "tokenizer.hpp"

int main (int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Incorrect usage. Use instead: " << std::endl;
    std::cerr << "tlan <file.tl>\n";
    return EXIT_FAILURE;
  }
  
  std::string contents;
  {
    std::fstream input(argv[1], std::ios::in);
    std::stringstream content_stream;
    content_stream << input.rdbuf();
    contents = content_stream.str();
  }

  Tokenizer tokenizer(contents);
  std::vector<Token> tokens = tokenizer.tokenize();

  {
    std::fstream file("out.asm", std::ios::out);
    // file << generator.gen_prog();
  }

  std::cout << contents << std::endl;
  return EXIT_SUCCESS;
}
