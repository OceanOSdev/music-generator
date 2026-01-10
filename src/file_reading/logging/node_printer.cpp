#include <iostream>

#include "file_reading/logging/node_printer.hpp"
#include "file_reading/parser/node.hpp"
#include "file_reading/parser/node_kinds.hpp"

namespace FileReading::Logging {
std::string node_kind_to_str(FileReading::Parser::NodeKind kind) {
  switch (kind) {
  case Parser::NodeKind::Bpm_Decl:
    return "Bpm_Decl";
  case Parser::NodeKind::Note_Info:
    return "Note_Info";
  case Parser::NodeKind::Note:
    return "Note";
  case Parser::NodeKind::Duration:
    return "Duration";
  case Parser::NodeKind::Label:
    return "Label";
  case Parser::NodeKind::Song:
    return "Song";
  case Parser::NodeKind::Error:
    return "Error";
  case Parser::NodeKind::Eof:
    return "Eof";
    break;
  }
}

std::string duration_to_string(FileReading::Parser::DurationKind dur) {
  switch (dur) {
  case Parser::DurationKind::Whole:
    return "W";
  case Parser::DurationKind::Half:
    return "H";
  case Parser::DurationKind::Quarter:
    return "Q";
  case Parser::DurationKind::Eighth:
    return "E";
  case Parser::DurationKind::Sixteenth:
    return "S";
  case Parser::DurationKind::ThirtySecond:
    return "T";
  }
}

std::string note_to_str(FileReading::Parser::Note note) {
  switch (note) {
  case Parser::Note::A:
    return "A";
  case Parser::Note::B:
    return "B";
  case Parser::Note::C:
    return "C";
  case Parser::Note::D:
    return "D";
  case Parser::Note::E:
    return "E";
  case Parser::Note::F:
    return "F";
  case Parser::Note::G:
    return "G";
  }
}
std::string acc_to_str(FileReading::Parser::Accidental acc) {
  switch (acc) {
  case Parser::Accidental::None:
    return "";
  case Parser::Accidental::Flat:
    return "Flat";
  case Parser::Accidental::Sharp:
    return "Sharp";
  }
}

void log_node(Parser::Node *node, std::string pad) {
  if (node == nullptr) {
    log_error(pad);
    return;
  }

  switch (node->kind()) {
  case Parser::NodeKind::Bpm_Decl:
    log_bpm(static_cast<FileReading::Parser::BpmNode *>(node), pad);
    break;
  case Parser::NodeKind::Note_Info:
    log_note_info(static_cast<FileReading::Parser::NoteInfoNode *>(node), pad);
    break;
  case Parser::NodeKind::Note:
    log_note(static_cast<FileReading::Parser::NoteNode *>(node), pad);
    break;
  case Parser::NodeKind::Duration:
    log_duration(static_cast<FileReading::Parser::DurationNode *>(node), pad);
    break;
  case Parser::NodeKind::Label:
    log_label(static_cast<FileReading::Parser::LabelNode *>(node), pad);
    break;
  case Parser::NodeKind::Song:
    break;
  case Parser::NodeKind::Error:
    log_error(pad);
    break;
  case Parser::NodeKind::Eof:
    std::cout << pad << "[EOF]" << std::endl;
    break;
  }
}

void log_nodes(std::vector<Parser::Node *> nodes) {
  std::cout << nodes.size() << std::endl;
  for (auto node : nodes) {
    log_node(node);
  }
}

void log_label(Parser::LabelNode *node, std::string pad) {
  std::cout << pad << "[Label] " << node->label() << std::endl;
}

void log_duration(Parser::DurationNode *node, std::string pad) {
  auto duration_kind = node->duration();
  auto dotted = node->dotted() ? "yes" : "no";

  std::cout << pad << "[Duration] " << duration_to_string(duration_kind)
            << "[dotted? " << dotted << "]" << std::endl;
}

void log_note(Parser::NoteNode *node, std::string pad) {
  auto note = note_to_str(node->note());
  auto acc = acc_to_str(node->accidental());
  auto oct = node->octave();

  std::cout << pad << "[Note] " << note << acc << " " << oct << std::endl;
}

void log_note_info(Parser::NoteInfoNode *node, std::string pad) {
  std::cout << pad << "[NoteInfo]" << std::endl;
  if (!node->is_rest())
    log_node(node->note(), pad + "    ");
  else
    std::cout << pad << "    " << "Rest" << std::endl;
  log_node(node->duration(), pad + "    ");
}

void log_bpm(Parser::BpmNode *node, std::string pad) {
  std::cout << pad << "[BPM] " << node->bpm() << std::endl;
  log_node(node->duration(), "    ");
}

void log_error(std::string pad) { std::cout << pad << "[Error]" << std::endl; }
} // namespace FileReading::Logging
