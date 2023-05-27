/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <cstdint>
#include <memory>

namespace udocs_processor {
class IdGenerator {
 public:
  explicit IdGenerator(std::shared_ptr<spdlog::sinks::sink> Sink);

  uint64_t ObtainId();

 private:
  uint64_t LastId = ~0;
  std::shared_ptr<spdlog::sinks::sink> Sink;
  std::shared_ptr<spdlog::logger> l;
  static constexpr char* LOGGER_NAME = "udocs-processor";
};
}  // namespace udocs_processor

