/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/blueprints/types/BlueprintDelegateTypeNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"
#include "udocs-processor/doctree/native/DelegateTypeNode.h"

bool udocs_processor::BlueprintDelegateTypeNode::IsMulticast() const {
  return mIsMulticast;
}

void udocs_processor::BlueprintDelegateTypeNode::SetIsMulticast(
    bool IsMulticast) {
  this->mIsMulticast = IsMulticast;
}

int udocs_processor::BlueprintDelegateTypeNode::Accept(
    DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

udocs_processor::BlueprintDelegateTypeNode::BlueprintDelegateTypeNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : BlueprintTypeNode(Parent, Id) {}

std::weak_ptr<udocs_processor::DocNode>
    udocs_processor::BlueprintDelegateTypeNode::GetCounterpart() const {
  return NativeCounterpart;
}

std::weak_ptr<udocs_processor::DelegateTypeNode>
    udocs_processor::BlueprintDelegateTypeNode::GetNativeCounterpart() const {
  return NativeCounterpart;
}

void udocs_processor::BlueprintDelegateTypeNode::SetNativeCounterpart(
    std::weak_ptr<DelegateTypeNode> NativeCounterpart) {
  this->NativeCounterpart = std::move(NativeCounterpart);
}
