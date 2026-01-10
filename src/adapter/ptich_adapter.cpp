#include "adapter/pitch_adapter.hpp"
#include "adapter/pitch_helpers.hpp"
#include "file_reading/parser/node.hpp"
#include "file_reading/parser/node_kinds.hpp"

namespace Adapter {

double nat_to_hertz(FileReading::Parser::Note note) {
  switch (note) {
  case FileReading::Parser::Note::A:
    return A;
  case FileReading::Parser::Note::B:
    return B;
  case FileReading::Parser::Note::C:
    return C;
  case FileReading::Parser::Note::D:
    return D;
  case FileReading::Parser::Note::E:
    return E;
  case FileReading::Parser::Note::F:
    return F;
  case FileReading::Parser::Note::G:
    return G;
  }
}

double flat_to_hertz(FileReading::Parser::Note note) {
  switch (note) {
  case FileReading::Parser::Note::A:
    return AFlat;
  case FileReading::Parser::Note::B:
    return BFlat;
  case FileReading::Parser::Note::C:
    return B;
  case FileReading::Parser::Note::D:
    return DFlat;
  case FileReading::Parser::Note::E:
    return EFlat;
  case FileReading::Parser::Note::F:
    return E;
  case FileReading::Parser::Note::G:
    return GFlat;
  }
}

double sharp_to_hertz(FileReading::Parser::Note note) {
  switch (note) {
  case FileReading::Parser::Note::A:
    return ASharp;
  case FileReading::Parser::Note::B:
    return C;
  case FileReading::Parser::Note::C:
    return CSharp;
  case FileReading::Parser::Note::D:
    return DSharp;
  case FileReading::Parser::Note::E:
    return F;
  case FileReading::Parser::Note::F:
    return FSharp;
  case FileReading::Parser::Note::G:
    return GSharp;
  }
}

double note_to_hertz(FileReading::Parser::Note note,
                     FileReading::Parser::Accidental acc, unsigned int octave) {

  double hertz = 0;
  switch (acc) {
  case FileReading::Parser::Accidental::None:
    hertz = nat_to_hertz(note);
    break;
  case FileReading::Parser::Accidental::Flat:
    hertz = flat_to_hertz(note);
    break;
  case FileReading::Parser::Accidental::Sharp:
    hertz = sharp_to_hertz(note);
    break;
  }

  return get_freq(hertz, octave);
}

double note_to_hertz(FileReading::Parser::NoteNode *note) {
  return note_to_hertz(note->note(), note->accidental(), note->octave());
}
} // namespace Adapter
