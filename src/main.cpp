#include <iostream>

#include "arg_parser.hpp"

int main(int argc, char *argv[]) {
  auto args = parse_args(argc, argv);
  if (args.help) {
    std::cout << get_help() << std::endl;
  }
  return 0;
}
