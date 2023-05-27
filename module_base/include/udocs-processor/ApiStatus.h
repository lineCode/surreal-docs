/* Copyright © 2022, Medelför OÜ. All rights reserved. */

#pragma once

#include <cstdint>
#include <string>

namespace udocs_processor {
class ApiStatus {
 public:
  ApiStatus() = default;

  ApiStatus(int32_t Code, std::string Message);

  int32_t GetCode() const;

  const std::string &GetMessageDescription() const;

  static const int32_t SUCCESS = 0;
  static const int32_t INTERNAL_ERROR = 1;
  static const int32_t FAILED_VALIDATION = 2;
  static const int32_t FAILED_PROCESSING = 3;
  static const int32_t UNAUTHORIZED = 4;
  static const int32_t GRPC_LAYER_FAILED = 5;

 private:
  int32_t Code = 0;
  std::string Message;
};
}  // namespace udocs_processor
