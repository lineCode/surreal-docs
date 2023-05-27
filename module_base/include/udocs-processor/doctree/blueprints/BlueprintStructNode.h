/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include "NativeClassCounterpart.h"
#include "udocs-processor/doctree/HasCounterpart.h"

namespace udocs_processor {
class NativeClassNode;

class BlueprintStructNode
    : public NativeClassCounterpart, public HasCounterpart {
 public:
  BlueprintStructNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  std::weak_ptr<DocNode> GetCounterpart() const override;

  std::weak_ptr<NativeClassNode> GetNativeCounterpart() const;

  void SetNativeCounterpart(std::weak_ptr<NativeClassNode> NativeCounterpart);

  bool IsBlueprintType() const;

  void SetIsBlueprintType(bool IsBlueprintType);

  int Accept(DocNodeVisitor &Visitor) override;

  ~BlueprintStructNode() override = default;

 private:
  std::weak_ptr<NativeClassNode> NativeCounterpart;
  bool mIsBlueprintType = false;
};
}  // namespace udocs_processor

