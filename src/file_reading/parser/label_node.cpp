#include "file_reading/lexer/token.hpp"
#include "file_reading/parser/node.hpp"
#include "file_reading/parser/node_kinds.hpp"

namespace FileReading::Parser {
LabelNode::LabelNode(FileReading::Lexer::Token *token, std::string label)
    : Node(token), _label(label) {}

NodeKind LabelNode::kind() const { return NodeKind::Label; }

std::string LabelNode::label() const { return _label; }
} // namespace FileReading::Parser
