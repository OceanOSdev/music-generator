#pragma once
#ifndef PARSER_HPP
#define PARSER_HPP

#include <string_view>
#include <vector>

namespace FileReading {
namespace Lexer {
struct Token;
struct SourceLocation;
enum class TokenKind : unsigned int;
} // namespace Lexer
namespace Parser {
class Node;
class SongNode;
class LabelNode;
class BpmNode;
class EofNode;
class DurationNode;
class NoteNode;
class NoteInfoNode;

class Parser {
private:
  std::string_view _contents;
  std::vector<Lexer::Token *> _tokens;
  std::size_t _idx = 0;

  std::vector<std::string> _diagnostics;

  FileReading::Lexer::Token *_peek() const;
  FileReading::Lexer::Token *_next();

  Node *parse_node();
  Node *parse_label_node();
  Node *parse_bpm_node();
  Node *parse_eof_node();
  Node *parse_duration_node();
  Node *parse_note_node();
  Node *parse_note_info_node();

  bool match(FileReading::Lexer::Token *token,
             FileReading::Lexer::TokenKind expected);
  bool match_and_flag(FileReading::Lexer::Token *token,
                      FileReading::Lexer::Token *&err_tok,
                      FileReading::Lexer::TokenKind expected);
  void report_error(std::string message,
                    FileReading::Lexer::SourceLocation loc);
  void report_unexpected_token(FileReading::Lexer::TokenKind expected,
                               FileReading::Lexer::TokenKind actual,
                               FileReading::Lexer::SourceLocation loc);

public:
  Parser(std::string_view contents);

  SongNode *parse();

  bool error() const;

  std::vector<std::string> diagnostics() const;
};

} // namespace Parser
} // namespace FileReading
#endif
