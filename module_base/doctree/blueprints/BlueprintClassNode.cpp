/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/blueprints/BlueprintClassNode.h"
#include "udocs-processor/doctree/native/NativeClassNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"

std::weak_ptr<udocs_processor::NativeClassNode>
udocs_processor::BlueprintClassNode::GetNativeCounterpart() const {
  return NativeCounterpart;
}

void udocs_processor::BlueprintClassNode::SetNativeCounterpart(
    std::weak_ptr<NativeClassNode> NativeCounterpart) {
  this->NativeCounterpart = std::move(NativeCounterpart);
}

bool udocs_processor::BlueprintClassNode::IsBlueprintType() const {
  return mIsBlueprintType;
}

void udocs_processor::BlueprintClassNode::SetIsBlueprintType(
    bool IsBlueprintType) {
  this->mIsBlueprintType = IsBlueprintType;
}

bool udocs_processor::BlueprintClassNode::IsBlueprintable() const {
  return mIsBlueprintable;
}

void udocs_processor::BlueprintClassNode::SetIsBlueprintable(
    bool IsBlueprintable) {
  this->mIsBlueprintable = IsBlueprintable;
}

bool udocs_processor::BlueprintClassNode::IsAbstract() const {
  return mIsAbstract;
}

void udocs_processor::BlueprintClassNode::SetIsAbstract(bool IsAbstract) {
  this->mIsAbstract = IsAbstract;
}

bool udocs_processor::BlueprintClassNode::IsInterface() const {
  return mIsInterface;
}

void udocs_processor::BlueprintClassNode::SetIsInterface(bool IsInterface) {
  this->mIsInterface = IsInterface;
}

udocs_processor::ClassType
  udocs_processor::BlueprintClassNode::GetClassType() const {
  return Type;
}

void udocs_processor::BlueprintClassNode::SetClassType(
    ClassType Type) {
  this->Type = Type;
}

const std::string &udocs_processor::BlueprintClassNode::GetNamespace() const {
  return Namespace;
}

void udocs_processor::BlueprintClassNode::SetNamespace(std::string Namespace) {
  this->Namespace = std::move(Namespace);
}

const std::string &udocs_processor::BlueprintClassNode::GetConfigName() const {
  return ConfigName;
}

void udocs_processor::BlueprintClassNode::SetConfigName(
    std::string ConfigName) {
  this->ConfigName = std::move(ConfigName);
}

int udocs_processor::BlueprintClassNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

udocs_processor::BlueprintClassNode::BlueprintClassNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : NativeClassCounterpart(Parent, Id) {}

std::weak_ptr<udocs_processor::DocNode>
    udocs_processor::BlueprintClassNode::GetCounterpart() const {
  return NativeCounterpart;
}
