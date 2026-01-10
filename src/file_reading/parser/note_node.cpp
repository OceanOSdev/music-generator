#include "file_reading/lexer/token.hpp"
#include "file_reading/parser/node.hpp"
#include "file_reading/parser/node_kinds.hpp"

namespace FileReading::Parser {

NoteNode::NoteNode(FileReading::Lexer::Token *token, Note note,
                   Accidental accidental, unsigned int octave)
    : Node(token), _note(note), _accidental(accidental), _octave(octave) {}

NodeKind NoteNode::kind() const { return NodeKind::Note; }

Note NoteNode::note() const { return _note; }

Accidental NoteNode::accidental() const { return _accidental; }

unsigned int NoteNode::octave() const { return _octave; }

} // namespace FileReading::Parser
