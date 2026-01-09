#pragma once
#ifndef NODE_HPP
#define NODE_HPP

#include <string>
#include <vector>

namespace FileReading {
namespace Lexer {
struct Token;
}

namespace Parser {
// Forward declarations
enum class NodeKind : unsigned int;
enum class DurationKind : unsigned int;
enum class Note : unsigned int;
enum class Accidental : unsigned char;

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
  unsigned int _octave;

public:
  NoteNode(Lexer::Token *token, Note note, Accidental accidental,
           unsigned int octave);

  NodeKind kind() const override;

  Note note() const;

  Accidental accidental() const;

  unsigned int octave() const;
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

class BpmNode : public Node {
private:
  unsigned int _bpm;
  DurationNode *_duration;

public:
  BpmNode(Lexer::Token *token, unsigned int bpm, DurationNode *duration);

  NodeKind kind() const override;

  unsigned int bpm() const;

  DurationNode *duration() const;
};

class SongNode : public Node {
private:
  BpmNode *_bpm;
  LabelNode *_start;
  std::vector<NoteInfoNode *> _notes;
  LabelNode *_end;

public:
  SongNode(Lexer::Token *token, BpmNode *bpm, LabelNode *start,
           std::vector<NoteInfoNode *> notes, LabelNode *end);

  NodeKind kind() const override;

  BpmNode *bpm() const;

  LabelNode *start() const;

  std::vector<NoteInfoNode *> notes() const;

  LabelNode *end() const;
};

} // namespace Parser
} // namespace FileReading

#endif
