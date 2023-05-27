/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/native/NativeEnumElementNode.h"
#include <utility>
#include "udocs-processor/doctree/DocNodeVisitor.h"

int udocs_processor::NativeEnumElementNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

udocs_processor::NativeEnumElementNode::NativeEnumElementNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : NativeMemberNode(Parent, Id) {}
