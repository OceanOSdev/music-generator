#pragma once
#ifndef NODE_KINDS_HPP
#define NODE_KINDS_HPP

namespace FileReading::Parser {
enum class NodeKind : unsigned int {
  Bpm_Decl,
  Note_Info,
  Note,
  Duration,
  Label,
  Eof
};

enum class DurationKind : unsigned int {
  Whole = 0,
  Half = 1,
  Quarter = 1 << 1,
  Eighth = 1 << 2,
  Sixteenth = 1 << 3,
  ThirtySecond = 1 << 4
};

enum class Note : unsigned int {
  A = 0,
  B = 1,
  C = 1 << 1,
  D = 1 << 2,
  E = 1 << 3,
  F = 1 << 4,
  G = 1 << 5
};

enum class Accidental : unsigned char { None = 0, Flat = 1, Sharp = 1 << 1 };

} // namespace FileReading::Parser
#endif
