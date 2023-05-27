/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/blueprints/BlueprintModuleNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"
#include "udocs-processor/doctree/native/NativeModuleNode.h"

int udocs_processor::BlueprintModuleNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

std::weak_ptr<udocs_processor::NativeModuleNode>
udocs_processor::BlueprintModuleNode::GetNativeCounterpart() const {
  return NativeCounterpart;
}

void udocs_processor::BlueprintModuleNode::SetNativeCounterpart(
    std::weak_ptr<NativeModuleNode> NativeCounterpart) {
  this->NativeCounterpart = NativeCounterpart;
}

udocs_processor::BlueprintModuleNode::BlueprintModuleNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : BlueprintUNode(Parent, Id) {}

std::weak_ptr<udocs_processor::DocNode>
    udocs_processor::BlueprintModuleNode::GetCounterpart() const {
  return NativeCounterpart;
}
