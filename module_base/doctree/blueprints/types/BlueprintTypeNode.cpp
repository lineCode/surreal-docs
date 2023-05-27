/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/blueprints/types/BlueprintTypeNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"

bool udocs_processor::BlueprintTypeNode::IsReference() const {
  return mIsReference;
}

void udocs_processor::BlueprintTypeNode::SetIsReference(bool IsReference) {
  this->mIsReference = IsReference;
}

bool udocs_processor::BlueprintTypeNode::IsConst() const {
  return mIsConst;
}

void udocs_processor::BlueprintTypeNode::SetIsConst(bool IsConst) {
  this->mIsConst = IsConst;
}

int udocs_processor::BlueprintTypeNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

udocs_processor::BlueprintTypeNode::BlueprintTypeNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : BlueprintUNode(Parent, Id) {}

udocs_processor::BlueprintTypeNode &
    udocs_processor::BlueprintTypeNode::operator=(
    udocs_processor::BlueprintTypeNode &&Node) noexcept {
  mIsConst = Node.mIsConst;
  mIsReference = Node.mIsReference;

  BlueprintUNode::operator=(std::move(Node));

  return *this;
}
