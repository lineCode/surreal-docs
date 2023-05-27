/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include "BlueprintUNode.h"
#include "udocs-processor/doctree/HasCounterpart.h"

namespace udocs_processor {
class NativeEnumNode;

class BlueprintEnumNode : public BlueprintUNode, public HasCounterpart {
 public:
  BlueprintEnumNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  std::weak_ptr<DocNode> GetCounterpart() const override;

  std::weak_ptr<NativeEnumNode> GetNativeCounterpart() const;

  void SetNativeCounterpart(std::weak_ptr<NativeEnumNode> NativeCounterpart);

  bool IsBlueprintType() const;

  void SetIsBlueprintType(bool IsBlueprintType);

  int Accept(DocNodeVisitor &Visitor) override;

  ~BlueprintEnumNode() override = default;

 private:
  std::weak_ptr<NativeEnumNode> NativeCounterpart;
  bool mIsBlueprintType = false;
};
}  // namespace udocs_processor

