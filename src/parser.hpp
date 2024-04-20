#pragma once

#include "tokenizer.hpp"
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <utility>
#include <vector>

enum class NodeType
{
  ExitNode,
  IncludeNode, 
  UsrFuncNode
};

struct ExprNode
{
  Token token;
};

// struct ExitNode
// {
//   ExprNode expr;
// };

// struct IncludeNode
// {
//   ExprNode expr;
// };

struct Node
{
  NodeType type;
  std::optional<TokenType> token_type;
  std::optional<ExprNode> expr;
};

class Parser
{
public:
  explicit Parser(std::vector<Token> tokens) : m_tokens(std::move(tokens))
  {
    for (auto &token : m_tokens)
    {
      if (token.token == TokenType::new_line)
      {
        m_new_line_count++;
      }
    }
  }

  std::optional<ExprNode> ParseExpr()
  {
    if (peek().has_value() && (
      peek().value().token == TokenType::int_lit ||
      peek().value().token == TokenType::stdio || 
      peek().value().token == TokenType::stdlib
      )
    )
    {
      return ExprNode{.token = consume()};
    }
    return {};
  }

  std::vector<std::optional<Node>> Parse()
  {
    std::cout << "\nParsing code:";
    std::vector<std::optional<Node>> nodes;
    while (peek().has_value())
    {
          std::optional<Node> node;
      std::cout << "\nloop start:\n";
      if (peek().value().token == TokenType::_exit)
      {
        std::cout << "Checking exit function\n";
        consume();

        // check for opening bracket
        if (peek().has_value() && peek().value().token == TokenType::open_brac)
        {
          consume();
          std::cout << "Found open_brac\n";
        }
        else
        {
          std::cerr << "Invalid Statement: Missing open bracket '('\n";
          exit(EXIT_FAILURE);
        }

        // check for inner expression
        if (auto exprNode = ParseExpr())
        {
          if (exprNode->token.token == TokenType::int_lit)
          {
            node = Node{.type = NodeType::ExitNode, .expr = exprNode.value()};
            std::cout << "ExitNode expr value: " << node->expr->token.value.value() << std::endl;
          }
          else
          {
            std::cerr << "Invalid Expression\n";
            exit(EXIT_FAILURE);
          }
        }
        else
        {
          std::cerr << "Invalid Expression\n";
          exit(EXIT_FAILURE);
        }

        // check for closed bracket
        if (peek().has_value() && peek().value().token == TokenType::close_brac)
        {
          consume();
          std::cout << "Found close_brac\n";
        }
        else
        {
          std::cerr << "Invalid Statement: Missing close bracket ')'\n";
          exit(EXIT_FAILURE);
        }
      }
      else if (peek().value().token == TokenType::include)
      {
        consume();
        std::cout << "Found include\n";
        if (peek().has_value() && peek().value().token == TokenType::open_brac)
        {
          consume();
          std::cout << "Found open_brac\n";
        }
        else
        {
          std::cerr << "Invalid Statement: Missing open bracket '('\n";
          exit(EXIT_FAILURE);
        }

        // check for inner expression
        if (auto exprNode = ParseExpr())
        {
          if (exprNode->token.token == TokenType::stdio)
          {
            node = Node{.type = NodeType::IncludeNode, .expr = exprNode.value()};
            std::cout << "IncludeNode expr value: " << TokenTypeToString(exprNode->token.token) << std::endl;
          }
          else if (exprNode->token.token == TokenType::stdlib)
          {
            node = Node{.type = NodeType::IncludeNode, .expr = exprNode.value()};
            std::cout << "IncludeNode expr value: " << TokenTypeToString(exprNode->token.token) << std::endl;
          }
          else
          {
            std::cerr << "Invalid Expression\n";
            exit(EXIT_FAILURE);
          }
        }
        else
        {
          std::cerr << "Invalid Expression\n";
          exit(EXIT_FAILURE);
        }

        // check for closed bracket
        if (peek().has_value() && peek().value().token == TokenType::close_brac)
        {
          consume();
          std::cout << "Found close_brac\n";
        }
        else
        {
          std::cerr << "Invalid Statement: Missing close bracket ')'\n";
          exit(EXIT_FAILURE);
        }
      }

      if (!peek().has_value() || peek().value().token != TokenType::semicol)
      {
        std::cerr << "Invalid Expression: Missing semicolon ';' after expression at index " << m_index << std::endl;
        if (peek().has_value())
        {
          std::cerr << "Current token: " << TokenTypeToString(peek().value().token) << std::endl;
        }
        else
        {
          std::cerr << "No token available." << std::endl;
        }
        exit(EXIT_FAILURE);
      }
      else
      {
        consume();
        std::cout << "Found semicolon\n";
      }

      if (peek().has_value() && peek().value().token == TokenType::new_line)
      {
        // Increment the index to skip the new line token
        consume();
        std::cout << "Found new line\n";
      }

              // Create a new node and add it to the nodes vector
        if (node.has_value())
        {
          nodes.push_back(node.value());
        }
          std::cout << "Index: " << m_index << std::endl;
    }

    m_index = 0;
    return nodes;
  }

private:
  [[nodiscard]] std::optional<Token> peek(const int offset = 0) const
  {
    if (m_index + offset >= m_tokens.size())
    {
      return {};
    }
    return m_tokens.at(m_index + offset);
  }

  Token consume()
  {
    return m_tokens.at(m_index++);
  }

  const std::vector<Token> m_tokens;
  size_t m_index = 0, m_new_line_count = 0;
};
