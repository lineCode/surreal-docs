/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include <string>
#include <string_view>

namespace udocs_processor {

class IdGenerator;
class DocNode;
class ProcessingReport;

class TreeLoader {
 public:
  virtual void LoadTree(IdGenerator &IdGen, std::weak_ptr<DocNode> Root) = 0;

  void SetReport(ProcessingReport* Report);

  ProcessingReport* GetReport();

  virtual std::string_view GetName() const;

 private:
  constexpr static char TREELOADER_NAME[] = "<Unnamed Tree Loader>";

  ProcessingReport* Report = nullptr;
};
}  // namespace udocs_processor

