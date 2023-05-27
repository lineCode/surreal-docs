/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <string>
#include <memory>
#include "udocs-processor/doctree/AccessModifier.h"
#include "NativeMemberNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"

namespace udocs_processor {
class ClassParentNode : public NativeMemberNode {
 public:
  ClassParentNode(std::weak_ptr<DocNode> Parent, uint64_t Id)
      : NativeMemberNode(Parent, Id) {}

  bool IsVirtual() const {
    return mIsVirtual;
  }

  void SetIsVirtual(bool IsVirtual) {
    mIsVirtual = IsVirtual;
  }

  std::weak_ptr<NativeClassNode> GetNode() const {
    return Node;
  }

  void SetNode(std::weak_ptr<NativeClassNode> Node) {
    this->Node = Node;
  }

  int Accept(DocNodeVisitor &Visitor) override {
    return Visitor.Visit(*this);
  }

  ~ClassParentNode() override = default;

 private:
  bool mIsVirtual = false;
  std::weak_ptr<NativeClassNode> Node;
};
}  // namespace udocs_processor

