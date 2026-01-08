#include "file_reading/lexer/token.hpp"
#include "file_reading/parser/node.hpp"

namespace FileReading::Parser {
EofNode::EofNode(FileReading::Lexer::Token *token) : Node(token) {}

NodeKind EofNode::kind() const { return NodeKind::Eof; }

} // namespace FileReading::Parser
