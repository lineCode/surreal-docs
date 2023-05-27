/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/SeeNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"

int udocs_processor::SeeNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

udocs_processor::SeeNode::SeeNode(std::weak_ptr<DocNode> Parent, uint64_t Id)
  : DocNode(Parent, Id) {}
