/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/blueprints/BlueprintInitializerNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"

int udocs_processor::BlueprintInitializerNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

udocs_processor::BlueprintInitializerNode::BlueprintInitializerNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : DocNode(Parent, Id) {}
