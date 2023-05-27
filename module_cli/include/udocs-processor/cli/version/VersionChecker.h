/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <spdlog/logger.h>
#include <string>
#include <optional>
#include <memory>
#include "ProductService.h"

namespace udocs_processor {
class VersionChecker {
 public:
  VersionChecker(std::shared_ptr<spdlog::sinks::sink> Sink,
      std::string ProductId, uint64_t CurrentMajor,
      uint64_t CurrentMinor, uint64_t CurrentIndex,
      uint64_t CurrentBuildNumber);

  void SetProductService(std::shared_ptr<ProductService> Product);

  void SetCachePath(const std::string& CachePath);

  void UpdateVersion();

  std::optional<std::string> IsOutdated() const;

 private:
  void SaveSelf() const;

  bool LoadSelf();

  std::shared_ptr<ProductService> Product;

  std::string CachePath;
  std::string ProductId;
  uint64_t CurrentMajor = 0;
  uint64_t CurrentMinor = 0;
  uint64_t CurrentIndex = 0;
  uint64_t CurrentBuildNumber = 0;

  static constexpr uint64_t VERSION_POLL_INTERVAL_S = 3600 * 12;  // every 12hr
  static constexpr const char* VERSION_CACHE_FILENAME = "version_cache.json";
  static constexpr const char* VERSION_CACHE_IS_OUTDATED = "is_outdated";
  static constexpr const char* VERSION_CACHE_NEW_VERSION = "latest";
  static constexpr const char* VERSION_CACHE_CHECKED_ON = "checked_on";

  bool IsOutdated_ = false;
  std::string LatestVersion;
  uint64_t LastCheckedOn = 0;

  static constexpr const char* LOGGER_NAME = "version-checker";
  std::shared_ptr<spdlog::logger> l;
};
}  // namespace udocs_processor
