/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <string>
#include <memory>
#include "udocs-processor/doctree/DocNode.h"

namespace udocs_processor {
// describes everything what is accessible in Blueprint
class BlueprintUNode : public DocNode {
 public:
  BlueprintUNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  const std::string &GetDisplayName() const;

  void SetDisplayName(std::string DisplayName);

  const std::string &GetCategory() const;

  void SetCategory(std::string Category);

  bool IsNative() const;

  void SetIsNative(bool IsNative);

  BlueprintUNode& operator=(BlueprintUNode&& Node) noexcept;

  int Accept(DocNodeVisitor &Visitor) override;

  ~BlueprintUNode() override = default;

 private:
  std::string DisplayName;
  std::string Category;
  bool mIsNative = false;
};
}  // namespace udocs_processor
