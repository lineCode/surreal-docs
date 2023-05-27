/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include "BlueprintTypeNode.h"
#include "udocs-processor/doctree/HasCounterpart.h"

namespace udocs_processor {
class DelegateTypeNode;

class BlueprintDelegateTypeNode : public BlueprintTypeNode,
    public HasCounterpart {
 public:
  BlueprintDelegateTypeNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  std::weak_ptr<DocNode> GetCounterpart() const override;

  std::weak_ptr<DelegateTypeNode> GetNativeCounterpart() const;

  void SetNativeCounterpart(std::weak_ptr<DelegateTypeNode> NativeCounterpart);

  bool IsMulticast() const;

  void SetIsMulticast(bool IsMulticast);

  int Accept(DocNodeVisitor &Visitor) override;

  ~BlueprintDelegateTypeNode() override = default;

 private:
  std::weak_ptr<DelegateTypeNode> NativeCounterpart;
  bool mIsMulticast = false;
};
}  // namespace udocs_processor
