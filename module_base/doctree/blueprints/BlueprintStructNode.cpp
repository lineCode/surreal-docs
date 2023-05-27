/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/blueprints/BlueprintStructNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"
#include "udocs-processor/doctree/native/NativeClassNode.h"

bool udocs_processor::BlueprintStructNode::IsBlueprintType() const {
  return mIsBlueprintType;
}

void udocs_processor::BlueprintStructNode::SetIsBlueprintType(
    bool IsBlueprintType) {
  mIsBlueprintType = IsBlueprintType;
}

int udocs_processor::BlueprintStructNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

std::weak_ptr<udocs_processor::NativeClassNode>
udocs_processor::BlueprintStructNode::GetNativeCounterpart() const {
  return NativeCounterpart;
}

void udocs_processor::BlueprintStructNode::SetNativeCounterpart(
    std::weak_ptr<NativeClassNode> NativeCounterpart) {
  this->NativeCounterpart = NativeCounterpart;
}

udocs_processor::BlueprintStructNode::BlueprintStructNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : NativeClassCounterpart(Parent, Id) {}

std::weak_ptr<udocs_processor::DocNode>
    udocs_processor::BlueprintStructNode::GetCounterpart() const {
  return NativeCounterpart;
}
