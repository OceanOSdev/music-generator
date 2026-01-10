#include "adapter/pitch_helpers.hpp"


namespace Adapter {

/// Given a [Note] note (assumed to be in the fourth octave)
/// returns the frequence of the note in the given octave.
Note get_freq(Note note, unsigned int octave) {
  // if default octave, don't do anything, just return note.
  if (octave == 4)
    return note;

  if (octave < 4) {
    unsigned int divisor = 1 << (4 - octave);
    return note / divisor;
  }

  unsigned int multi = 1 << (octave - 4);
  return note * multi;
}
}
