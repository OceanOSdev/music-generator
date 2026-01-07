#include "arg_parser.hpp"
#include <sstream>
#include <stdexcept>

Args parse_args(int argc, char *argv[]) {
  Args args{};
  for (int i = 1; i < argc; i++) {
    std::string_view arg{argv[i]};

    if (arg == "--help" || arg == "-h") {
      args.help = true;
    } else if (arg == "--input" || arg == "-i") {
      if (i == argc - 1) {
        throw std::runtime_error("Input file not provided");
      }
      std::string_view filename{argv[++i]};
      args.input_file = filename;
      args.input_file_provided = true;
    } else if (arg == "--output" || arg == "-o") {
      if (i == argc - 1) {
        throw std::runtime_error("Output file not provided");
      }
      std::string_view filename{argv[++i]};
      args.output_file = filename;
      args.output_file_provided = true;
    } else {
      throw std::runtime_error("Unknown argument: " + std::string(arg));
    }
  }
  return args;
}

std::string get_help() {
  std::stringstream ss;
  ss << "Usage: music-gen -i <input> -o <output>\n"
     << "\t-i, --input\tInput file\n"
     << "\t-o, --output\tOutput file\n";
  return ss.str();
}
