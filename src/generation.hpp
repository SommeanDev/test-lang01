#pragma once

#include "parser.hpp"
#include <sstream>
#include <string>
#include <utility>

class Generation
{
public:
  inline explicit Generation(std::vector<std::optional<Node>> root) : m_root(std::move(root)) {}

  std::string generate() const
  {
    std::stringstream outputstream, includeStream, programStream;
    // outputstream << "global _start\n_start:\n";
    // outputstream << "    mov rax, 60\n";
    // outputstream << "    mov rdi, " << m_root.expr.int_lit.value.value() << "\n";
    // outputstream << "    syscall";
    for (auto &optNode : m_root)
    {
      if (optNode.has_value())
      {
        const Node &node = optNode.value();
        if (node.type == NodeType::IncludeNode)
        {
          if (node.expr.has_value())
          {
            const ExprNode &exprNode = node.expr.value();
            includeStream << "#include <" << TokenTypeToString(node.expr->token.token) << ">\n";
          }
        }
        else if (node.type == NodeType::ExitNode)
        {
          if (node.expr.has_value())
          {
            programStream << "void main() {\n";
            const ExprNode &exprNode = node.expr.value();
            if (exprNode.token.token == TokenType::int_lit)
            {
              programStream << "exit(" << node.expr->token.value.value() << ");\n";
            }
            programStream << "}\n";
          }
        }
      }
    }

    outputstream << includeStream.str();
    outputstream << programStream.str();
    std::cout << outputstream.str();
    return outputstream.str();
  }

private:
  const std::vector<std::optional<Node>> m_root;
};
