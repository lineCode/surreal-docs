/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/docs/DocsElementNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"

const std::string &udocs_processor::DocsElementNode::GetFileReference() const {
  return FileReference;
}

void udocs_processor::DocsElementNode::SetFileReference(
    std::string FileReference) {
  this->FileReference = std::move(FileReference);
}

const std::string &udocs_processor::DocsElementNode::GetDocPath() const {
  return DocPath;
}

void udocs_processor::DocsElementNode::SetDocPath(std::string DocPath) {
  this->DocPath = std::move(DocPath);
}

udocs_processor::DocsElementNode::DocsElementNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : DocNode(Parent, Id) {}

int udocs_processor::DocsElementNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}
