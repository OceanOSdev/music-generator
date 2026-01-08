#include "file_reading/lexer/token.hpp"
#include "file_reading/parser/node.hpp"

namespace FileReading::Parser {

NoteNode::NoteNode(FileReading::Lexer::Token *token, Note note,
                   Accidental accidental)
    : Node(token), _note(note), _accidental(accidental) {}

NodeKind NoteNode::kind() const { return NodeKind::Note; }

Note NoteNode::note() const { return _note; }

Accidental NoteNode::accidental() const { return _accidental; }

} // namespace FileReading::Parser
