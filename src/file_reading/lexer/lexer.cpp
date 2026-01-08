#include "file_reading/lexer/lexer.hpp"
#include "file_reading/lexer/token.hpp"
#include <set>
#include <sstream>
#include <stdexcept>

namespace FileReading::Lexer {
Token *make_token(TokenKind kind, SourceLocation loc, std::string lexeme);
void trim_in_place(std::string &s);

Lexer::Lexer(std::string_view input)
    : _input(input), _loc(new SourceLocation()) {}

bool Lexer::eof() const { return _i >= _input.size(); }

char Lexer::peek() const { return _input[_i]; }

char Lexer::peek_next() const {
  if (_i + 1 >= _input.size())
    return _input[_i];
  return _input[_i + 1];
}

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
    return lex_identifier();
  case 'B':
    if (peek_next() == 'P') {
      return lex_bpm();
    }
    return lex_note_id();
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

    std::string letter = std::string{c};
    std::stringstream ss;
    ss << "Unexpected identifier: '" << c << "' at (" << _loc->line << ", "
       << _loc->col << ")";
    throw std::runtime_error(ss.str());
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
    throw std::runtime_error("Expected 'M' in lexing BPM");
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

  if (eof() || peek() != ']') {
    throw std::runtime_error("Unterminated section tag (missing ']')");
  }
  advance(); // ']'

  trim_in_place(name);

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
    std::stringstream ss;
    ss << "Unknown duration '" << dur << "'" << std::endl;
    throw std::runtime_error(ss.str());
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
    throw std::runtime_error("Invalid note: " + lexeme);
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
} // namespace FileReading::Lexer
