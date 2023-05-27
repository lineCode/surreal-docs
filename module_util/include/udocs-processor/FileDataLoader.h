/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <spdlog/spdlog.h>
#include <string>
#include <vector>
#include <filesystem>
#include <memory>

namespace udocs_processor {
class FileDataLoader {
 public:
  explicit FileDataLoader(std::shared_ptr<spdlog::logger> Logger);

  // directory where docs are contained
  void SetWorkingDirectory(std::string WorkingDirectory);

  const std::string &GetWorkingDirectory() const;

  // checks if path is inside WorkingDirectory
  virtual bool DoesExistAndUnderWorkingDirectory(const std::string& Path) const;

  // caller is responsible for disposal of resources
  virtual char * LoadBytes(
      const std::string& Path, size_t Count, size_t& Loaded) const;

  // caller is responsible for disposal of resources
  virtual char * LoadWholeBinaryFile(
      const std::string& Path, size_t& Loaded) const;

  virtual size_t LoadSizeOfFile(const std::string& Path) const;

  virtual std::string LoadText(const std::string& Path) const;

  virtual std::vector<std::string> FindAllFiles(
      const std::string& Path, bool Recursively = false);

  virtual std::string ToAbsolute(const std::string& RelativeToWd) const;

  virtual bool IsFile(const std::string& RelativeToWd) const;

  virtual bool WriteBytes(const std::string& Path,
      const char* Data, size_t Size) const;

  // Path - either absolute or relative to WD
  virtual std::string ToRelative(const std::string& Path) const;

  virtual std::string FileName(const std::string& Path,
      bool IncludeExtenstion) const;

  virtual std::string Directory(const std::string& Path) const;

  virtual std::string FileExtension(const std::string& Path) const;

 private:
  template<typename T>
  std::vector<std::string> FindAllFiles(const std::string& Path);

  constexpr static char UDOCS_PROCESSOR_LOGGER_NAME[] = "udocs-processor";
  constexpr static char DIRECTORY_SEPARATOR[] = "/";
  constexpr static int DIRECTORY_FILES_RESERVE_COUNT = 10;

  std::string WorkingDirectory;
  std::filesystem::path WorkingDirectoryPath;
  std::shared_ptr<spdlog::logger> l;
};
}  // namespace udocs_processor
