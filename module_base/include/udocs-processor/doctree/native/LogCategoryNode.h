/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include "NativeMemberNode.h"

#undef ERROR

namespace udocs_processor {
enum class LogVerbosity {
  FATAL,
  ERROR,
  WARNING,
  DISPLAY,
  LOG,
  VERBOSE,
  VERY_VERBOSE,
  ALL
};

enum class LogCategoryType {
  CLASS,
  EXTERN
};

class LogCategoryNode : public NativeMemberNode {
 public:
  LogCategoryNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  LogVerbosity GetRuntimeLevel() const;

  void SetRuntimeLevel(LogVerbosity RuntimeLevel);

  LogVerbosity GetCompileLevel() const;

  void SetCompileLevel(LogVerbosity CompileLevel);

  LogCategoryType GetType() const;

  void SetType(LogCategoryType Type);

  int Accept(DocNodeVisitor &Visitor) override;

  ~LogCategoryNode() override = default;

 private:
  LogCategoryType Type = LogCategoryType::CLASS;
  LogVerbosity RuntimeLevel = LogVerbosity::ALL;
  LogVerbosity CompileLevel = LogVerbosity::ALL;
};
}  // namespace udocs_processor

