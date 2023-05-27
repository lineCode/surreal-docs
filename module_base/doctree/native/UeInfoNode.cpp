/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/native/UeInfoNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"

udocs_processor::UeInfoType udocs_processor::UeInfoNode::GetType() const {
  return Type;
}

void udocs_processor::UeInfoNode::SetType(UeInfoType Type) {
  this->Type = Type;
}

const std::string &
  udocs_processor::UeInfoNode::GetSpecifierName() const {
  return SpecifierName;
}

void udocs_processor::UeInfoNode::SetSpecifierName(
    std::string SpecifierName) {
  this->SpecifierName = SpecifierName;
}

const std::string &
  udocs_processor::UeInfoNode::GetSpecifierValue() const {
  return SpecifierValue;
}

void udocs_processor::UeInfoNode::SetSpecifierValue(
    std::string SpecifierValue) {
  this->SpecifierValue = SpecifierValue;
}

void udocs_processor::UeInfoNode::CopyInfoTo(UeInfoNode &Node) const {
  DocNode& BaseNode = Node;
  DocNode::CopyInfoTo(BaseNode);

  Node.Type = Type;
  Node.SpecifierName = SpecifierName;
  Node.SpecifierValue = SpecifierValue;
}

int udocs_processor::UeInfoNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

udocs_processor::UeInfoNode::UeInfoNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : DocNode(Parent, Id) {}
