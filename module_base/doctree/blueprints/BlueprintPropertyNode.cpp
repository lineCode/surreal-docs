/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/blueprints/BlueprintPropertyNode.h"
#include "udocs-processor/doctree/native/NativePropertyNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"

bool udocs_processor::BlueprintPropertyNode::IsInstanceEditable() const {
  return mIsInstanceEditable;
}

void udocs_processor::BlueprintPropertyNode::SetIsInstanceEditable(
    bool IsInstanceEditable) {
  mIsInstanceEditable = IsInstanceEditable;
}

void udocs_processor::BlueprintPropertyNode::SetIsArchetypeEditable(
    bool IsArchetypeEditable) {
  mIsArchetypeEditable = IsArchetypeEditable;
}

bool udocs_processor::BlueprintPropertyNode::IsArchetypeEditable() const {
  return mIsArchetypeEditable;
}

bool udocs_processor::BlueprintPropertyNode::IsBlueprintReadOnly() const {
  return mIsBlueprintReadOnly;
}

void udocs_processor::BlueprintPropertyNode::SetIsBlueprintReadOnly(
    bool BlueprintReadOnly) {
  mIsBlueprintReadOnly = BlueprintReadOnly;
}

bool udocs_processor::BlueprintPropertyNode::DoExposeOnSpawn() const {
  return mDoExposeOnSpawn;
}

void udocs_processor::BlueprintPropertyNode::SetDoExposeOnSpawn(
    bool DoExposeOnSpawn) {
  mDoExposeOnSpawn = DoExposeOnSpawn;
}

bool udocs_processor::BlueprintPropertyNode::IsPrivate() const {
  return mIsPrivate;
}

void udocs_processor::BlueprintPropertyNode::SetIsPrivate(bool IsPrivate) {
  mIsPrivate = IsPrivate;
}

bool udocs_processor::BlueprintPropertyNode::DoExposeToCinematic() const {
  return mDoExposeToCinematic;
}

void udocs_processor::BlueprintPropertyNode::SetDoExposeToCinematic(
    bool DoExposeToCinematic) {
  mDoExposeToCinematic = DoExposeToCinematic;
}

bool udocs_processor::BlueprintPropertyNode::IsConfigVariable() const {
  return mIsConfigVariable;
}

void udocs_processor::BlueprintPropertyNode::SetIsConfigVariable(
    bool IsConfigVariable) {
  mIsConfigVariable = IsConfigVariable;
}

bool udocs_processor::BlueprintPropertyNode::IsBlueprintVisible() const {
  return mIsBlueprintVisible;
}

void udocs_processor::BlueprintPropertyNode::SetIsBlueprintVisible(
    bool IsBlueprintVisible) {
  mIsBlueprintVisible = IsBlueprintVisible;
}

const std::string &udocs_processor::BlueprintPropertyNode::GetClampMin() const {
  return ClampMin;
}

void udocs_processor::BlueprintPropertyNode::SetClampMin(std::string ClampMin) {
  this->ClampMin = std::move(ClampMin);
}

const std::string &udocs_processor::BlueprintPropertyNode::GetClampMax() const {
  return ClampMax;
}

void udocs_processor::BlueprintPropertyNode::SetClampMax(std::string ClampMax) {
  this->ClampMax = std::move(ClampMax);
}

udocs_processor::Replication
  udocs_processor::BlueprintPropertyNode::GetReplication() const {
  return ReplicationMode;
}

void udocs_processor::BlueprintPropertyNode::SetReplication(
    Replication ReplicationMode) {
  this->ReplicationMode = ReplicationMode;
}

const udocs_processor::LifetimeCondition &
  udocs_processor::BlueprintPropertyNode::GetReplicationCondition() const {
  return ReplicationCondition;
}

void udocs_processor::BlueprintPropertyNode::SetReplicationCondition(
    LifetimeCondition ReplicationCondition) {
  this->ReplicationCondition = ReplicationCondition;
}

udocs_processor::MulticastDelegateFlags
  udocs_processor::BlueprintPropertyNode::GetMulticastDelegateFlags() const {
  return McDelegateFlags;
}

void udocs_processor::BlueprintPropertyNode::SetMulticastDelegateFlags(
    MulticastDelegateFlags McDelegateFlags) {
  this->McDelegateFlags = McDelegateFlags;
}

int udocs_processor::BlueprintPropertyNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

std::weak_ptr<udocs_processor::NativePropertyNode>
udocs_processor::BlueprintPropertyNode::GetNativeCounterpart() const {
  return NativeCounterpart;
}

void udocs_processor::BlueprintPropertyNode::SetNativeCounterpart(
    std::weak_ptr<NativePropertyNode> NativeCounterpart) {
  this->NativeCounterpart = NativeCounterpart;
}

udocs_processor::BlueprintPropertyNode::BlueprintPropertyNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : BlueprintUNode(Parent, Id) {}

std::weak_ptr<udocs_processor::DocNode>
    udocs_processor::BlueprintPropertyNode::GetCounterpart() const {
  return NativeCounterpart;
}
