/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/blueprints/BlueprintEnumNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"
#include "udocs-processor/doctree/native/NativeEnumNode.h"

std::weak_ptr<udocs_processor::NativeEnumNode>
udocs_processor::BlueprintEnumNode::GetNativeCounterpart() const {
  return NativeCounterpart;
}

void udocs_processor::BlueprintEnumNode::SetNativeCounterpart(
    std::weak_ptr<NativeEnumNode> NativeCounterpart) {
  this->NativeCounterpart = std::move(NativeCounterpart);
}

bool udocs_processor::BlueprintEnumNode::IsBlueprintType() const {
  return mIsBlueprintType;
}

void udocs_processor::BlueprintEnumNode::SetIsBlueprintType(
    bool IsBlueprintType) {
  mIsBlueprintType = IsBlueprintType;
}

int udocs_processor::BlueprintEnumNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

udocs_processor::BlueprintEnumNode::BlueprintEnumNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : BlueprintUNode(Parent, Id) {}

std::weak_ptr<udocs_processor::DocNode>
    udocs_processor::BlueprintEnumNode::GetCounterpart() const {
  return NativeCounterpart;
}
