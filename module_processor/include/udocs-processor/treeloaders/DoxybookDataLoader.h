/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <spdlog/spdlog.h>
#include <string>
#include <memory>
#include <nlohmann/json.hpp>

namespace udocs_processor {
class DoxybookDataLoader {
 public:
  DoxybookDataLoader();

  // directory where manifest.json and all the JSON files are contained
  virtual void SetWorkingDirectory(std::string WorkingDirectory);

  virtual nlohmann::json LoadManifest();

  virtual nlohmann::json LoadChunk(const std::string& ChunkName);

 private:
  constexpr static char UDOCS_PROCESSOR_LOGGER_NAME[] = "udocs-processor";
  constexpr static char DIRECTORY_SEPARATOR[] = "/";

  static constexpr char MANIFEST_FILENAME[] = "manifest";
  static constexpr char CHUNK_EXTENSION[] = ".json";

  std::string WorkingDirectory;
  std::shared_ptr<spdlog::logger> l;
};
}  // namespace udocs_processor
