#include <limits>
#include <sstream>
#include <stdexcept>

#include "file_reading/lexer/lexer.hpp"
#include "file_reading/lexer/token.hpp"
#include "file_reading/parser/node.hpp"
#include "file_reading/parser/node_kinds.hpp"
#include "file_reading/parser/parser.hpp"

namespace FileReading::Parser {

DurationKind dur_from_char(char c) {
  switch (c) {
  case 'w':
    return DurationKind::Whole;
  case 'h':
    return DurationKind::Half;
  case 'q':
    return DurationKind::Quarter;
  case 'e':
    return DurationKind::Eighth;
  case 's':
    return DurationKind::Sixteenth;
  case 't':
    return DurationKind::ThirtySecond;
  default:
    std::stringstream ss;
    ss << "unknown duration kind: '" << c << "'" << std::endl;
    throw std::runtime_error(ss.str());
  }
}

Parser::Parser(std::string_view contents) : _contents(contents) {
  auto const lexer = new FileReading::Lexer::Lexer(contents);
  _tokens = lexer->lex();
}

FileReading::Lexer::Token *Parser::_peek() const { return _tokens[_idx]; }

FileReading::Lexer::Token *Parser::_next() {
  if (_idx >= _tokens.size())
    return _tokens.back();

  auto tok = _tokens[_idx];
  _idx++;
  return tok;
}

SongNode *Parser::parse() { return nullptr; }

Node *Parser::parse_node() { return nullptr; }

LabelNode *Parser::parse_label_node() {
  _next();

  auto id_token = _next();

  _next();

  return new LabelNode(id_token, id_token->lexeme);
}

BpmNode *Parser::parse_bpm_node() {
  auto bpm_token = _next();
  _next(); // ':'

  auto duration_node = parse_duration_node();

  _next(); // '='

  auto bpm_number_token = _next();

  return new BpmNode(bpm_token, std::atoll(bpm_number_token->lexeme.c_str()),
                     duration_node);
}

EofNode *Parser::parse_eof_node() {
  auto token = _next();
  return new EofNode(token);
}

DurationNode *Parser::parse_duration_node() {
  auto duration_token = _next();
  FileReading::Lexer::Token *dot_token = nullptr;
  if (_peek()->kind == FileReading::Lexer::TokenKind::Dot) {
    dot_token = _next();
  }

  return new DurationNode(duration_token,
                          dur_from_char(duration_token->lexeme[0]),
                          dot_token != nullptr);
}

Note note_from_char(char c) {
  if (c < 'A' || c > 'G') {
    std::stringstream ss;
    ss << "unknown note kind: '" << c << "'" << std::endl;
    throw std::runtime_error(ss.str());
  }

  if (c == 'A')
    return Note::A;

  unsigned int note_idx = 1 << (c - 'B');
  return static_cast<Note>(note_idx);
}

Accidental accidental_from_char(char c) {
  switch (c) {
  case '#':
    return Accidental::Sharp;
  case 'b':
    return Accidental::Flat;
  default:
    return Accidental::None;
  }
}

NoteNode *Parser::parse_note_node() {
  auto note_token = _next();
  auto octave_token = _next();
  FileReading::Lexer::Token *accidental_token = nullptr;
  if (_peek()->kind == FileReading::Lexer::TokenKind::Accidental) {
    accidental_token = _next();
  }

  auto accidental = accidental_token != nullptr
                        ? accidental_from_char(accidental_token->lexeme[0])
                        : FileReading::Parser::Accidental::None;
  auto note_letter = note_from_char(note_token->lexeme[0]);
  auto note_octave = std::atoll(octave_token->lexeme.c_str());
  return new NoteNode(note_token, note_letter, accidental, note_octave);
}

NoteInfoNode *Parser::parse_note_info_node() {
  auto token = _peek();
  auto note_node = parse_note_node();
  auto duration_node = parse_duration_node();

  return new NoteInfoNode(token, note_node, duration_node);
}

} // namespace FileReading::Parser
