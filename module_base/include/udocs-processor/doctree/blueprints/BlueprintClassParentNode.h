/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <string>
#include <memory>
#include "udocs-processor/doctree/AccessModifier.h"
#include "BlueprintUNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"

namespace udocs_processor {
class BlueprintClassParentNode : public BlueprintUNode {
 public:
  BlueprintClassParentNode(std::weak_ptr<DocNode> Parent, uint64_t Id)
      : BlueprintUNode(Parent, Id) {}

  std::weak_ptr<BlueprintClassNode> GetNode() const {
    return Node;
  }

  void SetNode(std::weak_ptr<BlueprintClassNode> Node) {
    this->Node = Node;
  }

  bool IsInterface() const {
    return mIsInterface;
  }

  void SetIsInterface(bool IsInterface) {
    mIsInterface = IsInterface;
  }

  int Accept(DocNodeVisitor &Visitor) override {
    return Visitor.Visit(*this);
  }

  ~BlueprintClassParentNode() override = default;

 private:
  std::weak_ptr<BlueprintClassNode> Node;
  bool mIsInterface = false;
};
}  // namespace udocs_processor
