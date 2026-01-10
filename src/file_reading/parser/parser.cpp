#include <iostream>

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
    // this can't actually happen, gets caught in the
    // lexer, so just return a random one.
    return DurationKind::Whole;
  }
}

Parser::Parser(std::string_view contents) : _contents(contents) {
  auto const lexer = new FileReading::Lexer::Lexer(contents);
  _tokens = lexer->lex();
  _diagnostics = lexer->diagnostics();
}

ParseResult::ParseResult(SongNode *song, std::vector<Node *> nodes,
                         std::vector<std::string> diag)
    : _song_node(song), _nodes(nodes), _diagnostics(diag) {}

std::vector<std::string> ParseResult::diagnostics() const {
  return _diagnostics;
}

std::vector<Node *> ParseResult::nodes() const { return _nodes; }

bool ParseResult::error() const { return !_diagnostics.empty(); }

SongNode *ParseResult::song() const { return _song_node; }

FileReading::Lexer::Token *Parser::_peek() const { return _tokens[_idx]; }

FileReading::Lexer::Token *Parser::_next() {
  if (_idx >= _tokens.size())
    return _tokens.back();

  auto tok = _tokens[_idx];
  _idx++;
  return tok;
}

ParseResult *Parser::parse() {
  if (!_diagnostics.empty()) {
    return new ParseResult(nullptr, {}, _diagnostics);
  }

  auto bpm = parse_bpm_node();
  auto start = parse_label_node();
  LabelNode *end = nullptr;
  std::vector<Node *> nodes{bpm, start};
  std::vector<NoteInfoNode *> note_info_nodes{};
  bool eof = false;
  while (!eof) {
    auto node = parse_node();
    nodes.push_back(node);
    auto kind = node->kind();
    switch (kind) {
    case NodeKind::Label:
      end = static_cast<LabelNode *>(node);
      break;
    case NodeKind::Eof:
      eof = true;
      break;
    case NodeKind::Note_Info:
      note_info_nodes.push_back(static_cast<NoteInfoNode *>(node));
      break;
    default:
      break;
    }
  }

  auto song_node =
      new SongNode(_tokens.front(), dynamic_cast<BpmNode *>(bpm),
                   dynamic_cast<LabelNode *>(start), note_info_nodes,
                   dynamic_cast<LabelNode *>(end));

  return new ParseResult(song_node, nodes, _diagnostics);
}

Node *Parser::parse_node() {
  auto kind = _peek()->kind;
  switch (kind) {
  case Lexer::TokenKind::Bpm:
    return parse_bpm_node();
  case Lexer::TokenKind::Rest:
  case Lexer::TokenKind::NoteId:
    return parse_note_info_node();
  case Lexer::TokenKind::Duration:
    return parse_duration_node();
  case Lexer::TokenKind::LBracket:
    return parse_label_node();
  case Lexer::TokenKind::Error:
    return new ErrorNode(_next()); // TODO: handle error type
  case Lexer::TokenKind::Eof:
    return parse_eof_node();
  default:
    return new ErrorNode(_next()); // TODO: handle error type
  }
}

Node *Parser::parse_label_node() {
  FileReading::Lexer::Token *err_tok = nullptr;

  auto l_bracket_token = _next();
  match_and_flag(l_bracket_token, err_tok,
                 FileReading::Lexer::TokenKind::LBracket);

  auto id_token = _next();
  match_and_flag(id_token, err_tok, FileReading::Lexer::TokenKind::Identifier);

  auto r_bracket_token = _next();
  match_and_flag(r_bracket_token, err_tok,
                 FileReading::Lexer::TokenKind::RBracket);

  if (err_tok != nullptr)
    return new ErrorNode(err_tok);

  return new LabelNode(id_token, id_token->lexeme);
}

