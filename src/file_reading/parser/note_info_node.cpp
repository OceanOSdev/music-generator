#include "file_reading/lexer/token.hpp"
#include "file_reading/parser/node.hpp"

namespace FileReading::Parser {

NoteInfoNode::NoteInfoNode(FileReading::Lexer::Token *token, NoteNode *note,
                           DurationNode *duration)
    : Node(token), _note(note), _duration(duration) {}

NodeKind NoteInfoNode::kind() const { return NodeKind::Note_Info; }

NoteNode *NoteInfoNode::note() const { return _note; }

DurationNode *NoteInfoNode::duration() const { return _duration; }

} // namespace FileReading::Parser
