/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/native/NativeMemberNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"

udocs_processor::AccessModifier
udocs_processor::NativeMemberNode::GetAccess() const {
  return Access;
}

void udocs_processor::NativeMemberNode::SetAccess(AccessModifier Access) {
  this->Access = Access;
}

const std::string &udocs_processor::NativeMemberNode::GetInclude() const {
  return Include;
}

void udocs_processor::NativeMemberNode::SetInclude(
    std::string Include) {
  this->Include = std::move(Include);
}

const std::string &udocs_processor::NativeMemberNode::GetNamespace() const {
  return Namespace;
}

void udocs_processor::NativeMemberNode::SetNamespace(
    std::string Namespace) {
  this->Namespace = std::move(Namespace);
}

int udocs_processor::NativeMemberNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

udocs_processor::NativeMemberNode::NativeMemberNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id) : DocNode(Parent, Id) {}

const std::string &udocs_processor::NativeMemberNode::GetApi() const {
  return Api;
}

void udocs_processor::NativeMemberNode::SetApi(std::string Api) {
  this->Api = std::move(Api);
}

udocs_processor::NativeMemberNode&
udocs_processor::NativeMemberNode::operator=(NativeMemberNode&& Node) noexcept {
  Api = std::move(Node.Api);
  Namespace = std::move(Node.Namespace);
  Include = std::move(Node.Include);
  Access = std::move(Node.Access);

  DocNode::operator=(std::move(Node));

  return *this;
}

const std::string &udocs_processor::NativeMemberNode::GetCleanName() const {
  return CleanName;
}

void udocs_processor::NativeMemberNode::SetCleanName(
    std::string CleanName) {
  this->CleanName = std::move(CleanName);
}