Node *Parser::parse_bpm_node() {
  FileReading::Lexer::Token *err_tok = nullptr;

  auto bpm_token = _next();
  match_and_flag(bpm_token, err_tok, FileReading::Lexer::TokenKind::Bpm);

  auto colon_token = _next(); // ':'
  match_and_flag(colon_token, err_tok, FileReading::Lexer::TokenKind::Colon);

  auto duration_node = parse_duration_node();
  if (duration_node->kind() == NodeKind::Error) {
    // do something
  }

  auto eq_token = _next(); // '='
  match_and_flag(eq_token, err_tok, FileReading::Lexer::TokenKind::Equal);

  auto bpm_number_token = _next();
  match_and_flag(bpm_number_token, err_tok,
                 FileReading::Lexer::TokenKind::Number);

  if (err_tok != nullptr)
    return new ErrorNode(err_tok);

  return new BpmNode(bpm_token, std::atoll(bpm_number_token->lexeme.c_str()),
                     dynamic_cast<DurationNode *>(duration_node));
}

Node *Parser::parse_eof_node() {
  auto token = _next();
  return new EofNode(token);
}

Node *Parser::parse_duration_node() {
  FileReading::Lexer::Token *err_tok = nullptr;

  auto duration_token = _next();
  match_and_flag(duration_token, err_tok,
                 FileReading::Lexer::TokenKind::Duration);
  FileReading::Lexer::Token *dot_token = nullptr;
  if (_peek()->kind == FileReading::Lexer::TokenKind::Dot) {
    dot_token = _next();
  }

  if (err_tok != nullptr)
    return new ErrorNode(err_tok);

  return new DurationNode(duration_token,
                          dur_from_char(duration_token->lexeme[0]),
                          dot_token != nullptr);
}

Note note_from_char(char c) {
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

Node *Parser::parse_note_node() {
  FileReading::Lexer::Token *err_tok = nullptr;

  auto note_token = _next();
  match_and_flag(note_token, err_tok, FileReading::Lexer::TokenKind::NoteId);

  unsigned int note_octave = 99;
  auto octave_token = _next();
  if (!match_and_flag(octave_token, err_tok,
                      FileReading::Lexer::TokenKind::Number)) {
  } else {
    note_octave = std::atoll(octave_token->lexeme.c_str());
  }
  FileReading::Lexer::Token *accidental_token = nullptr;
  if (_peek()->kind == FileReading::Lexer::TokenKind::Accidental) {
    accidental_token = _next();
  }

  if (err_tok != nullptr) {
    return new ErrorNode(err_tok);
  }

  auto accidental = accidental_token != nullptr
                        ? accidental_from_char(accidental_token->lexeme[0])
                        : FileReading::Parser::Accidental::None;
  auto note_letter = note_from_char(note_token->lexeme[0]);
  return new NoteNode(note_token, note_letter, accidental, note_octave);
}

Node *Parser::parse_note_info_node() {
  Node *note_node = nullptr;
  FileReading::Lexer::Token *tok = nullptr;
  if (_peek()->kind != FileReading::Lexer::TokenKind::Rest) {
    note_node = parse_note_node();
    tok = note_node->token();
  } else {
    auto rest_token = _next();
    match(rest_token, FileReading::Lexer::TokenKind::Rest);
    tok = rest_token;
  }
  auto duration_node = parse_duration_node();

  return new NoteInfoNode(tok, dynamic_cast<NoteNode *>(note_node),
                          dynamic_cast<DurationNode *>(duration_node));
}

bool Parser::match(FileReading::Lexer::Token *token,
                   FileReading::Lexer::TokenKind expected) {

  if (token->kind != expected) {
    report_unexpected_token(expected, token->kind, token->loc);
    return false;
  }

  return true;
}

bool Parser::match_and_flag(FileReading::Lexer::Token *token,
                            FileReading::Lexer::Token *&err_tok,
                            FileReading::Lexer::TokenKind expected) {
  auto matches = match(token, expected);
  if (!matches) {
    err_tok = token;
  }
  return matches;
}

void Parser::report_error(std::string message,
                          FileReading::Lexer::SourceLocation loc) {
  _diagnostics.push_back("Error: " + message + " at " + loc.to_string());
}

void Parser::report_unexpected_token(FileReading::Lexer::TokenKind expected,
                                     FileReading::Lexer::TokenKind actual,
                                     FileReading::Lexer::SourceLocation loc) {
  auto expected_tok = FileReading::Lexer::token_kind_to_str(expected);
  auto actual_tok = FileReading::Lexer::token_kind_to_str(actual);
  auto message = "Unexpected token [" + actual_tok + "], (expected [" +
                 expected_tok + "])";
  report_error(message, loc);
}

} // namespace FileReading::Parser
