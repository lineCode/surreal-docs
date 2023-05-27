/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#define NOLINT()

#include <spdlog/spdlog.h>
#include <spdlog/sinks/base_sink.h>
#include <mutex> NOLINT()
#include "udocs-processor/report/ProcessingReport.h"

namespace udocs_processor {
template<typename Mutex>
class ReportSpdLogSink : public spdlog::sinks::base_sink<Mutex> {
 public:
  explicit ReportSpdLogSink(ProcessingReport& Report) : ReportToSink(Report) {}

 protected:
  void sink_it_(const spdlog::details::log_msg& Message) override {
    spdlog::memory_buf_t formatted;
    spdlog::sinks::base_sink<Mutex>::formatter_->format(Message, formatted);

    ReportToSink.AddUnstructuredLogMessage(fmt::to_string(formatted));
  }

  void flush_() override {}

 private:
  ProcessingReport& ReportToSink;
};
}  // namespace udocs_processor

using ReportSpdLogSink_mt = udocs_processor::ReportSpdLogSink<std::mutex>;
using ReportSpdLogSink_st =
    udocs_processor::ReportSpdLogSink<spdlog::details::null_mutex>;

