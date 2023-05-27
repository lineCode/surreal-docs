/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include <fmt/format.h>
#include "udocs-processor/telemetry/grpcTelemetryService.h"

udocs_processor::ApiStatus
    udocs_processor::grpcTelemetryService::ReportTelemetryEvent(
    const TelemetryService::TelemetryEvent &Event) const {
  grpc::ClientContext Context;
  api::telemetry::TelemetryEvent Request;
  Request.set_clientid(Event.ClientId);
  Request.set_eventname(Event.EventName);
  Request.set_eventsource(Event.EventSource);
  Request.set_payload(Event.Payload);

  api::Status Response;
  grpc::Status Status = Telemetry->ReportTelemetryEvent(
      &Context, Request, &Response);
  if (!Status.ok()) {
    return {ApiStatus::GRPC_LAYER_FAILED,
        fmt::format("Couldn't process RPC call {}", Status.error_code())};
  }

  return {Response.code(), Response.message()};
}

udocs_processor::grpcTelemetryService::grpcTelemetryService(
    std::shared_ptr<grpc::Channel> Channel) {
  Telemetry = std::make_unique<api::telemetry::Telemetry::Stub>(Channel);
}
