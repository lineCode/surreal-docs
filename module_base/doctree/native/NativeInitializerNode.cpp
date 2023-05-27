/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/native/NativeInitializerNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"

int udocs_processor::NativeInitializerNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

udocs_processor::NativeInitializerNode::NativeInitializerNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : DocNode(Parent, Id) {}
