/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include "BlueprintUNode.h"
#include "udocs-processor/doctree/HasCounterpart.h"

namespace udocs_processor {
class NativePropertyNode;

enum class Replication {
  None,
  Replicate,
  RepNotify
};

enum class LifetimeCondition {
  None,
  InitialOnly,
  OwnerOnly,
  SkipOwner,
  SimulatedOnly,
  AutonomousOnly,
  SimulatedOrPhysics,
  InitialOrOwner,
  Custom,
  ReplayOrOwner,
  ReplayOnly,
  SimulatedOnlyNoReplay,
  SimulatedOrPhysicsNoReplay,
  SkipReplay,
  Never
};

enum MulticastDelegateFlags : uint32_t {
  UDMC_NoFlags = 0x00000000,
  UDMC_BlueprintAssignable = 0x00000001,
  UDMC_BlueprintCallable = 0x00000002,
  UDMC_BlueprintAuthorityOnly = 0x00000004,
};

inline MulticastDelegateFlags operator|(
    MulticastDelegateFlags A, MulticastDelegateFlags B) {
  return static_cast<MulticastDelegateFlags>(
      static_cast<int>(A) | static_cast<int>(B));
}

inline void operator|=(
    MulticastDelegateFlags& A, MulticastDelegateFlags B) {
  A = A | B;
}

class BlueprintPropertyNode : public BlueprintUNode, public HasCounterpart {
 public:
  BlueprintPropertyNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  std::weak_ptr<DocNode> GetCounterpart() const override;

  std::weak_ptr<NativePropertyNode> GetNativeCounterpart() const;

  void SetNativeCounterpart(
      std::weak_ptr<NativePropertyNode> NativeCounterpart);

  bool IsInstanceEditable() const;

  void SetIsInstanceEditable(bool IsInstanceEditable);

  bool IsArchetypeEditable() const;

  void SetIsArchetypeEditable(bool IsArchetypeEditable);

  bool IsBlueprintReadOnly() const;

  void SetIsBlueprintReadOnly(bool BlueprintReadOnly);

  bool DoExposeOnSpawn() const;

  void SetDoExposeOnSpawn(bool DoExposeOnSpawn);

  bool IsPrivate() const;

  void SetIsPrivate(bool IsPrivate);

  bool DoExposeToCinematic() const;

  void SetDoExposeToCinematic(bool DoExposeToCinematic);

  bool IsConfigVariable() const;

  void SetIsConfigVariable(bool IsConfigVariable);

  bool IsBlueprintVisible() const;

  void SetIsBlueprintVisible(bool IsBlueprintVisible);

  const std::string& GetClampMin() const;

  void SetClampMin(std::string ClampMin);

  const std::string& GetClampMax() const;

  void SetClampMax(std::string ClampMax);

  Replication GetReplication() const;

  void SetReplication(Replication ReplicationMode);

  const LifetimeCondition& GetReplicationCondition() const;

  void SetReplicationCondition(LifetimeCondition ReplicationCondition);

  MulticastDelegateFlags GetMulticastDelegateFlags() const;

  void SetMulticastDelegateFlags(
      MulticastDelegateFlags McDelegateFlags);

  int Accept(DocNodeVisitor &Visitor) override;

  ~BlueprintPropertyNode() override = default;

 private:
  std::weak_ptr<NativePropertyNode> NativeCounterpart;
  bool mIsInstanceEditable = false;
  bool mIsArchetypeEditable = false;
  bool mIsBlueprintReadOnly = false;
  bool mDoExposeOnSpawn = false;
  bool mIsPrivate = false;
  bool mDoExposeToCinematic = false;
  bool mIsConfigVariable = false;
  bool mIsBlueprintVisible = false;
  std::string ClampMin;
  std::string ClampMax;
  Replication ReplicationMode = Replication::None;
  LifetimeCondition ReplicationCondition = LifetimeCondition::None;
  MulticastDelegateFlags McDelegateFlags = UDMC_NoFlags;
};
}  // namespace udocs_processor

