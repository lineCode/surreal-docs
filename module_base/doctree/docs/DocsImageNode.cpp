/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/docs/DocsImageNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"

udocs_processor::ImageType udocs_processor::DocsImageNode::GetType() const {
  return Type;
}

void udocs_processor::DocsImageNode::SetType(ImageType Type) {
  this->Type = Type;
}

udocs_processor::DocsImageNode::DocsImageNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : DocsElementNode(Parent, Id) {}

int udocs_processor::DocsImageNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}
