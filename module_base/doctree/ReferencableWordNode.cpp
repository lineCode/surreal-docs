/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/ReferencableWordNode.h"
#include <utility>
#include "udocs-processor/doctree/DocNodeVisitor.h"

const std::string &udocs_processor::ReferencableWordNode::GetWord() const {
  return Word;
}

void udocs_processor::ReferencableWordNode::SetWord(std::string Word) {
  this->Word = std::move(Word);
}

const std::weak_ptr<udocs_processor::DocNode> &
    udocs_processor::ReferencableWordNode::GetReference() const {
  return Reference;
}

void udocs_processor::ReferencableWordNode::SetReference(
    std::weak_ptr<DocNode> Reference) {
  this->Reference = Reference;
}

int udocs_processor::ReferencableWordNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

udocs_processor::ReferencableWordNode::ReferencableWordNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : DocNode(Parent, Id) {}

bool udocs_processor::ReferencableWordNode::ShouldBeReferenced() const {
  return mShouldBeReferenced;
}

void udocs_processor::ReferencableWordNode::SetShouldBeReferenced(
    bool ShouldBeReferenced) {
  this->mShouldBeReferenced = ShouldBeReferenced;
}
