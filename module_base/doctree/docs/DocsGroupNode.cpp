/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/docs/DocsGroupNode.h"

#include <utility>
#include "udocs-processor/doctree/DocNodeVisitor.h"

const std::string &udocs_processor::DocsGroupNode::GetTitle() const {
  return Title;
}

void udocs_processor::DocsGroupNode::SetTitle(std::string Title) {
  this->Title = std::move(Title);
}

udocs_processor::DocsGroupNode::DocsGroupNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : DocNode(Parent, Id) {}

int udocs_processor::DocsGroupNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}
