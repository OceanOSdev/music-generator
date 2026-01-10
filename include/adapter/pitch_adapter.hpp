#pragma once
#ifndef PITCH_ADAPTER_HPP
#define PITCH_ADAPTER_HPP

namespace FileReading::Parser {
class NoteNode;
} // namespace FileReading::Parser

namespace Adapter {
double note_to_hertz(FileReading::Parser::NoteNode *note);
}
#endif
