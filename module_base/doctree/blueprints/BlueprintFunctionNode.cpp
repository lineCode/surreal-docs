/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/blueprints/BlueprintFunctionNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"
#include "udocs-processor/doctree/native/NativeFunctionNode.h"

const std::vector<std::string> &
    udocs_processor::BlueprintFunctionNode::GetKeywords() const {
  return Keywords;
}

void udocs_processor::BlueprintFunctionNode::SetKeywords(
    std::vector<std::string> Keywords) {
  this->Keywords = std::move(Keywords);
}

const std::string &
  udocs_processor::BlueprintFunctionNode::GetCompactNodeTitle() const {
  return CompactNodeTitle;
}

void udocs_processor::BlueprintFunctionNode::SetCompactNodeTitle(
    std::string CompactNodeTitle) {
  this->CompactNodeTitle = std::move(CompactNodeTitle);
}

const std::string &
  udocs_processor::BlueprintFunctionNode::GetNodeComment() const {
  return NodeComment;
}

void udocs_processor::BlueprintFunctionNode::SetNodeComment(
    std::string NodeComment) {
  this->NodeComment = std::move(NodeComment);
}

udocs_processor::AccessModifier
  udocs_processor::BlueprintFunctionNode::GetAccessModifier() const {
  return Access;
}

void udocs_processor::BlueprintFunctionNode::SetAccessModifier(
    AccessModifier Access) {
  this->Access = Access;
}

bool udocs_processor::BlueprintFunctionNode::IsPure() const {
  return bIsPure;
}

void udocs_processor::BlueprintFunctionNode::SetIsPure(bool bIsPure) {
  this->bIsPure = bIsPure;
}

bool udocs_processor::BlueprintFunctionNode::IsCallInEditor() const {
  return bCallInEditor;
}

void udocs_processor::BlueprintFunctionNode::SetCallInEditor(
    bool bCallInEditor) {
  this->bCallInEditor = bCallInEditor;
}

bool udocs_processor::BlueprintFunctionNode::IsOverride() const {
  return bIsOverride;
}

void udocs_processor::BlueprintFunctionNode::SetIsOverride(bool bIsOverride) {
  this->bIsOverride = bIsOverride;
}

bool udocs_processor::BlueprintFunctionNode::IsEvent() const {
  return bIsEvent;
}

void udocs_processor::BlueprintFunctionNode::SetIsEvent(bool bIsEvent) {
  this->bIsEvent = bIsEvent;
}

bool udocs_processor::BlueprintFunctionNode::IsReliable() const {
  return bIsReliable;
}

void udocs_processor::BlueprintFunctionNode::SetIsReliable(bool bIsReliable) {
  this->bIsReliable = bIsReliable;
}

bool udocs_processor::BlueprintFunctionNode::IsStatic() const {
  return bIsStatic;
}

void udocs_processor::BlueprintFunctionNode::SetIsStatic(bool bIsStatic) {
  this->bIsStatic = bIsStatic;
}

bool udocs_processor::BlueprintFunctionNode::IsConst() const {
  return bIsConst;
}

void udocs_processor::BlueprintFunctionNode::SetIsConst(bool bIsConst) {
  this->bIsConst = bIsConst;
}

bool udocs_processor::BlueprintFunctionNode::IsBlueprintCallable() const {
  return bIsBlueprintCallable;
}

void udocs_processor::BlueprintFunctionNode::SetIsBlueprintCallable(
    bool bIsBlueprintCallable) {
  this->bIsBlueprintCallable = bIsBlueprintCallable;
}

bool udocs_processor::BlueprintFunctionNode::IsOverridable() const {
  return bIsOverridable;
}

void udocs_processor::BlueprintFunctionNode::SetIsOverridable(
    bool bIsOverridable) {
  this->bIsOverridable = bIsOverridable;
}

bool udocs_processor::BlueprintFunctionNode::IsBlueprintProtected() const {
  return bIsBlueprintProtected;
}

void udocs_processor::BlueprintFunctionNode::SetIsBlueprintProtected(
    bool bIsBlueprintProtected) {
  this->bIsBlueprintProtected = bIsBlueprintProtected;
}

const std::weak_ptr<udocs_processor::BlueprintFunctionNode> &
udocs_processor::BlueprintFunctionNode::GetSuperFunction() const {
  return SuperFunction;
}

void udocs_processor::BlueprintFunctionNode::SetSuperFunction(
    std::weak_ptr<BlueprintFunctionNode> SuperFunction) {
  this->SuperFunction = SuperFunction;
}

bool udocs_processor::BlueprintFunctionNode::IsInterfaceImplementation() const {
  return mIsInterfaceImplementation;
}

void udocs_processor::BlueprintFunctionNode::SetIsInterfaceImplementation(
    bool IsInterfaceImplementation) {
  mIsInterfaceImplementation = IsInterfaceImplementation;
}

udocs_processor::RpcMode
  udocs_processor::BlueprintFunctionNode::GetRpcMode() const {
  return Rpc;
}

void udocs_processor::BlueprintFunctionNode::SetRpcMode(
    RpcMode Rpc) {
  this->Rpc = Rpc;
}

int udocs_processor::BlueprintFunctionNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

std::weak_ptr<udocs_processor::NativeFunctionNode>
udocs_processor::BlueprintFunctionNode::GetNativeCounterpart() const {
  return NativeCounterpart;
}

void udocs_processor::BlueprintFunctionNode::SetNativeCounterpart(
    std::weak_ptr<NativeFunctionNode> NativeCounterpart) {
  this->NativeCounterpart = NativeCounterpart;
}

udocs_processor::BlueprintFunctionNode::BlueprintFunctionNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : BlueprintUNode(Parent, Id) {}

std::weak_ptr<udocs_processor::DocNode>
    udocs_processor::BlueprintFunctionNode::GetCounterpart() const {
  return NativeCounterpart;
}

bool udocs_processor::BlueprintFunctionNode::IsLatent() const {
  return mIsLatent;
}

void udocs_processor::BlueprintFunctionNode::SetIsLatent(bool IsLatent) {
  this->mIsLatent = IsLatent;
}

bool udocs_processor::BlueprintFunctionNode::IsCosmetic() const {
  return mIsCosmetic;
}

void udocs_processor::BlueprintFunctionNode::SetIsCosmetic(bool IsCosmetic) {
  this->mIsCosmetic = IsCosmetic;
}

bool udocs_processor::BlueprintFunctionNode::IsAuthorityOnly() const {
  return mIsAuthorityOnly;
}

void udocs_processor::BlueprintFunctionNode::SetIsAuthorityOnly(
    bool IsAuthorityOnly) {
  mIsAuthorityOnly = IsAuthorityOnly;
}
