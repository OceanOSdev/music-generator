#pragma once
#ifndef LEXER_HPP
#define LEXER_HPP

#include <string_view>
#include <vector>

namespace FileReading::Lexer {
struct Token; // Forward declaration
struct SourceLocation;

class Lexer final {
private:
  std::string_view _input;
  std::size_t _i = 0;
  SourceLocation *_loc;
  std::vector<std::string> _diagnostics;
  void eat_whitespace();
  Token *next_token();
  Token *lex_bpm();
  Token *lex_identifier();
  Token *lex_note_id();
  Token *lex_accidental();
  Token *lex_number();
  Token *lex_note_id_or_duration();
  Token *lex_duration();

  bool eof() const;
  char peek() const;
  char peek_next() const;
  void advance();

  bool _lexing_identifier = false;

public:
  Lexer(std::string_view input);
  std::vector<Token *> lex();
  std::vector<std::string> diagnostics() const;
  bool error() const;
};

} // namespace FileReading::Lexer

#endif
