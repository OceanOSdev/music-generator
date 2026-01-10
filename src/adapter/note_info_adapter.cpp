#include <iostream>

#include "adapter/note_info_adapter.hpp"
#include "adapter/pitch_adapter.hpp"
#include "audio/note_info.hpp"
#include "file_reading/parser/node.hpp"
#include "file_reading/parser/node_kinds.hpp"

namespace Adapter {

double duration_coeff(FileReading::Parser::DurationKind base,
                      FileReading::Parser::DurationKind note_dur) {
  return static_cast<double>(base) / static_cast<double>(note_dur);
}

double calculate_duration(FileReading::Parser::DurationKind base,
                          FileReading::Parser::DurationKind note_dur,
                          unsigned int bpm, bool dotted = false) {
  double beats_per_sec = static_cast<double>(bpm) / 60;
  double beat_dur = 1 / beats_per_sec;
  double dur = duration_coeff(base, note_dur) * beat_dur;
  return dotted ? 1.5 * dur : dur;
}

NoteInfoAdapter::NoteInfoAdapter(FileReading::Parser::SongNode *song)
    : _song(song) {}

std::vector<Audio::NoteInfo *> NoteInfoAdapter::convert() {
  auto bpm_node = _song->bpm();
  auto bpm = bpm_node->bpm();
  auto duration = _song->bpm()->duration();
  if (duration->dotted()) {
    // will figure out conversion later
    std::cout << "Warning: dotted bpms not yet supported" << std::endl;
  }
  std::vector<Audio::NoteInfo *> notes{};
  for (auto note : _song->notes()) {
    auto hertz = note_to_hertz(note->note());
    auto durr = calculate_duration(duration->duration(),
                                   note->duration()->duration(), bpm);
    notes.push_back(new Audio::NoteInfo(hertz, durr));
  }

  return notes;
}

} // namespace Adapter
