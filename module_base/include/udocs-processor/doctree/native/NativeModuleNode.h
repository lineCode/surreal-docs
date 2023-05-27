/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include "udocs-processor/doctree/native/NativeMemberNode.h"
#include "udocs-processor/doctree/HasCounterpart.h"

namespace udocs_processor {
class BlueprintModuleNode;

class NativeModuleNode : public NativeMemberNode, public HasCounterpart {
 public:
  NativeModuleNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  std::weak_ptr<DocNode> GetCounterpart() const override;

  std::weak_ptr<BlueprintModuleNode> GetBlueprintCounterpart() const;

  void SetBlueprintCounterpart(
      std::weak_ptr<BlueprintModuleNode> BlueprintCounterpart);

  int Accept(DocNodeVisitor &Visitor) override;

  ~NativeModuleNode() override = default;

 private:
  std::weak_ptr<BlueprintModuleNode> BlueprintCounterpart;
};
}  // namespace udocs_processor

