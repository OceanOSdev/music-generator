#include <fstream>
#include <iostream>
#include <string>

#include "arg_parser.hpp"
#include "file_reading/lexer/lexer.hpp"
#include "file_reading/lexer/token.hpp"

std::string read_file_to_string(const std::string &path);

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

  try {
    auto text = read_file_to_string(std::string(args.input_file));
    auto lexer = new FileReading::Lexer::Lexer(text);
    auto contents = lexer->lex();
    for (auto content : contents) {
      std::cout << content->to_string();
    }
  } catch (const std::exception &e) {
    // errm... what
    std::cerr << "errrm" << std::endl;
    std::cerr << e.what() << std::endl;
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
