/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/ExecStreamHelper.h"

bool udocs_processor::ExecStreamHelper::ReadErrLine(
    std::string& Line, exec_stream_t &ExecStream) {
  Line = "";
  try {
    std::string ReadLine;
    if (std::getline(ExecStream.err(), ReadLine).good()) {
      Line = std::move(ReadLine) + NEWLINE;
      return true;
    }
  } catch (const std::exception& Exc) {}

  return false;
}

bool udocs_processor::ExecStreamHelper::ReadOutLine(
    std::string& Line, exec_stream_t &ExecStream) {
  Line = "";
  try {
    std::string ReadLine;
    if (std::getline(ExecStream.out(), ReadLine).good()) {
      Line = std::move(ReadLine) + NEWLINE;
      return true;
    }
  } catch (const std::exception& Exc) {}

  return false;
}

bool udocs_processor::ExecStreamHelper::IsProcessRunning(
    exec_stream_t &ExecStream) {
  try {
    ExecStream.exit_code();
    return false;
  } catch (const std::exception& Exc) {
    return true;
  }
}
