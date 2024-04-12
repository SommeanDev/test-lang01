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
    std::cerr << "tlan <file.tal>\n";
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

  std::vector<std::optional<Node>> tree = parser.Parse();

  if (tree.size() == 0) {
    std::cerr << "No statement found\n";
    exit(EXIT_FAILURE);
  }
  else {
    std::cout << "Tree nodes: " << tree.size() << std::endl;
  }
  
  Generation generation(tree);
  {
    std::fstream file("out.c", std::ios::out);
    file << generation.generate();
  }

 std::string filename = argv[1];

  // Find the position of the last '.' character
  size_t dot_position = filename.find_last_of('.');
  if (dot_position == std::string::npos) {
      std::cerr << "No extension found in the filename.\n";
      return 1;
  }

  // Extract the filename without the extension
  std::string basename = filename.substr(0, dot_position);

  // Construct the command
  std::stringstream command_stream;
  command_stream << "gcc out.c -o " << basename << ".exe";
  std::string command = command_stream.str();

  std::cout << "\n";

  system(command.c_str());
  return EXIT_SUCCESS;
}
