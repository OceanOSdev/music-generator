#include "file_reading/lexer/token.hpp"
#include "file_reading/parser/node.hpp"
#include "file_reading/parser/node_kinds.hpp"

namespace FileReading::Parser {
EofNode::EofNode(FileReading::Lexer::Token *token) : Node(token) {}

NodeKind EofNode::kind() const { return NodeKind::Eof; }

} // namespace FileReading::Parser
