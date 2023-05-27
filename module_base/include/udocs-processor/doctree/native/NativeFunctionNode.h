/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include <string>
#include <vector>
#include "NativeMemberNode.h"
#include "BlueprintableNode.h"
#include "udocs-processor/doctree/AccessModifier.h"
#include "udocs-processor/doctree/HasCounterpart.h"

namespace udocs_processor {
class BlueprintFunctionNode;

enum class NativeFunctionType {
  FUNCTION,
  IMPLEMENTATION,
  VALIDATION,
  OPERATOR,
  DESTRUCTOR,
  CONSTRUCTOR
};

enum class NativeFunctionDefinition {
  NORMAL,
  DELETED,
  DEFAULTED
};

class NativeFunctionNode : public NativeMemberNode,
    public HasCounterpart, public BlueprintableNode {
 public:
  NativeFunctionNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  std::weak_ptr<DocNode> GetCounterpart() const override;

  std::weak_ptr<BlueprintFunctionNode> GetBlueprintCounterpart() const;

  void SetBlueprintCounterpart(
      std::weak_ptr<BlueprintFunctionNode> BlueprintCounterpart);

  NativeFunctionType GetType() const;

  void SetType(NativeFunctionType Type);

  bool IsTemplate() const;

  void SetIsTemplate(bool IsTemplate);

  bool IsVirtual() const;

  void SetIsVirtual(bool IsVirtual);

  bool IsPureVirtual() const;

  void SetIsPureVirtual(bool IsPureVirtual);

  bool IsOverride() const;

  void SetIsOverride(bool IsOverride);

  bool IsConst() const;

  void SetIsConst(bool IsConst);

  bool IsFinal() const;

  void SetIsFinal(bool IsFinal);

  bool IsInline() const;

  void SetIsInline(bool IsInline);

  bool IsStatic() const;

  void SetIsStatic(bool IsStatic);

  bool IsExplicit() const;

  void SetIsExplicit(bool IsExplicit);

  bool IsImplementation() const;

  void SetIsImplementation(bool IsImplementation);

  bool IsInterfaceImplementation() const;

  void SetIsInterfaceImplementation(bool IsInterfaceImplementation);

  bool IsBlueprintNativeEvent() const;

  void SetIsBlueprintNativeEvent(bool IsBlueprintNativeEvent);

  const std::string &GetReturnDescription() const;

  void SetReturnDescription(std::string ReturnDescription);

  NativeFunctionDefinition GetDefinitionType() const;

  void SetDefinitionType(NativeFunctionDefinition DefinitionType);

  const std::weak_ptr<NativeFunctionNode> &GetSuperFunction() const;

  void SetSuperFunction(std::weak_ptr<NativeFunctionNode> SuperFunction);

  const std::string &GetAfterArgsString() const;

  void SetAfterArgsString(std::string AfterArgsString);

  std::vector<std::weak_ptr<NativeFunctionNode>>
      GetComplementaryFunctions() const;

  void AddComplementaryFunction(std::weak_ptr<NativeFunctionNode> Function);

  int Accept(DocNodeVisitor &Visitor) override;

  ~NativeFunctionNode() override = default;

 private:
  std::weak_ptr<BlueprintFunctionNode> BlueprintCounterpart;
  NativeFunctionType Type = NativeFunctionType::FUNCTION;
  bool mIsTemplate = false;
  bool mIsVirtual = false;
  bool mIsPureVirtual = false;
  bool mIsOverride = false;
  bool mIsConst = false;
  bool mIsFinal = false;
  bool mIsInline = false;
  bool mIsStatic = false;
  bool mIsExplicit = false;
  bool mIsImplementation = false;
  bool mIsInterfaceImplementation = false;
  bool mIsBlueprintNativeEvent = false;
  NativeFunctionDefinition DefinitionType = NativeFunctionDefinition::NORMAL;
  std::weak_ptr<NativeFunctionNode> SuperFunction;
  std::string AfterArgsString;
  std::string ReturnDescription;
  std::vector<std::weak_ptr<NativeFunctionNode>> ComplementaryFunctions;
};
}  // namespace udocs_processor
