#pragma once
#ifndef NOTE_INFO_ADAPTER_HPP
#define NOTE_INFO_ADAPTER_HPP

#include <vector>

namespace FileReading::Parser {
class SongNode;
}

namespace Audio {
struct NoteInfo;
}

namespace Adapter {
class NoteInfoAdapter {
private:
  FileReading::Parser::SongNode *_song;

public:
  NoteInfoAdapter(FileReading::Parser::SongNode *song);

  std::vector<Audio::NoteInfo *> convert();
};

} // namespace Adapter

#endif
