#pragma once
#ifndef NOTE_INFO_HPP
#define NOTE_INFO_HPP

namespace Audio {
struct NoteInfo {
  NoteInfo(double freq, double dur) : freq_hz(freq), dur_s(dur) {}
  double freq_hz; // Frequency in hertz
  double dur_s;   // Duration in seconds
};
} // namespace Audio

#endif
