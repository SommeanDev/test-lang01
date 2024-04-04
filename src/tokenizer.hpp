#pragma once

#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <string>
#include <sys/types.h>
#include <utility>
#include <vector>

enum class TokenType {
  _exit,
  int_lit,
  semicol,
  open_brac,
  close_brac
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
      std::vector<Token> tokens;
      std::string buf;
      while (peek().has_value()) {
        if (std::isalpha(peek().value())) {
          buf.push_back(consume());
          while (peek().has_value() && std::isalnum(peek().value())) {
            buf.push_back(consume());    
          }
          if (buf == "exit") {
            tokens.push_back({.token = TokenType::_exit});
            buf.clear();
            continue;
          }
        }
        else if (std::isdigit(peek().value())) {
          buf.push_back(consume());
          while (peek().has_value() && std::isdigit(peek().value())) {
            buf.push_back(consume());
          }
          tokens.push_back({.token = TokenType::int_lit, .value = buf});
          buf.clear();
          continue;
        }
        else if (peek().value() == ';') {
          consume();
          tokens.push_back({.token = TokenType::semicol});
          continue;
        }
        else if (peek().value() == '(') {
          consume();
          tokens.push_back({.token = TokenType::open_brac});
          continue;
        }
        else if (peek().value() == ')') {
          consume();
          tokens.push_back({.token = TokenType::close_brac});
          continue;
        }
        else if (std::isspace(peek().value())) {
          consume();
          continue;
        }
        else {
          std::cerr << "Ya F'ed up bro!" << std::endl;
          exit(EXIT_FAILURE);
        }
      }
      m_index = 0;
      return tokens;
    }

  private:
    [[nodiscard]]inline std::optional<char> peek(int offset = 0) const 
    {
      if (m_index + 1 >= m_src.length()) {
        return {};
      }
      else {
        return m_src.at(m_index + offset);
      }
    }

    inline char consume() {
      return m_src.at(m_index++);
    }

    const std::string m_src;
    size_t m_index = 0;
};
