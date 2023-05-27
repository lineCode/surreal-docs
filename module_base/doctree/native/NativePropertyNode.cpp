/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/native/NativePropertyNode.h"
#include "udocs-processor/doctree/blueprints/BlueprintPropertyNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"

std::weak_ptr<udocs_processor::BlueprintPropertyNode>
    udocs_processor::NativePropertyNode::GetBlueprintCounterpart() const {
  return BlueprintCounterpart;
}

void udocs_processor::NativePropertyNode::SetBlueprintCounterpart(
    std::weak_ptr<BlueprintPropertyNode> BlueprintCounterpart) {
  this->BlueprintCounterpart = BlueprintCounterpart;
}

bool udocs_processor::NativePropertyNode::IsStatic() const {
  return mIsStatic;
}

void udocs_processor::NativePropertyNode::SetIsStatic(bool IsStatic) {
  mIsStatic = IsStatic;
}

bool udocs_processor::NativePropertyNode::IsVolatile() const {
  return mIsVolatile;
}

void udocs_processor::NativePropertyNode::SetIsVolatile(bool IsVolatile) {
  mIsVolatile = IsVolatile;
}

bool udocs_processor::NativePropertyNode::IsMutable() const {
  return mIsMutable;
}

void udocs_processor::NativePropertyNode::SetIsMutable(bool IsMutable) {
  mIsMutable = IsMutable;
}

int udocs_processor::NativePropertyNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

udocs_processor::NativePropertyNode::NativePropertyNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : NativeMemberNode(Parent, Id) {}

bool udocs_processor::NativePropertyNode::IsConst() const {
  return mIsConst;
}

void udocs_processor::NativePropertyNode::SetIsConst(bool IsConst) {
  mIsConst = IsConst;
}

std::weak_ptr<udocs_processor::DocNode>
    udocs_processor::NativePropertyNode::GetCounterpart() const {
  return BlueprintCounterpart;
}
