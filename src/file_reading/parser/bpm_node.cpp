#include "file_reading/lexer/token.hpp"
#include "file_reading/parser/node.hpp"
#include "file_reading/parser/node_kinds.hpp"

namespace FileReading::Parser {
BpmNode::BpmNode(FileReading::Lexer::Token *token, unsigned int bpm,
                 DurationNode *duration)
    : Node(token), _bpm(bpm), _duration(duration) {}

NodeKind BpmNode::kind() const { return NodeKind::Bpm_Decl; }

unsigned int BpmNode::bpm() const { return _bpm; }

DurationNode *BpmNode::duration() const { return _duration; }

} // namespace FileReading::Parser
