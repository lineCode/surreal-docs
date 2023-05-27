/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/native/NativeFunctionNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"
#include "udocs-processor/doctree/blueprints/BlueprintFunctionNode.h"

std::weak_ptr<udocs_processor::BlueprintFunctionNode>
udocs_processor::NativeFunctionNode::GetBlueprintCounterpart() const {
  return BlueprintCounterpart;
}

void udocs_processor::NativeFunctionNode::SetBlueprintCounterpart(
    std::weak_ptr<BlueprintFunctionNode> BlueprintCounterpart) {
  this->BlueprintCounterpart = BlueprintCounterpart;
}

udocs_processor::NativeFunctionType
  udocs_processor::NativeFunctionNode::GetType() const {
  return Type;
}

void udocs_processor::NativeFunctionNode::SetType(NativeFunctionType Type) {
  this->Type = Type;
}

bool udocs_processor::NativeFunctionNode::IsTemplate() const {
  return mIsTemplate;
}

void udocs_processor::NativeFunctionNode::SetIsTemplate(bool IsTemplate) {
  mIsTemplate = IsTemplate;
}

bool udocs_processor::NativeFunctionNode::IsVirtual() const {
  return mIsVirtual;
}

void udocs_processor::NativeFunctionNode::SetIsVirtual(bool IsVirtual) {
  mIsVirtual = IsVirtual;
}

bool udocs_processor::NativeFunctionNode::IsPureVirtual() const {
  return mIsPureVirtual;
}

void udocs_processor::NativeFunctionNode::SetIsPureVirtual(
    bool IsPureVirtual) {
  mIsPureVirtual = IsPureVirtual;
}

bool udocs_processor::NativeFunctionNode::IsOverride() const {
  return mIsOverride;
}

void udocs_processor::NativeFunctionNode::SetIsOverride(bool IsOverride) {
  mIsOverride = IsOverride;
}

bool udocs_processor::NativeFunctionNode::IsConst() const {
  return mIsConst;
}

void udocs_processor::NativeFunctionNode::SetIsConst(bool IsConst) {
  mIsConst = IsConst;
}

bool udocs_processor::NativeFunctionNode::IsFinal() const {
  return mIsFinal;
}

void udocs_processor::NativeFunctionNode::SetIsFinal(bool IsFinal) {
  mIsFinal = IsFinal;
}

bool udocs_processor::NativeFunctionNode::IsInline() const {
  return mIsInline;
}

void udocs_processor::NativeFunctionNode::SetIsInline(bool IsInline) {
  mIsInline = IsInline;
}

udocs_processor::NativeFunctionDefinition
  udocs_processor::NativeFunctionNode::GetDefinitionType() const {
  return DefinitionType;
}

void udocs_processor::NativeFunctionNode::SetDefinitionType(
    NativeFunctionDefinition DefinitionType) {
  this->DefinitionType = DefinitionType;
}

const std::weak_ptr<udocs_processor::NativeFunctionNode> &
    udocs_processor::NativeFunctionNode::GetSuperFunction() const {
  return SuperFunction;
}

void udocs_processor::NativeFunctionNode::SetSuperFunction(
    std::weak_ptr<NativeFunctionNode> SuperFunction) {
  this->SuperFunction = SuperFunction;
}

const std::string &
  udocs_processor::NativeFunctionNode::GetAfterArgsString() const {
  return AfterArgsString;
}

void udocs_processor::NativeFunctionNode::SetAfterArgsString(
    std::string AfterArgsString) {
  this->AfterArgsString = std::move(AfterArgsString);
}

std::vector<std::weak_ptr<udocs_processor::NativeFunctionNode>>
    udocs_processor::NativeFunctionNode::GetComplementaryFunctions() const {
  return ComplementaryFunctions;
}

void udocs_processor::NativeFunctionNode::AddComplementaryFunction(
    std::weak_ptr<NativeFunctionNode> MainFunction) {
  ComplementaryFunctions.emplace_back(MainFunction);
}

int udocs_processor::NativeFunctionNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

udocs_processor::NativeFunctionNode::NativeFunctionNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : NativeMemberNode(Parent, Id) {}

const std::string &
  udocs_processor::NativeFunctionNode::GetReturnDescription() const {
  return ReturnDescription;
}

void udocs_processor::NativeFunctionNode::SetReturnDescription(
    std::string ReturnDescription) {
  this->ReturnDescription = std::move(ReturnDescription);
}

bool udocs_processor::NativeFunctionNode::IsStatic() const {
  return mIsStatic;
}

void udocs_processor::NativeFunctionNode::SetIsStatic(bool IsStatic) {
  mIsStatic = IsStatic;
}

bool udocs_processor::NativeFunctionNode::IsExplicit() const {
  return mIsExplicit;
}

void udocs_processor::NativeFunctionNode::SetIsExplicit(bool IsExplicit) {
  mIsExplicit = IsExplicit;
}

bool udocs_processor::NativeFunctionNode::IsImplementation() const {
  return mIsImplementation;
}

void udocs_processor::NativeFunctionNode::SetIsImplementation(
    bool IsImplementation) {
  mIsImplementation = IsImplementation;
}

bool udocs_processor::NativeFunctionNode::IsInterfaceImplementation() const {
  return mIsInterfaceImplementation;
}

void udocs_processor::NativeFunctionNode::SetIsInterfaceImplementation(
    bool IsInterfaceImplementation) {
  mIsInterfaceImplementation = IsInterfaceImplementation;
}

bool udocs_processor::NativeFunctionNode::IsBlueprintNativeEvent() const {
  return mIsBlueprintNativeEvent;
}

void udocs_processor::NativeFunctionNode::SetIsBlueprintNativeEvent(
    bool IsBlueprintNativeEvent) {
  mIsBlueprintNativeEvent = IsBlueprintNativeEvent;
}

std::weak_ptr<udocs_processor::DocNode>
    udocs_processor::NativeFunctionNode::GetCounterpart() const {
  return BlueprintCounterpart;
}
