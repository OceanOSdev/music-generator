#pragma once
#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

namespace FileReading::Lexer {
enum class TokenKind : unsigned int {
  Bpm,
  Identifier, // START, END, etc
  NoteId,     // [A-G]
  Number,     // 1,2,3,...
  Accidental, // #, b, or empty
  Duration,   // q, e, h, s, ...
  Dot,        // .
  Colon,      // :
  Equal,      // =
  LBracket,   // [
  RBracket,   // ]
  Error,
  Eof
};

std::string token_kind_to_str(TokenKind kind);

struct SourceLocation {
  std::size_t line = 1;
  std::size_t col = 1;

  std::string to_string() const;
};

struct Token {
  TokenKind kind;
  SourceLocation loc;
  std::string lexeme;

  std::string to_string() const;
};
} // namespace FileReading::Lexer

#endif
