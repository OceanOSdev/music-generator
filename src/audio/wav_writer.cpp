#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iostream>

#include "audio/note_info.hpp"
#include "audio/wav_writer.hpp"

namespace Audio {

constexpr std::uint32_t kSampleRate = 44'100;
constexpr std::uint16_t kChannels = 1;
constexpr std::uint16_t kBitsPerSample = 16;
constexpr std::uint16_t kBytesPerSample = kBitsPerSample / 8;
constexpr std::uint16_t kBlockAlign = static_cast<std::uint16_t>(
    kChannels * kBytesPerSample); // bytes per audio frame
constexpr std::uint32_t kByteRate = kSampleRate * kBlockAlign;

void write_bytes(std::ostream &os, const void *data, std::size_t n) {
  os.write(static_cast<const char *>(data), static_cast<std::streamsize>(n));
  if (!os)
    throw std::runtime_error("I/O error while writing WAV");
}

void write_u16_le(std::ostream &os, std::uint16_t v) {
  const std::array<std::uint8_t, 2> b{
      static_cast<std::uint8_t>(v & 0xFFu),
      static_cast<std::uint8_t>((v >> 8) & 0xFFu),
  };

  write_bytes(os, b.data(), b.size());
}

void write_u32_le(std::ostream &os, std::uint32_t v) {
  const std::array<std::uint8_t, 4> b{
      static_cast<std::uint8_t>(v & 0xFFu),
      static_cast<std::uint8_t>((v >> 8) & 0xFFu),
      static_cast<std::uint8_t>((v >> 16) & 0xFFu),
      static_cast<std::uint8_t>((v >> 24) & 0xFFu),
  };

  write_bytes(os, b.data(), b.size());
}

void write_tag(std::ostream &os, const char (&tag)[5]) {
  // 4 ASCII bytes, e.g. "RIFF", "WAVE", "fmt ", "data"
  write_bytes(os, tag, 4);
}

void write_pcm16_mono_header(std::ostream &os, std::uint32_t num_samples) {
  // "data" chunk size is the number of bytes of sample payload.
  // mono PCM16 => 2 bytes per sample.
  const std::uint32_t data_bytes = num_samples * kBlockAlign;

  // RIFF chunk size is file size minus 8 bytes (the "RIFF" tag + this size
  // field). A PCM WAV header before the data payload is 44 bytes total, i.e.:
  //   12 bytes RIFF header ("RIFF"+size+"WAVE")
  // + 24 bytes fmt chunk ("fmt "+size+fmtdata)
  // + 8 bytes data header ("data"+datasize)
  // => 44 bytes
  const std::uint32_t riff_chunk_size = 36u + data_bytes;

  // --- RIFF container header ---
  write_tag(os, "RIFF");
  write_u32_le(os, riff_chunk_size);
  write_tag(os, "WAVE");

  // --- fmt chunk (describes how to interpret the sample bytes) ---
  write_tag(os, "fmt ");
  write_u32_le(os, 16);             // PCM fmt chunk payload size (always 16)
  write_u16_le(os, 1);              // AudioFormat = 1 (PCM Integer)
  write_u16_le(os, kChannels);      // NumChannels
  write_u32_le(os, kSampleRate);    // SampleRate
  write_u32_le(os, kByteRate);      // ByteRate = SampleRate * BlockAlign
  write_u16_le(os, kBlockAlign);    // BlockAlign = NumChannels * BytesPerSample
  write_u16_le(os, kBitsPerSample); // BitsPerSample

  // --- data chunk header ---
  write_tag(os, "data");
  write_u32_le(os, data_bytes);
}

void write_pcm16_mono_wav(const std::string &path,
                          const std::vector<std::int16_t> &samples) {

  std::ofstream out(path, std::ios::binary);
  if (!out)
    throw std::runtime_error("Failed to open output file: " + path);

  write_pcm16_mono_header(out, static_cast<std::uint32_t>(samples.size()));

  // We can write sample bytes directly because PCM16 payload is just
  // little-endian i16. However, host endianness might be big-endian on some
  // (probably exotic) systems. If we want total portability, we should probably
  // write each sample via write_u16_le instead.
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
  for (std::int16_t s : samples)
    write_u16_le(out, static_cast<std::uint16_t>(s));
#else
  write_bytes(out, samples.data(), samples.size() * sizeof(std::int16_t));
#endif
}

std::vector<std::int16_t> encode_melody(const std::vector<NoteInfo *> &notes,
                                        double amplitude, double fade_s) {

  if (amplitude < 0.0 || amplitude > 1.0) {
    std::cerr << "Amplitude must be in [0,1] range." << std::endl;
  }

  const auto sr = static_cast<double>(kSampleRate);
  const int fade_samples = std::max(0, static_cast<int>(fade_s * sr));

  std::vector<std::int16_t> out;
  out.reserve(1'000'000); // cheap guiess; not required

  constexpr double tau = 2.0 * std::numbers::pi;

  for (const auto &n : notes) {
    const int n_samples = std::max(0, static_cast<int>(n->dur_s * sr));
    for (int i = 0; i < n_samples; ++i) {
      // Envelope to avoiod hard incontinuities (clicks)
      double env = 1.0;
      if (fade_samples > 0) {
        if (i < fade_samples)
          env = static_cast<double>(i) / fade_samples;
        if (n_samples - i - 1 < fade_samples) {
          env = std::min(env,
                         static_cast<double>(n_samples - i - 1) / fade_samples);
        }
        env = std::clamp(env, 0.0, 1.0);
      }

      double sample = 0.0;
      if (n->freq_hz > 0.0) {
        const double t = static_cast<double>(i) / sr;
        sample = std::sin(tau * n->freq_hz * t);
      }

      const double x = amplitude * env * sample;
      const double scaled =
          x * static_cast<double>(std::numeric_limits<std::int16_t>::max());
      out.push_back(static_cast<std::int16_t>(scaled));
    }
  }

  return out;
}

} // namespace Audio
