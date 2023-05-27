/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/blueprints/types/BlueprintEnumTypeNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"

int udocs_processor::BlueprintEnumTypeNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

udocs_processor::BlueprintEnumTypeNode::BlueprintEnumTypeNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : BlueprintTypeNode(Parent, Id) {}
