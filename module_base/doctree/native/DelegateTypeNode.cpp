/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/native/DelegateTypeNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"

bool udocs_processor::DelegateTypeNode::IsEvent() const {
  return mIsEvent;
}

void udocs_processor::DelegateTypeNode::SetIsEvent(bool IsEvent) {
  mIsEvent = IsEvent;
}

bool udocs_processor::DelegateTypeNode::IsDerivedEvent() const {
  return mIsDerivedEvent;
}

void udocs_processor::DelegateTypeNode::SetIsDerivedEvent(bool IsDerivedEvent) {
  mIsDerivedEvent = IsDerivedEvent;
}

bool udocs_processor::DelegateTypeNode::IsMulticast() const {
  return mIsMulticast;
}

void udocs_processor::DelegateTypeNode::SetIsMulticast(bool IsMulticast) {
  mIsMulticast = IsMulticast;
}

bool udocs_processor::DelegateTypeNode::IsDynamic() const {
  return mIsDynamic;
}

void udocs_processor::DelegateTypeNode::SetIsDynamic(bool IsDynamic) {
  mIsDynamic = IsDynamic;
}

bool udocs_processor::DelegateTypeNode::HasReturnParameter() const {
  return mHasReturnParameter;
}

void udocs_processor::DelegateTypeNode::SetHasReturnParameter(
    bool HasReturnParameter) {
  mHasReturnParameter = HasReturnParameter;
}

int udocs_processor::DelegateTypeNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

udocs_processor::DelegateTypeNode::DelegateTypeNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : NativeMemberNode(Parent, Id) {}

const std::string &
    udocs_processor::DelegateTypeNode::GetReturnDescription() const {
  return ReturnDescription;
}

void udocs_processor::DelegateTypeNode::SetReturnDescription(
    std::string ReturnDescription) {
  this->ReturnDescription = std::move(ReturnDescription);
}

udocs_processor::EventOwningClassNode::EventOwningClassNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : NativeMemberNode(Parent, Id) {}

std::weak_ptr<udocs_processor::NativeClassNode>
    udocs_processor::EventOwningClassNode::GetNode() const {
  return Node;
}

void udocs_processor::EventOwningClassNode::SetNode(
    std::weak_ptr<NativeClassNode> Node) {
  this->Node = Node;
}

int udocs_processor::EventOwningClassNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

udocs_processor::EventParentClassNode::EventParentClassNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : NativeMemberNode(Parent, Id) {}

std::weak_ptr<udocs_processor::NativeClassNode>
    udocs_processor::EventParentClassNode::GetNode() const {
  return Node;
}

void udocs_processor::EventParentClassNode::SetNode(
    std::weak_ptr<NativeClassNode> Node) {
  this->Node = Node;
}

int udocs_processor::EventParentClassNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}
