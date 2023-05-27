/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/blueprints/types/BlueprintPrimitiveTypeNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"

udocs_processor::PrimitiveType::PrimitiveType
  udocs_processor::BlueprintPrimitiveTypeNode::GetPrimitiveType() const {
  return PrimitiveType;
}

void udocs_processor::BlueprintPrimitiveTypeNode::SetPrimitiveType(
    PrimitiveType::PrimitiveType PrimitiveType) {
  this->PrimitiveType = PrimitiveType;
}

int udocs_processor::BlueprintPrimitiveTypeNode::Accept(
    DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

udocs_processor::BlueprintPrimitiveTypeNode::BlueprintPrimitiveTypeNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : BlueprintTypeNode(Parent, Id) {}
