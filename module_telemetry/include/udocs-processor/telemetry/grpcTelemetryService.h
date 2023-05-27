/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <grpcpp/channel.h>
#include <memory>

#undef ReportTelemetryEvent

#include "TelemetryService.h"
#include "api/telemetry/Telemetry.grpc.pb.h"

namespace udocs_processor {
class grpcTelemetryService : public TelemetryService {
 public:
  explicit grpcTelemetryService(std::shared_ptr<grpc::Channel> Channel);

  ApiStatus ReportTelemetryEvent(const TelemetryEvent &Event) const override;

 private:
  std::unique_ptr<api::telemetry::Telemetry::Stub> Telemetry;
};
}  // namespace udocs_processor
