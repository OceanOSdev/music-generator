#pragma once
#ifndef WAV_WRITER_HPP
#define WAV_WRITER_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace Audio {
struct NoteInfo;

void write_pcm16_mono_wav(const std::string &path,
                          const std::vector<std::int16_t> &samples);

std::vector<std::int16_t> encode_melody(const std::vector<NoteInfo *> &notes,
                                        double amplitude = 0.25,
                                        double fade_s = 0.005);

} // namespace Audio

#endif
