/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <spdlog/sinks/sink.h>
#include <spdlog/logger.h>
#include <string>
#include <vector>
#include <memory>
#include <inja/inja.hpp>
#include <inja/environment.hpp>
#include <inja/template.hpp>

namespace udocs_processor {
class CppPreparer {
 public:
  struct PrepareRequest {
    // absolute paths
    std::vector<std::string> Inputs;
    std::vector<std::string> Exclude;
    std::string OutDirectory;
  };

  explicit CppPreparer(std::shared_ptr<spdlog::sinks::sink> Sink);

  void Prepare(const PrepareRequest& Request);

  // abs dir
  void SetBinDirectory(const std::string& BinDirectory);

  // abs dir
  void SetResDirectory(const std::string& ResDirectory);

  void CleanUp(const std::string& OutDirectory) const;

 private:
  std::string MakeCppWd(const std::string& OutDirectory) const;

  void PrepareDoxyfile(const PrepareRequest& Request);

  void RunDoxygen(const PrepareRequest& Request);

  void RunDoxybook(const PrepareRequest& Request) const;

  static constexpr const char* CPP_WD_NAME = "cpp_wd";

  static constexpr const char* DOXYFILE_DATA = "d";
  static constexpr const char* DOXYFILE_DATA_INCLUDE = "include";
  static constexpr const char* DOXYFILE_DATA_EXCLUDE = "exclude";
  static constexpr const char* DOXYFILE_DATA_OUTPUT = "output";

  static constexpr const int64_t DOXYGEN_POLL_DELAY_MS = 1000;  // 1s
  static constexpr const int64_t DOXYBOOK_POLL_DELAY_MS = 1000;  // 1s
  static constexpr const int64_t DOXYGEN_TIMEOUT_MS = 1000;  // 1s
  static constexpr const char* DOXYFILE = "misc/util/udocs_processor/Doxyfile";
  static constexpr const char* DOXYFILE_NAME = "Doxyfile";
  static constexpr const char* DOXYGEN_EXECUTABLE = "thirdparty/doxygen.exe";
  static constexpr const char* DOXYGEN_XML_DIR = "xml";
  static constexpr const int64_t DOXYBOOK2_TIMEOUT_MS = 1000;  // 1s
  static constexpr const char* DOXYBOOK2_EXECUTABLE =
      "thirdparty/doxybook2.exe";
  static constexpr const char* DOXYBOOK2_INPUT = "-i";
  static constexpr const char* DOXYBOOK2_OUTPUT = "-o";
  static constexpr const char* DOXYBOOK_OUTPUT_DIR = "json";
  static constexpr const char* DOXYBOOK2_J = "-j";

  static constexpr char DIRECTORY_SEPARATOR = '/';
  static constexpr const char* NEWLINE = "\n";

  static constexpr const char* LOGGER_NAME = "cpp-preparer";

  std::string BinDirectory;
  std::string ResDirectory;
  std::shared_ptr<spdlog::logger> l;
  inja::Environment Env;
};
}  // namespace udocs_processor
