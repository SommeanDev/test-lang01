#include <cstdlib>
#include <fstream>
#include <ios>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "generation.hpp"

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

  Parser parser(std::move(tokens));

  std::optional<ExitNode> tree = parser.Parse();

  if (!tree.has_value()) {
    std::cerr << "No statement found\n";
    exit(EXIT_FAILURE);
  }
  else {
    std::cout << "Found statement: " << tree->expr.int_lit.value.value();
  }
  Generation generation(tree.value());
  {
    std::fstream file("out.asm", std::ios::out);
    file << generation.generate();
  }

  std::cout << "\n";
  system("nasm -felf64 out.asm");
  system("ld -o ../test.out out.o");
  return EXIT_SUCCESS;
}
