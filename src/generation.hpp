#pragma once

#include "parser.hpp"
#include <sstream>
#include <string>
#include <utility>

class Generation {
  public:
    inline explicit Generation(ExitNode root) : m_root(std::move(root)) {}

    inline std::string generate() const {
      std::stringstream outputstream;
      outputstream << "global _start\n_start:\n";
      outputstream << "    mov rax, 60\n";
      outputstream << "    mov rdi, " << m_root.expr.int_lit.value.value() << "\n";
      outputstream << "    syscall";
      return outputstream.str();
    }
  private:
    const ExitNode m_root;
};
