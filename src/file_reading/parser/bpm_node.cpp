#include "file_reading/lexer/token.hpp"
#include "file_reading/parser/node.hpp"

namespace FileReading::Parser {
BpmNode::BpmNode(FileReading::Lexer::Token *token, unsigned int bpm)
    : Node(token), _bpm(bpm) {}

NodeKind BpmNode::kind() const { return NodeKind::Bpm_Decl; }

unsigned int BpmNode::bpm() const { return _bpm; }

} // namespace FileReading::Parser
