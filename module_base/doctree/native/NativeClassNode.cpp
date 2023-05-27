/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/native/NativeClassNode.h"
#include "udocs-processor/doctree/blueprints/BlueprintClassNode.h"
#include "udocs-processor/doctree/blueprints/NativeClassCounterpart.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"

std::weak_ptr<udocs_processor::NativeClassCounterpart>
  udocs_processor::NativeClassNode::GetBlueprintCounterpart() const {
  return BlueprintCounterpart;
}

void udocs_processor::NativeClassNode::SetBlueprintCounterpart(
    std::weak_ptr<NativeClassCounterpart> BlueprintCounterpart) {
  this->BlueprintCounterpart = BlueprintCounterpart;
}

udocs_processor::NativeClassType
  udocs_processor::NativeClassNode::GetType() const {
  return Type;
}

void udocs_processor::NativeClassNode::SetType(NativeClassType Type) {
  this->Type = Type;
}

std::weak_ptr<udocs_processor::NativeClassNode>
udocs_processor::NativeClassNode::GetInterfaceComplementary() const {
  return InterfaceComplementary;
}

void udocs_processor::NativeClassNode::SetInterfaceComplementary(
    std::weak_ptr<NativeClassNode> InterfaceComplementary) {
  this->InterfaceComplementary = InterfaceComplementary;
}

bool udocs_processor::NativeClassNode::IsFinal() const {
  return mIsFinal;
}

void udocs_processor::NativeClassNode::SetIsFinal(bool IsFinal) {
  mIsFinal = IsFinal;
}

bool udocs_processor::NativeClassNode::IsTemplate() const {
  return mIsTemplate;
}

void udocs_processor::NativeClassNode::SetIsTemplate(bool IsTemplate) {
  mIsTemplate = IsTemplate;
}

int udocs_processor::NativeClassNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

udocs_processor::NativeClassNode::NativeClassNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : NativeMemberNode(Parent, Id) {}

std::weak_ptr<udocs_processor::DocNode>
    udocs_processor::NativeClassNode::GetCounterpart() const {
  return BlueprintCounterpart;
}
