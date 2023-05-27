/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include <string>
#include "TelemetryReporter.h"

namespace udocs_processor {
class BasicTelemetry {
 public:
  explicit BasicTelemetry(std::shared_ptr<TelemetryReporter> Reporter);

  void ReportInit(std::string Command);

  void ReportFail(std::string Command, std::string ErrorMessage);

  void ReportFinish(std::string Command);

 private:
  static constexpr const char* EVENT_INIT = "init";
  static constexpr const char* EVENT_FAIL = "fail";
  static constexpr const char* EVENT_FINISH = "finish";

  static constexpr const char* PAYLOAD_VERSION = "version";
  static constexpr const char* PAYLOAD_COMMAND = "command";
  static constexpr const char* PAYLOAD_ERROR_MESSAGE = "error";

  std::string Version;

  std::shared_ptr<TelemetryReporter> Reporter;
};
}  // namespace udocs_processor
