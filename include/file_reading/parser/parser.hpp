#pragma once
#ifndef PARSER_HPP
#define PARSER_HPP

#include <string_view>
#include <vector>

namespace FileReading {
namespace Lexer {
struct Token;
}
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

  FileReading::Lexer::Token *_peek() const;
  FileReading::Lexer::Token *_next();

  Node *parse_node();
  LabelNode *parse_label_node();
  BpmNode *parse_bpm_node();
  EofNode *parse_eof_node();
  DurationNode *parse_duration_node();
  NoteNode *parse_note_node();
  NoteInfoNode *parse_note_info_node();

public:
  Parser(std::string_view contents);

  SongNode *parse();
};

} // namespace Parser
} // namespace FileReading
#endif
