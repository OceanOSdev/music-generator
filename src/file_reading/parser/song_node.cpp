#include "file_reading/lexer/token.hpp"
#include "file_reading/parser/node.hpp"
#include "file_reading/parser/node_kinds.hpp"

namespace FileReading::Parser {

SongNode::SongNode(Lexer::Token *token, BpmNode *bpm, LabelNode *start,
                   std::vector<NoteInfoNode *> notes, LabelNode *end)
    : Node(token), _bpm(bpm), _start(start), _notes(notes), _end(end) {}

NodeKind SongNode::kind() const { return NodeKind::Song; }

BpmNode *SongNode::bpm() const { return _bpm; }

LabelNode *SongNode::start() const { return _start; }

std::vector<NoteInfoNode *> SongNode::notes() const { return _notes; }

LabelNode *SongNode::end() const { return _end; }

} // namespace FileReading::Parser
