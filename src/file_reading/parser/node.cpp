#include "file_reading/parser/node.hpp"
#include "file_reading/lexer/token.hpp"

namespace FileReading::Parser {

Node::Node(FileReading::Lexer::Token *token) : _token(token) {}

FileReading::Lexer::Token *Node::token() const { return _token; }

} // namespace FileReading::Parser
