#pragma once
#ifndef ARG_PARSER_HPP
#define ARG_PARSER_HPP

#include <string>
#include <string_view>

struct Args {
  std::string_view input_file;
  std::string_view output_file;
  bool help = false;
  bool input_file_provided = false;
  bool output_file_provided = false;
};

Args parse_args(int argc, char *argv[]);

std::string get_help();

#endif
