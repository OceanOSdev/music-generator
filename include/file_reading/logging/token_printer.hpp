#pragma once
#ifndef TOKEN_PRINTER_HPP
#define TOKEN_PRINTER_HPP

#include <vector>

namespace FileReading {
namespace Lexer {
struct Token;
}

namespace Logging {
void log_tokens(std::vector<Lexer::Token *> tokens);
}
} // namespace FileReading

#endif
