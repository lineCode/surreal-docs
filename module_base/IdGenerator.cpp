/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include <spdlog/spdlog.h>
#include "udocs-processor/IdGenerator.h"

udocs_processor::IdGenerator::IdGenerator(
    std::shared_ptr<spdlog::sinks::sink> Sink) : Sink(Sink) {
  l = spdlog::get(LOGGER_NAME);

  if (!l) {
    l = std::make_shared<spdlog::logger>(LOGGER_NAME);
    spdlog::register_logger(l);
    if (Sink) {
      l->sinks().emplace_back(Sink);
    }
  }
}

uint64_t udocs_processor::IdGenerator::ObtainId() {
  if (LastId + 1 == 0) {
    l->warn("The id has overflown 64 bit grid, it's 0 again");
  }

  return ++LastId;
}
