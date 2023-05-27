/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/native/LogCategoryNode.h"
#include <utility>
#include "udocs-processor/doctree/DocNodeVisitor.h"

udocs_processor::LogVerbosity
    udocs_processor::LogCategoryNode::GetRuntimeLevel() const {
  return RuntimeLevel;
}

void udocs_processor::LogCategoryNode::SetRuntimeLevel(
    LogVerbosity RuntimeLevel) {
  this->RuntimeLevel = RuntimeLevel;
}

udocs_processor::LogVerbosity
    udocs_processor::LogCategoryNode::GetCompileLevel() const {
  return CompileLevel;
}

void udocs_processor::LogCategoryNode::SetCompileLevel(
    LogVerbosity CompileLevel) {
  this->CompileLevel = CompileLevel;
}

int udocs_processor::LogCategoryNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

udocs_processor::LogCategoryNode::LogCategoryNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : NativeMemberNode(Parent, Id) {}

udocs_processor::LogCategoryType
  udocs_processor::LogCategoryNode::GetType() const {
  return Type;
}

void udocs_processor::LogCategoryNode::SetType(LogCategoryType Type) {
  this->Type = Type;
}
