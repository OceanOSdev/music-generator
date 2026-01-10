#include "file_reading/lexer/token.hpp"
#include "file_reading/parser/node.hpp"
#include "file_reading/parser/node_kinds.hpp"

namespace FileReading::Parser {
ErrorNode::ErrorNode(FileReading::Lexer::Token *token) : Node(token) {}

NodeKind ErrorNode::kind() const { return NodeKind::Error; }
} // namespace FileReading::Parser
