#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "arg_parser.hpp"
#include "file_reading/lexer/lexer.hpp"
#include "file_reading/logging/node_printer.hpp"
#include "file_reading/logging/token_printer.hpp"
#include "file_reading/parser/parser.hpp"

std::string read_file_to_string(const std::string &path);
void log_diagnostics(std::vector<std::string> diagnostics);

int main(int argc, char *argv[]) {
  auto args = parse_args(argc, argv);
  if (args.help) {
    std::cout << get_help() << std::endl;
    return 0;
  }
  if (!args.input_file_provided) {
    std::cerr << "Error: need input file" << std::endl;
    std::cerr << get_help() << std::endl;
    return 1;
  }

  auto text = read_file_to_string(std::string(args.input_file));
  if (args.lex_only) {
    auto lexer = new FileReading::Lexer::Lexer(text);
    auto contents = lexer->lex();
    if (lexer->error()) {
      log_diagnostics(lexer->diagnostics());
      return 1;
    }
    FileReading::Logging::log_tokens(contents);
    return 0;
  }
  if (args.parse_only) {
    auto parser = new FileReading::Parser::Parser(text);
    auto result = parser->parse();
    if (result->error()) {
      log_diagnostics(result->diagnostics());
      return 1;
    }
    FileReading::Logging::log_nodes(result->nodes());
    return 0;
  }

  return 0;
}

std::string read_file_to_string(const std::string &path) {
  std::ifstream file(path, std::ios::binary | std::ios::ate);
  if (!file) {
    throw std::runtime_error("Failed to open file: " + path);
  }

  std::size_t size = file.tellg();
  file.seekg(0);

  std::string buffer(size, '\0');
  file.read(buffer.data(), size);

  return buffer;
}

void log_diagnostics(std::vector<std::string> diagnostics) {
  for (auto diag : diagnostics) {
    std::cerr << diag << std::endl;
  }
}
