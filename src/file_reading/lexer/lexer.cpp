#include "file_reading/lexer/lexer.hpp"
#include "file_reading/lexer/token.hpp"
#include <set>
#include <sstream>

namespace FileReading::Lexer {
Token *make_token(TokenKind kind, SourceLocation loc, std::string lexeme);
void trim_in_place(std::string &s);
std::string error_unexpected_identifier(char expected, char actual,
                                        SourceLocation loc);

std::string error_unexpected_eof(char expected, SourceLocation loc);
std::string error_unexpected_identifier(char id, SourceLocation loc);
std::string report_error(std::string error, SourceLocation loc);

Lexer::Lexer(std::string_view input)
    : _input(input), _loc(new SourceLocation()) {}

bool Lexer::eof() const { return _i >= _input.size(); }

char Lexer::peek() const { return _input[_i]; }

char Lexer::peek_next() const {
  if (_i + 1 >= _input.size())
    return _input[_i];
  return _input[_i + 1];
}

bool Lexer::error() const { return !_diagnostics.empty(); }

std::vector<std::string> Lexer::diagnostics() const { return _diagnostics; }

std::vector<Token *> Lexer::lex() {
  std::vector<Token *> lexemes;

  while (true) {
    Token *t = next_token();
    lexemes.push_back(t);
    if (t->kind == TokenKind::Eof)
      break;
  }

  return lexemes;
}

Token *Lexer::next_token() {
  eat_whitespace();
  const auto start = _loc;

  if (eof())
    return make_token(TokenKind::Eof, *start, "");

  if (_lexing_identifier) {
    _lexing_identifier = false;
    return lex_identifier();
  }
  const char c = peek();
  switch (c) {
  case ':':
    advance();
    return make_token(TokenKind::Colon, *start, ":");
  case '=':
    advance();
    return make_token(TokenKind::Equal, *start, "=");
  case '.':
    advance();
    return make_token(TokenKind::Dot, *start, ".");
  case '[':
    _lexing_identifier = true;
    advance();
    return make_token(TokenKind::LBracket, *start, "[");
  case ']':
    advance();
    return make_token(TokenKind::RBracket, *start, "]");
  case 'B':
    if (peek_next() == 'P') {
      return lex_bpm();
    }
    return lex_note_id();
  case 'R':
    advance();
    return make_token(TokenKind::Rest, *start, "R");
  case '#':
  case 'b':
    return lex_accidental();
  default:
    if (std::isdigit(static_cast<unsigned char>(c))) {
      return lex_number();
    }
    if (std::isalpha(static_cast<unsigned char>(c))) {
      return lex_note_id_or_duration();
    }

    _diagnostics.push_back(error_unexpected_identifier(c, *start));
    advance();
    return make_token(TokenKind::Error, *start, std::string{c});
  }

  return nullptr;
}

Token *Lexer::lex_bpm() {
  const auto start = _loc;
  std::string lexeme;
  lexeme.push_back(peek());
  advance(); // B
  lexeme.push_back(peek());
  advance(); // P
  char M = peek();
  if (M != 'M') {
    _diagnostics.push_back(error_unexpected_identifier('M', M, *start));
    advance();
    return make_token(TokenKind::Error, *start, lexeme);
  }
  lexeme.push_back(M);
  advance(); // M
  return make_token(TokenKind::Bpm, *start, lexeme);
}

Token *Lexer::lex_identifier() {
  const auto start = _loc;
  const std::size_t begin = _i;

  advance(); // '['
  std::string name;
  while (!eof() && peek() != ']' && peek() != '\n') {
    name.push_back(peek());
    advance();
  }

  trim_in_place(name);

  if (eof() || peek() != ']') {
    if (eof()) {
      _diagnostics.push_back(error_unexpected_eof(']', *start));
    } else {
      _diagnostics.push_back(error_unexpected_identifier(']', peek(), *start));
    }

    return make_token(TokenKind::Error, *start, name);
  }

  return make_token(TokenKind::Identifier, *start,
                    std::string(_input.substr(begin, _i - begin)));
}

Token *Lexer::lex_note_id_or_duration() {
  char curr = peek();

  if (curr >= 'a' && curr <= 'z') {
    return lex_duration();
  }

  return lex_note_id();
}

const std::set<char> durations = {'w', 'h', 'q', 'e', 's', 't'};

Token *Lexer::lex_duration() {
  const auto start = _loc;
  char dur = peek();
  std::string lexeme = std::string{peek()};
  if (!durations.contains(dur)) {
    _diagnostics.push_back(error_unexpected_identifier(dur, *start));
    advance();
    return make_token(TokenKind::Error, *start, lexeme);
  }
  advance();
  return make_token(TokenKind::Duration, *start, lexeme);

  return nullptr;
}

Token *Lexer::lex_note_id() {
  const auto start = _loc;
  char note = peek();
  std::string lexeme = std::string{peek()};
  if (note < 'A' || note > 'G') {
    advance();
    _diagnostics.push_back(error_unexpected_identifier(note, *start));
    return make_token(TokenKind::Error, *start, lexeme);
  }
  advance();
  return make_token(TokenKind::NoteId, *start, lexeme);
}

Token *Lexer::lex_accidental() {
  const auto start = _loc;
  std::string lexeme = std::string{peek()};
  advance();
  return make_token(TokenKind::Accidental, *start, lexeme);
}

Token *Lexer::lex_number() {
  const auto start = _loc;

  std::stringstream ss;
  while (!eof() && std::isdigit(static_cast<unsigned char>(peek()))) {
    ss << peek();
    advance();
  }

  return make_token(TokenKind::Number, *start, ss.str());
}

void Lexer::eat_whitespace() {
  while (!eof()) {
    const char c = peek();

    // Simple comment convention: ';' to end-of-line
    if (c == ';') {
      while (!eof() && peek() != '\n')
        advance();
      continue;
    }

    if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
      advance();
      continue;
    }

    break;
  }
}

void Lexer::advance() {
  const char c = _input[_i++];
  if (c == '\n') {
    ++_loc->line;
    _loc->col = 1;
  } else {
    ++_loc->col;
  }
}

Token *make_token(TokenKind kind, SourceLocation loc, std::string lexeme) {
  return new Token{.kind = kind, .loc = loc, .lexeme = std::move(lexeme)};
}

void trim_in_place(std::string &s) {
  auto is_ws = [](unsigned char ch) { return std::isspace(ch) != 0; };
  while (!s.empty() && is_ws(static_cast<unsigned char>(s.front())))
    s.erase(s.begin());
  while (!s.empty() && is_ws(static_cast<unsigned char>(s.back())))
    s.pop_back();
}

std::string error_unexpected_identifier(char id, SourceLocation loc) {
  auto message = "Unexpected identifier '" + std::string{id} + "'";
  return report_error(message, loc);
}

std::string error_unexpected_identifier(char expected, char actual,
                                        SourceLocation loc) {
  auto expected_str = std::string{expected};
  auto actual_str = std::string{actual};
  auto message = "Expected '" + expected_str + "' but got '" + actual_str + "'";
  return report_error(message, loc);
}

std::string error_unexpected_eof(char expected, SourceLocation loc) {
  auto message = "Expected '" + std::string{expected} + "' but got End of File";
  return report_error(message, loc);
}

std::string report_error(std::string error, SourceLocation loc) {
  return "Error: " + error + " at " + loc.to_string();
}
} // namespace FileReading::Lexer
