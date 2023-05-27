/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <exec-stream.h>
#include <string>
#include <functional>

namespace udocs_processor {
class ExecStreamHelper {
 public:
  static bool ReadErrLine(std::string& Line, exec_stream_t& ExecStream);

  static bool ReadOutLine(std::string& Line, exec_stream_t& ExecStream);

  static bool IsProcessRunning(exec_stream_t& ExecStream);

 private:
  static constexpr const char* NEWLINE = "\n";
};
}  // namespace udocs_processor
