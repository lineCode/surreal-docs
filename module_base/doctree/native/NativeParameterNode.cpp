/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/native/NativeParameterNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"

int udocs_processor::NativeParameterNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

udocs_processor::NativeParameterNode::NativeParameterNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : NativeMemberNode(Parent, Id) {}
