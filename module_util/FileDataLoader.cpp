/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include <fstream>
#include <filesystem>
#include <algorithm>
#include <sstream>
#include "udocs-processor/FileDataLoader.h"

void udocs_processor::FileDataLoader::SetWorkingDirectory(
    std::string WorkingDirectory) {
  WorkingDirectoryPath = std::filesystem::canonical(WorkingDirectory);
  this->WorkingDirectory = std::move(WorkingDirectory);
}

udocs_processor::FileDataLoader::FileDataLoader(
    std::shared_ptr<spdlog::logger> Logger) : l(Logger) {}

char * udocs_processor::FileDataLoader::LoadBytes(
    const std::string &Path, size_t Count, size_t& Loaded) const {
  std::ifstream File(WorkingDirectoryPath / Path,
      std::ios::binary | std::ios::ate);

  if (File.good()) {
    size_t Size = File.tellg();
    size_t ToActuallyLoad = Size > Count ? Count : Size;
    Loaded = ToActuallyLoad;
    File.seekg(0);

    char* Data = new char[ToActuallyLoad];

    File.read(Data, ToActuallyLoad);

    return Data;
  }

  return nullptr;
}

char * udocs_processor::FileDataLoader::LoadWholeBinaryFile(
    const std::string &Path, size_t& Loaded) const {
  Loaded = 0;

  std::ifstream File(WorkingDirectoryPath / Path,
      std::ios::binary | std::ios::ate);

  if (File.good()) {
    std::streampos Size = File.tellg();
    char* Buffer = new char[Size];
    File.seekg(0, std::ios::beg);
    File.read(Buffer, Size);
    Loaded = Size;

    return Buffer;
  } else {
    l->warn("File `{}` doesn't exist. Couldn't retrieve text content", Path);
  }

  return nullptr;
}

size_t udocs_processor::FileDataLoader::LoadSizeOfFile(
    const std::string &Path) const {
  std::filesystem::path FilePath(WorkingDirectoryPath / Path);
  if (std::filesystem::exists(FilePath) &&
      std::filesystem::is_regular_file(FilePath)) {
    std::ifstream File(FilePath.string(),
        std::ios::binary | std::ios::ate);
    // no need to check good(), -1u will be returned in case of failure
    return File.tellg();
  } else {
    l->warn("File `{}` doesn't exist. Couldn't retrieve file size", Path);
  }

  return 0;
}

std::string udocs_processor::FileDataLoader::LoadText(
    const std::string &Path) const {
  std::ifstream File(WorkingDirectoryPath / Path);

  if (File.good()) {
    std::stringstream Buffer;
    Buffer << File.rdbuf();

    return Buffer.str();
  } else {
    l->warn("File `{}` doesn't exist. Couldn't retrieve text content", Path);
  }

  return {};
}

std::vector<std::string> udocs_processor::FileDataLoader::FindAllFiles(
    const std::string &Path, bool Recursively) {
  if (Recursively) {
    return FindAllFiles<std::filesystem::recursive_directory_iterator>(Path);
  } else {
    return FindAllFiles<std::filesystem::directory_iterator>(Path);
  }
}

template<typename T>
std::vector<std::string> udocs_processor::FileDataLoader::FindAllFiles(
    const std::string& Path) {
  std::vector<std::string> Result;
  std::filesystem::path TruePath = WorkingDirectoryPath / Path;

  if (std::filesystem::exists(TruePath)) {
    T It(TruePath);

    Result.reserve(DIRECTORY_FILES_RESERVE_COUNT);

    for (const auto& File : It) {
      if (File.is_regular_file()) {
        std::filesystem::path RelativeToWd =
            std::filesystem::relative(File.path(), WorkingDirectoryPath);
        Result.emplace_back(RelativeToWd.string());
      }
    }
  } else {
    l->warn("File `{}` doesn't exist. Couldn't find all files", Path);
  }

  return Result;
}

bool udocs_processor::FileDataLoader::DoesExistAndUnderWorkingDirectory(
    const std::string &Path) const {
  std::filesystem::path TruePath = WorkingDirectoryPath / Path;
  if (std::filesystem::exists(TruePath)) {
    TruePath = std::filesystem::canonical(TruePath);

    auto It = std::search(TruePath.begin(), TruePath.end(),
      WorkingDirectoryPath.begin(), WorkingDirectoryPath.end());

    return It == TruePath.begin();
  } else {
    l->warn("File `{}` doesn't exist. Couldn't check if is under WD", Path);
  }

  return false;
}

const std::string &
    udocs_processor::FileDataLoader::GetWorkingDirectory() const {
  return WorkingDirectory;
}

std::string udocs_processor::FileDataLoader::ToAbsolute(
    const std::string &RelativeToWd) const {
  std::filesystem::path Path =
      std::filesystem::path(WorkingDirectory) / RelativeToWd;
  return std::filesystem::canonical(Path).string();
}

bool udocs_processor::FileDataLoader::IsFile(
    const std::string& RelativeToWd) const {
  std::filesystem::path Path =
      std::filesystem::path(WorkingDirectory) / RelativeToWd;

  return std::filesystem::is_regular_file(Path);
}

bool udocs_processor::FileDataLoader::WriteBytes(
    const std::string &Path, const char *Data, size_t Size) const {
  // todo(artsiom.drapun): a bug! the method should take into account current WD
  std::filesystem::path FsPath(Path);
  std::filesystem::create_directories(FsPath.parent_path());
  std::ofstream Stream(Path, std::ios::binary | std::ios::trunc);

  if (Stream.good()) {
    Stream.write(Data, Size);

    return true;
  } else {
    l->error("Unable to write bytes to file `{}`", Path);
  }

  return false;
}

std::string udocs_processor::FileDataLoader::ToRelative(
    const std::string& Path) const {
  std::filesystem::path RelativeToWd =
      std::filesystem::relative(WorkingDirectoryPath / Path,
          WorkingDirectoryPath);

  return RelativeToWd.string();
}

std::string udocs_processor::FileDataLoader::FileName(
    const std::string &Path, bool IncludeExtenstion) const {
  std::filesystem::path lPath(Path);

  return IncludeExtenstion ? lPath.filename().string() : lPath.stem().string();
}

std::string udocs_processor::FileDataLoader::Directory(
    const std::string &Path) const {
  return std::filesystem::path(Path).parent_path().string();
}

std::string udocs_processor::FileDataLoader::FileExtension(
    const std::string &Path) const {
  return std::filesystem::path(Path).extension().string();
}
