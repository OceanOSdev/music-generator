#include "file_reading/lexer/token.hpp"

#include <sstream>

namespace FileReading::Lexer {

std::string token_kind_to_str(TokenKind kind) {
  switch (kind) {
  case TokenKind::Bpm:
    return "BPM";
  case TokenKind::Identifier:
    return "IDENTIFIER";
  case TokenKind::NoteId:
    return "NOTEID";
  case TokenKind::Number:
    return "NUMBER";
  case TokenKind::Accidental:
    return "ACCIDENTAL";
  case TokenKind::Duration:
    return "DURATION";
  case TokenKind::Dot:
    return "DOT";
  case TokenKind::Colon:
    return "COLON";
  case TokenKind::Equal:
    return "EQUAL";
  case TokenKind::LBracket:
    return "LBRACKET";
  case TokenKind::RBracket:
    return "RBRACKET";
  case TokenKind::Eof:
    return "EOF";
  }
}

std::string SourceLocation::to_string() const {
  std::stringstream ss;
  ss << "(" << line << ", " << col << ")";
  return ss.str();
}

std::string Token::to_string() const {
  std::stringstream ss;
  ss << "[" << token_kind_to_str(kind) << "] " << "'" << lexeme << "' "
     << loc.to_string() << std::endl;
  return ss.str();
}
} // namespace FileReading::Lexer
