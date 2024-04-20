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

enum class TokenType
{
  _exit,
  int_lit,
  semicol,
  open_brac,
  close_brac,
  space,
  include,
  double_quote,
  stdio,
  stdlib,
  new_line,
  string_lit,
  typedef_string,
  equals_to,
  identifier_name,
  _print
};

std::string TokenTypeToString(TokenType type)
{
  switch (type)
  {
  case TokenType::int_lit:
    return "Integer Literal";
  case TokenType::_exit:
    return "Exit Token";
  case TokenType::open_brac:
    return "Open Bracket";
  case TokenType::close_brac:
    return "Close Bracket";
  case TokenType::semicol:
    return "Semicolon";
  case TokenType::space:
    return "Space";
  case TokenType::include:
    return "Include";
  case TokenType::double_quote:
    return "Double Quote";
  case TokenType::stdio:
    return "stdio.h";
  case TokenType::stdlib:
    return "stdlib.h";
  case TokenType::new_line:
    return "New Line";
  case TokenType::string_lit:
    return "String Literal";
  case TokenType::typedef_string:
    return "typedef String";
  case TokenType::equals_to:
    return "Equals To";
  case TokenType::identifier_name:
    return "Identifier Name";
  case TokenType::_print:
    return "Print";
  default:
    return "Unknown";
  }
}

struct Token
{
  TokenType token;
  std::optional<std::string> identifier_name;
  std::optional<std::string> value;
};

class Tokenizer
{
public:
  inline explicit Tokenizer(std::string content)
      : m_src(std::move(content)) {}

  inline std::string get_identifier_name(std::string buf)
  {
    std::string identifier_name;
    if (peek().has_value() && peek().value() == ' ')
    {
      consume();
      while (peek().has_value() && peek().value() != '=')
      {
        buf.push_back(consume());
      }

      if (peek().has_value() && peek().value() == '=')
      {
        consume();
      }

      if (peek().has_value() && peek().value() == ' ')
      {
        consume();
      }

      identifier_name = buf;
      buf.clear();
    }
    return identifier_name;
  }

  inline std::string get_string_value(std::string buf)
  {
    std::string value;
    if (peek().has_value() && peek().value() == '"')
    {
      consume();
      while (peek().has_value() && peek().value() != '"')
      {
        buf.push_back(consume());
      }
      consume();
      value = buf;
      buf.clear();
    }
    return value;
  }

  inline std::string get_print_argument(std::string buf) {

  }

  inline std::vector<Token> tokenize()
  {
    std::vector<Token> tokens;
    std::string buf;
    while (peek().has_value())
    {
      if (std::isalpha(peek().value()))
      {
        while (peek().has_value() && std::isalnum(peek().value()))
        {
          buf.push_back(consume());
        }

        if (buf == "exit")
        {
          tokens.push_back({.token = TokenType::_exit});
          std::cout << TokenTypeToString(tokens.back().token) << std::endl;
          buf.clear();
          continue;
        }
        else if (buf == "include")
        {
          tokens.push_back({.token = TokenType::include});
          std::cout << TokenTypeToString(tokens.back().token) << std::endl;
          buf.clear();
          continue;
        }
        else if (buf == "stdio")
        {
          tokens.push_back({.token = TokenType::stdio});
          std::cout << TokenTypeToString(tokens.back().token) << std::endl;
          buf.clear();
          continue;
        }
        else if (buf == "stdlib")
        {
          tokens.push_back({.token = TokenType::stdlib});
          std::cout << TokenTypeToString(tokens.back().token) << std::endl;
          buf.clear();
          continue;
        }
        else if (buf == "print")
        {
          //get print argument
          string argument = get_print_argument(buf);
          if (argument.empty())
          {
            std::cerr << "Invalid print argument: Print cannot be called without any arguments" << std::endl;
            exit(1);
          }

          tokens.push_back({.token = TokenType::_print});
          std::cout << TokenTypeToString(tokens.back().token) << std::endl;
          buf.clear();
          continue;
        }

        // variable assignment
        // 1: string
        else if (buf == "string")
        {
          buf.clear();

          // get the variable name
          std::string identifier_name = get_identifier_name(buf);

          if (identifier_name.empty())
          {
            std::cerr << "Invalid identifier name: Please enter a variable name" << std::endl;
            exit(1);
          }
          else {
            std::cout << "Identifier Name: " << identifier_name << std::endl;
          }
          // get the value of the variable
          std::string value = get_string_value(buf);
          std::cout << "Value: " << value << std::endl;

          tokens.push_back({.token = TokenType::string_lit, .identifier_name = identifier_name, .value = value});
          std::cout << TokenTypeToString(tokens.back().token) << " " << m_index << std::endl;
          buf.clear();
          continue;
        }
      }
      else if (std::isdigit(peek().value()))
      {
        buf.push_back(consume());
        while (peek().has_value() && std::isdigit(peek().value()))
        {
          buf.push_back(consume());
        }
        tokens.push_back({.token = TokenType::int_lit, .value = buf});
        std::cout << TokenTypeToString(tokens.back().token) << std::endl;
        buf.clear();
        continue;
      }
      else if (peek().value() == '(')
      {
        consume();
        tokens.push_back({.token = TokenType::open_brac});
        std::cout << TokenTypeToString(tokens.back().token) << std::endl;
        continue;
      }
      else if (peek().value() == ')')
      {
        consume();
        tokens.push_back({.token = TokenType::close_brac});
        std::cout << TokenTypeToString(tokens.back().token) << std::endl;
        continue;
      }
      else if (peek().value() == '"')
      {
        consume();
        tokens.push_back({.token = TokenType::double_quote});
        std::cout << TokenTypeToString(tokens.back().token) << std::endl;
        while (peek().has_value() && peek().value() != '"')
        {
          buf.push_back(consume());
        }
        continue;
      }
      else if (peek().value() == '\n')
      {
        consume();
        tokens.push_back({.token = TokenType::new_line});
        std::cout << TokenTypeToString(tokens.back().token) << std::endl;
        continue;
      }
      else if (std::isspace(peek().value()))
      {
        consume();
        tokens.push_back({.token = TokenType::space});
        std::cout << TokenTypeToString(tokens.back().token) << std::endl;
        continue;
      }
      else if (peek().value() == ';')
      {
        consume();
        tokens.push_back({.token = TokenType::semicol});
        std::cout << TokenTypeToString(tokens.back().token) << std::endl;
        continue;
      }
      else
      {
        std::cerr << "Ya F'ed up bro!" << std::endl;
        exit(EXIT_FAILURE);
      }
    }
    m_index = 0;
    return tokens;
  }

private:
  [[nodiscard]] inline std::optional<char> peek(int offset = 0) const
  {
    if (m_index + offset >= m_src.length())
    {
      return {};
    }
    else
    {
      return m_src.at(m_index + offset);
    }
  }

  inline char consume()
  {
    return m_src.at(m_index++);
  }

  const std::string m_src;
  size_t m_index = 0;
};
