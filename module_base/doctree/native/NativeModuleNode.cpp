/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/native/NativeModuleNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"
#include "udocs-processor/doctree/blueprints/BlueprintModuleNode.h"

int udocs_processor::NativeModuleNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

udocs_processor::NativeModuleNode::NativeModuleNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : NativeMemberNode(Parent, Id) {}

std::weak_ptr<udocs_processor::BlueprintModuleNode>
    udocs_processor::NativeModuleNode::GetBlueprintCounterpart() const {
  return BlueprintCounterpart;
}

void udocs_processor::NativeModuleNode::SetBlueprintCounterpart(
    std::weak_ptr<BlueprintModuleNode> BlueprintCounterpart) {
  this->BlueprintCounterpart = BlueprintCounterpart;
}

std::weak_ptr<udocs_processor::DocNode>
    udocs_processor::NativeModuleNode::GetCounterpart() const {
  return BlueprintCounterpart;
}
