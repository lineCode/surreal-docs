/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/native/TemplateNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"

int udocs_processor::TemplateNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

udocs_processor::TemplateNode::TemplateNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : DocNode(Parent, Id) {}
