/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include <string>
#include <vector>
#include "BlueprintUNode.h"
#include "udocs-processor/doctree/AccessModifier.h"
#include "udocs-processor/doctree/HasCounterpart.h"

namespace udocs_processor {
class NativeFunctionNode;

enum class RpcMode {
  NotReplicated,
  NetMulticast,
  Server,
  Client
};

class BlueprintFunctionNode : public BlueprintUNode, public HasCounterpart {
 public:
  BlueprintFunctionNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  std::weak_ptr<DocNode> GetCounterpart() const override;

  std::weak_ptr<NativeFunctionNode> GetNativeCounterpart() const;

  void SetNativeCounterpart(
      std::weak_ptr<NativeFunctionNode> NativeCounterpart);

  const std::vector<std::string>& GetKeywords() const;

  void SetKeywords(std::vector<std::string> Keywords);

  const std::string& GetCompactNodeTitle() const;

  void SetCompactNodeTitle(std::string CompactNodeTitle);

  const std::string& GetNodeComment() const;

  void SetNodeComment(std::string NodeComment);

  AccessModifier GetAccessModifier() const;

  void SetAccessModifier(AccessModifier Access);

  bool IsPure() const;

  void SetIsPure(bool bIsPure);

  bool IsCallInEditor() const;

  void SetCallInEditor(bool bCallInEditor);

  bool IsOverride() const;

  void SetIsOverride(bool bIsOverride);

  bool IsEvent() const;

  void SetIsEvent(bool bIsEvent);

  bool IsReliable() const;

  void SetIsReliable(bool bIsReliable);

  bool IsStatic() const;

  void SetIsStatic(bool bIsStatic);

  bool IsConst() const;

  void SetIsConst(bool bIsConst);

  bool IsBlueprintCallable() const;

  void SetIsBlueprintCallable(bool bIsBlueprintCallable);

  // relatively to blueprints
  bool IsOverridable() const;

  void SetIsOverridable(bool bIsOverridable);

  bool IsBlueprintProtected() const;

  void SetIsBlueprintProtected(bool bIsBlueprintProtected);

  bool IsLatent() const;

  void SetIsLatent(bool IsLatent);

  bool IsCosmetic() const;

  void SetIsCosmetic(bool IsCosmetic);

  bool IsAuthorityOnly() const;

  void SetIsAuthorityOnly(bool IsAuthorityOnly);

  const std::weak_ptr<BlueprintFunctionNode> &GetSuperFunction() const;

  void SetSuperFunction(std::weak_ptr<BlueprintFunctionNode> SuperFunction);

  bool IsInterfaceImplementation() const;

  void SetIsInterfaceImplementation(bool IsInterfaceImplementation);

  RpcMode GetRpcMode() const;

  void SetRpcMode(RpcMode Rpc);

  int Accept(DocNodeVisitor &Visitor) override;

  ~BlueprintFunctionNode() override = default;

 private:
  std::weak_ptr<NativeFunctionNode> NativeCounterpart;
  std::weak_ptr<BlueprintFunctionNode> SuperFunction;
  std::vector<std::string> Keywords;
  std::string CompactNodeTitle;
  std::string NodeComment;
  AccessModifier Access = AccessModifier::PUBLIC;
  bool bIsPure = false;
  bool bCallInEditor = false;
  bool bIsOverride = false;
  bool bIsEvent = false;
  bool bIsReliable = false;
  bool bIsStatic = false;
  bool bIsConst = false;
  bool mIsLatent = false;
  bool mIsCosmetic = false;
  bool mIsAuthorityOnly = false;
  bool bIsBlueprintCallable = false;
  bool mIsInterfaceImplementation = false;
  bool bIsOverridable = false;
  bool bIsBlueprintProtected = false;
  RpcMode Rpc = RpcMode::NotReplicated;
};
}  // namespace udocs_processor

