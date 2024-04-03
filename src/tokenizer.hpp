#pragma once

#include <optional>
#include <string>
#include <utility>
#include <vector>

enum class TokenType {
  _exit,
  int_lit,
  semicol
};

struct Token {
  TokenType token;
  std::optional<std::string> value;
};

class Tokenizer {
  public:
    inline explicit Tokenizer(std::string content) 
      : m_src(std::move(content)) {}

    inline std::vector<Token> tokenize() {

    }

  private:
    const std::string m_src;
};
