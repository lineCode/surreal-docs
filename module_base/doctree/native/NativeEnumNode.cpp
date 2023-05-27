/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/native/NativeFunctionNode.h"
#include "udocs-processor/doctree/native/NativeEnumNode.h"
#include "udocs-processor/doctree/blueprints/BlueprintEnumNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"

std::weak_ptr<udocs_processor::BlueprintEnumNode>
udocs_processor::NativeEnumNode::GetBlueprintCounterpart() const {
  return BlueprintCounterpart;
}

void udocs_processor::NativeEnumNode::SetBlueprintCounterpart(
    std::weak_ptr<BlueprintEnumNode> BlueprintCounterpart) {
  this->BlueprintCounterpart = BlueprintCounterpart;
}

bool udocs_processor::NativeEnumNode::IsStrong() const {
  return mIsStrong;
}

void udocs_processor::NativeEnumNode::SetIsStrong(bool IsStrong) {
  mIsStrong = IsStrong;
}

int udocs_processor::NativeEnumNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

udocs_processor::NativeEnumNode::NativeEnumNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : NativeMemberNode(Parent, Id) {}

bool udocs_processor::NativeEnumNode::IsLegacyUeEnum() const {
  return mIsLegacyUeEnum;
}

void udocs_processor::NativeEnumNode::SetIsLegacyUeEnum(bool IsLegacyUeEnum) {
  mIsLegacyUeEnum = IsLegacyUeEnum;
}

std::weak_ptr<udocs_processor::DocNode>
    udocs_processor::NativeEnumNode::GetCounterpart() const {
  return BlueprintCounterpart;
}
