/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/blueprints/BlueprintUNode.h"
#include <utility>
#include "udocs-processor/doctree/DocNodeVisitor.h"

const std::string &udocs_processor::BlueprintUNode::GetDisplayName() const {
  return DisplayName;
}

void udocs_processor::BlueprintUNode::SetDisplayName(std::string DisplayName) {
  this->DisplayName = std::move(DisplayName);
}

const std::string &udocs_processor::BlueprintUNode::GetCategory() const {
  return Category;
}

void udocs_processor::BlueprintUNode::SetCategory(std::string Category) {
  this->Category = std::move(Category);
}

int udocs_processor::BlueprintUNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

bool udocs_processor::BlueprintUNode::IsNative() const {
  return mIsNative;
}

void udocs_processor::BlueprintUNode::SetIsNative(bool IsNative) {
  mIsNative = IsNative;
}

udocs_processor::BlueprintUNode::BlueprintUNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id) : DocNode(Parent, Id) {}


udocs_processor::BlueprintUNode &
    udocs_processor::BlueprintUNode::operator=(
    udocs_processor::BlueprintUNode &&Node) noexcept {
  Category = std::move(Node.Category);
  DisplayName = std::move(Node.DisplayName);
  mIsNative = Node.mIsNative;

  DocNode::operator=(std::move(Node));

  return *this;
}
