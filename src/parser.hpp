#pragma once

#include "tokenizer.hpp"
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <utility>
#include <vector>

struct ExprNode {
  Token int_lit;
};

struct ExitNode {
  ExprNode expr;
};

class Parser {
public:
  inline explicit Parser(std::vector<Token> tokens) : m_tokens(std::move(tokens)) {}

  inline std::optional<ExprNode> ParseExpr() {
    if (peek().has_value() && peek().value().token == TokenType::int_lit) {
      return ExprNode{.int_lit = consume()}; 
    }
    return {};
  }

  inline std::optional<ExitNode> Parse() {
    std::cout << "Parsing code:\n";
    std::optional<ExitNode> exitNode;
    while (peek().has_value()) {
      std::cout << "loop start:\n";
      if (peek().value().token == TokenType::_exit) {
        std::cout << "Checking exit function\n";
        consume();
        
        //check for opening bracket
        if (peek().has_value() && peek().value().token == TokenType::open_brac) {
          consume();
          std::cout << "Found open_brac\n";
        }
        else {
          std::cerr << "Invalid Statement: Missing open bracket '('\n";
          exit(EXIT_FAILURE);
        }
        
        //check for inner expression
        if (auto exprNode = ParseExpr()) {
          exitNode = ExitNode{.expr = exprNode.value()};
          std::cout << "ExitNode expr value: " << exitNode->expr.int_lit.value.value() << std::endl;
        }
        else {
          std::cerr << "Invalid Expression\n";
          exit(EXIT_FAILURE);
        }

        //check for closed bracket
        if (peek().has_value() && peek().value().token == TokenType::close_brac) {
          consume();
          std::cout << "Found close_brac\n"; 
        }
        else {
          std::cerr << "Invalid Statement: Missing close bracket ')'\n";
          exit(EXIT_FAILURE);
        }
      }

      if (!peek().has_value() || peek().value().token != TokenType::semicol) {
        std::cerr << "Invalid Expressiion: Missing semicolon ';' after expression\n";
        exit(EXIT_FAILURE);
      }
      else {
        consume();
        std::cout << "Found semicolon\n";
      }
    }
    m_index = 0;
    return exitNode;
  }
private:
    [[nodiscard]] std::optional<Token> peek(const int offset = 0) const
    {
        if (m_index + offset >= m_tokens.size()) {
            return {};
        }
        return m_tokens.at(m_index + offset);
    }

    Token consume()
    {
        return m_tokens.at(m_index++);
    }


  const std::vector<Token> m_tokens;
  size_t m_index = 0; 
};
