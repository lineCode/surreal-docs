/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <spdlog/sinks/sink.h>
#include <spdlog/logger.h>
#include <string>
#include <vector>
#include <memory>
#include "udocs-processor/cli/ue/UnrealInteraction.h"

namespace udocs_processor {
class BpPreparer {
 public:
  struct PrepareRequest {
    // absolute paths
    std::string OutDirectory;
    std::string ProjectDirectory;
    std::vector<std::string> ContentInclude;
    std::vector<std::string> ContentExclude;
    std::vector<std::string> NativeInclude;
    std::vector<std::string> NativeExclude;
    UnrealInteraction::UnrealVersion Version;
    std::string Target;
    std::string Configuration;
    bool DoUseCmd = false;
  };

  enum class Status {
    COMPILING,
    EXTRACTING_INFO
  };

  explicit BpPreparer(std::shared_ptr<spdlog::sinks::sink> Sink,
      std::unique_ptr<UnrealInteraction> Ue);

  void Prepare(const PrepareRequest& Request,
      const std::function<void(Status)>& StageCallback);

  // abs dir
  void SetResDirectory(const std::string& ResDirectory);

  void CleanUp(const std::string& OutDirectory) const;

 private:
  std::string MakeBpWd(const std::string& OutDirectory) const;

  static constexpr const char* LOGGER_NAME = "bp-preparer";
  static constexpr const char* BP_WD_NAME = "bp_wd";
  static constexpr const char* PROTO_UDATA_CONTENT_INCLUDE_PATH =
      "/extraction/filters/content/include";
  static constexpr const char* PROTO_UDATA_CONTENT_EXCLUDE_PATH =
      "/extraction/filters/content/exclude";
  static constexpr const char* PROTO_UDATA_NATIVE_INCLUDE_PATH =
      "/extraction/filters/native_modules/include";
  static constexpr const char* PROTO_UDATA_NATIVE_EXCLUDE_PATH =
      "/extraction/filters/native_modules/exclude";
  static constexpr const char* PROTO_UDATA_OUTPUT_PATH =
      "/extraction/output/dir";
  static constexpr const char* REQUEST = "request.json";
  static constexpr const char* EDITOR_COMMANDLET = "-run=ExtractUData";
  static constexpr const char* EDITOR_REQUEST_FILE = "-request-file=";
  static constexpr const char* PROTO_UDATA_REQUEST =
      "misc/util/udocs_processor/UDataRequest.json";
  static constexpr char DIRECTORY_SEPARATOR = '/';
  static constexpr const char* QUOTE = "\"";

  std::string ResDirectory;
  std::shared_ptr<spdlog::logger> l;
  std::unique_ptr<UnrealInteraction> Ue;
};
}  // namespace udocs_processor
