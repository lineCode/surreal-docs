/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/blueprints/types/BlueprintObjectTypeNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"

udocs_processor::ObjectReference
  udocs_processor::BlueprintObjectTypeNode::GetObjectReference() const {
  return Reference;
}

void udocs_processor::BlueprintObjectTypeNode::SetObjectReference(
    ObjectReference Reference) {
  this->Reference = Reference;
}

udocs_processor::ClassType
  udocs_processor::BlueprintObjectTypeNode::GetClassType() const {
  return Type;
}

void udocs_processor::BlueprintObjectTypeNode::SetClassType(
    ClassType Type) {
  this->Type = Type;
}

int udocs_processor::BlueprintObjectTypeNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

udocs_processor::BlueprintObjectTypeNode::BlueprintObjectTypeNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : BlueprintTypeNode(Parent, Id) {}
