/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <spdlog/logger.h>
#include <memory>

namespace udocs_processor {
class DocNode;
class ProcessingReport;
class GraphStyle;

class DocTreeSerializer {
 public:
  virtual void Serialize(DocNode &Root) = 0;

  void SetReport(ProcessingReport* Report);

  ProcessingReport* GetReport();

  virtual void SetLogger(std::shared_ptr<spdlog::logger> Logger) const = 0;

 private:
  ProcessingReport* Report = nullptr;
};
}  // namespace udocs_processor

