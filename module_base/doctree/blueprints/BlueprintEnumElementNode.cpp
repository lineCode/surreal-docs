/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/blueprints/BlueprintEnumElementNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"

bool udocs_processor::BlueprintEnumElementNode::IsHidden() const {
  return mIsHidden;
}

void udocs_processor::BlueprintEnumElementNode::SetIsHidden(bool IsHidden) {
  mIsHidden = IsHidden;
}

int udocs_processor::BlueprintEnumElementNode::Accept(
    DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

udocs_processor::BlueprintEnumElementNode::BlueprintEnumElementNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : BlueprintUNode(Parent, Id) {}
