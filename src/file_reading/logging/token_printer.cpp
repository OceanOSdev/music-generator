#include <iostream>

#include "file_reading/lexer/token.hpp"
#include "file_reading/logging/token_printer.hpp"

namespace FileReading::Logging {
void log_tokens(std::vector<Lexer::Token *> tokens) {
  for (auto token : tokens) {
    std::cout << token->to_string();
  }
}
} // namespace FileReading::Logging
