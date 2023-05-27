/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include "NativeMemberNode.h"
#include "udocs-processor/doctree/HasCounterpart.h"
#include "BlueprintableNode.h"

namespace udocs_processor {
class BlueprintEnumNode;

class NativeEnumNode : public NativeMemberNode,
    public HasCounterpart, public BlueprintableNode {
 public:
  NativeEnumNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  std::weak_ptr<DocNode> GetCounterpart() const override;

  std::weak_ptr<BlueprintEnumNode> GetBlueprintCounterpart() const;

  void SetBlueprintCounterpart(
      std::weak_ptr<BlueprintEnumNode> BlueprintCounterpart);

  bool IsStrong() const;

  void SetIsStrong(bool IsStrong);

  bool IsLegacyUeEnum() const;

  void SetIsLegacyUeEnum(bool IsLegacyUeEnum);

  int Accept(DocNodeVisitor &Visitor) override;

  ~NativeEnumNode() override = default;

 private:
  std::weak_ptr<BlueprintEnumNode> BlueprintCounterpart;
  bool mIsStrong = false;
  bool mIsLegacyUeEnum = false;
};
}  // namespace udocs_processor

