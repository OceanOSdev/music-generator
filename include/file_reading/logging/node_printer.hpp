#pragma once
#ifndef NODE_PRINTER_HPP
#define NODE_PRINTER_HPP

#include <string>
#include <vector>

namespace FileReading {
namespace Parser {
class Node;
class BpmNode;
class LabelNode;
class DurationNode;
class NoteNode;
class NoteInfoNode;
} // namespace Parser

namespace Logging {
void log_node(Parser::Node *node, std::string pad = "");
void log_nodes(std::vector<Parser::Node *> nodes);
void log_label(Parser::LabelNode *node, std::string pad = "");
void log_duration(Parser::DurationNode *node, std::string pad = "");
void log_note(Parser::NoteNode *node, std::string pad = "");
void log_note_info(Parser::NoteInfoNode *node, std::string pad = "");
void log_bpm(Parser::BpmNode *node, std::string pad = "");
void log_error(std::string pad = "");
} // namespace Logging
} // namespace FileReading

#endif
