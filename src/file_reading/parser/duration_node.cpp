#include "file_reading/lexer/token.hpp"
#include "file_reading/parser/node.hpp"
#include "file_reading/parser/node_kinds.hpp"

namespace FileReading::Parser {

DurationNode::DurationNode(FileReading::Lexer::Token *token,
                           DurationKind duration, bool dotted)
    : Node(token), _duration(duration), _dotted(dotted) {}

NodeKind DurationNode::kind() const { return NodeKind::Duration; }

DurationKind DurationNode::duration() const { return _duration; }

bool DurationNode::dotted() const { return _dotted; }

} // namespace FileReading::Parser
