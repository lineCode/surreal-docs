/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#define NOLINT()

#include <spdlog/spdlog.h>
#include <string>
#include <memory>
#include <vector>
#include <chrono> NOLINT()
#include "subprocessors/Subprocessor.h"
#include "treeloaders/TreeLoader.h"
#include "udocs-processor/serializers/DocTreeSerializer.h"
#include "udocs-processor/report/ProcessingReport.h"

namespace udocs_processor {
class IdGenerator;

class UDocsProcessor {
 public:
  enum class Status {
    LOADING,
    PROCESSING
  };

  explicit UDocsProcessor(IdGenerator &IdGen,
      std::shared_ptr<spdlog::sinks::sink> Sink);

  void AddTreeLoader(std::unique_ptr<TreeLoader> TreeLoader);

  void AddSubprocessor(std::unique_ptr<Subprocessor> Subprocessor);

  void SetDocTreeSerializer(
      std::unique_ptr<DocTreeSerializer> DocTreeSerializer);

  void Process();

  const ProcessingReport& GetReport() const;

  void SetStatusReporter(std::function<void(Status)> ReportCallback);

 private:
  std::string FormatTime(std::chrono::milliseconds Duration) const;

  std::vector<std::unique_ptr<TreeLoader>> TreeLoaders;
  std::vector<std::unique_ptr<Subprocessor>> Subprocessors;
  std::unique_ptr<DocTreeSerializer> SelectedDocTreeSerializer;
  std::unique_ptr<ProcessingReport> Report;

  std::shared_ptr<spdlog::logger> l;

  IdGenerator &IdGen;
  std::function<void(Status)> ReportCallback;
};
}  // namespace udocs_processor

