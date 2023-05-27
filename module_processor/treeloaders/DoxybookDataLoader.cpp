/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include <fstream>
#include "udocs-processor/treeloaders/DoxybookDataLoader.h"

void udocs_processor::DoxybookDataLoader::SetWorkingDirectory(
    std::string WorkingDirectory) {
  this->WorkingDirectory = std::move(WorkingDirectory);
}

nlohmann::json udocs_processor::DoxybookDataLoader::LoadManifest() {
  nlohmann::json manifest = LoadChunk(MANIFEST_FILENAME);

  if (manifest.empty()) {
    l->warn("Empty manifest loaded. Maybe the file is missing");
  } else {
    l->info("Manifest loaded successfully");
  }

  return manifest;
}

nlohmann::json udocs_processor::DoxybookDataLoader::LoadChunk(
    const std::string &ChunkName) {
  std::string chunkPath = WorkingDirectory + DIRECTORY_SEPARATOR +
      ChunkName + CHUNK_EXTENSION;
  std::ifstream chunkStream(chunkPath);
  nlohmann::json chunk;

  if (!chunkStream.good()) {
    l->error("Couldn't open chunk file `{}`", chunkPath);
  } else {
    chunk = nlohmann::json::parse(chunkStream);
  }

  if (chunk.empty()) {
    l->warn("Empty chunk loaded. Maybe the file is missing");
  } else {
    l->info("The chunk `{}` loaded successfully", ChunkName);
  }

  return chunk;
}

udocs_processor::DoxybookDataLoader::DoxybookDataLoader() {
  l = spdlog::get(UDOCS_PROCESSOR_LOGGER_NAME);
}
