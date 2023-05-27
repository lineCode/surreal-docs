/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <string>
#include <cstdint>
#include "udocs-processor/ApiStatus.h"

namespace udocs_processor {
class ProductService {
 public:
  struct LatestVersionResponse {
    ApiStatus Status;
    uint64_t Major = 0;
    uint64_t Minor = 0;
    uint64_t Index = 0;
    uint64_t BuildNumber = 0;
    std::string Version;
  };

  virtual LatestVersionResponse GetLatestVersion(
      const std::string& ProductId) = 0;
};
}  // namespace udocs_processor
