/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <string>
#include "udocs-processor/ApiStatus.h"

#undef ReportTelemetryEvent

namespace udocs_processor {
class TelemetryService {
 public:
  struct TelemetryEvent {
    std::string ClientId;
    std::string EventSource;
    std::string EventName;
    std::string Payload;
  };

  virtual ApiStatus ReportTelemetryEvent(const TelemetryEvent& Event) const = 0;
};
}  // namespace udocs_processor
