/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include "BlueprintUNode.h"
#include "udocs-processor/doctree/HasCounterpart.h"

namespace udocs_processor {
class NativeModuleNode;

class BlueprintModuleNode : public BlueprintUNode, public HasCounterpart {
 public:
  BlueprintModuleNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  std::weak_ptr<DocNode> GetCounterpart() const override;

  std::weak_ptr<NativeModuleNode> GetNativeCounterpart() const;

  void SetNativeCounterpart(std::weak_ptr<NativeModuleNode> NativeCounterpart);

  int Accept(DocNodeVisitor &Visitor) override;

  ~BlueprintModuleNode() override = default;

 private:
  std::weak_ptr<NativeModuleNode> NativeCounterpart;
};
}  // namespace udocs_processor

