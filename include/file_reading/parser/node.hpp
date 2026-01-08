#pragma once
#ifndef NODE_HPP
#define NODE_HPP

#include <string>

namespace FileReading {
namespace Lexer {
struct Token;
}

namespace Parser {
enum class NodeKind : unsigned int {
  Bpm_Decl,
  Note_Info,
  Note,
  Duration,
  Label,
  Eof
};

enum class DurationKind : unsigned int {
  Whole = 0,
  Half = 1,
  Quarter = 1 << 1,
  Eighth = 1 << 2,
  Sixteenth = 1 << 3,
  ThirtySecond = 1 << 4
};

enum class Note : unsigned int {
  A = 0,
  B = 1,
  C = 1 << 1,
  D = 1 << 2,
  E = 1 << 3,
  F = 1 << 4,
  G = 1 << 5
};

enum class Accidental : unsigned char { None = 0, Flat = 1, Sharp = 1 << 1 };

DurationKind char_to_duration(char c);
Note char_to_note(char c);
DurationKind char_to_dur(char c);

class Node {
protected:
  Lexer::Token *_token;

public:
  Node(Lexer::Token *token);

  virtual NodeKind kind() const = 0;

  Lexer::Token *token() const;
};

class EofNode : public Node {
public:
  EofNode(Lexer::Token *token);

  NodeKind kind() const override;
};

class BpmNode : public Node {
private:
  unsigned int _bpm;

public:
  BpmNode(Lexer::Token *token, unsigned int bpm);

  NodeKind kind() const override;

  unsigned int bpm() const;
};

class LabelNode : public Node {
private:
  std::string _label;

public:
  LabelNode(Lexer::Token *token, std::string label);

  NodeKind kind() const override;

  std::string label() const;
};

class DurationNode : public Node {
private:
  DurationKind _duration;
  bool _dotted;

public:
  DurationNode(Lexer::Token *token, DurationKind duration, bool dotted);

  NodeKind kind() const override;

  DurationKind duration() const;

  bool dotted() const;
};

class NoteNode : public Node {
private:
  Note _note;
  Accidental _accidental;

public:
  NoteNode(Lexer::Token *token, Note note, Accidental accidental);

  NodeKind kind() const override;

  Note note() const;

  Accidental accidental() const;
};

class NoteInfoNode : public Node {
private:
  NoteNode *_note;
  DurationNode *_duration;

public:
  NoteInfoNode(Lexer::Token *token, NoteNode *note, DurationNode *duration);

  NodeKind kind() const override;

  NoteNode *note() const;

  DurationNode *duration() const;
};

} // namespace Parser
} // namespace FileReading

#endif
