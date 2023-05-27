/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include <string>
#include "udocs-processor/doctree/DocNodeVisitor.h"

namespace udocs_processor {
class ProcessingReport;

class Subprocessor : public DocNodeVisitor {
 public:
  void SetRoot(std::shared_ptr<DocNode> Root);

  std::shared_ptr<DocNode> GetRoot();

  void SetReport(ProcessingReport* Report);

  ProcessingReport* GetReport();

  virtual bool Before();

  virtual void After();

  virtual std::string_view GetName() const;

 private:
  constexpr static char SUBPROCESSOR_NAME[] = "<Unnamed Subprocessor>";

  std::shared_ptr<DocNode> Root;
  ProcessingReport* Report = nullptr;
};
}  // namespace udocs_processor
