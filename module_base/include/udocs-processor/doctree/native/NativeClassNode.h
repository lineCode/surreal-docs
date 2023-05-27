/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include <string>
#include "NativeMemberNode.h"
#include "BlueprintableNode.h"
#include "ClassParentNode.h"
#include "udocs-processor/doctree/HasCounterpart.h"

namespace udocs_processor {
class NativeClassCounterpart;

enum class NativeClassType {
  CLASS,
  STRUCT,
  INTERFACE
};

class NativeClassNode : public NativeMemberNode,
    public HasCounterpart, public BlueprintableNode {
 public:
  NativeClassNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  std::weak_ptr<DocNode> GetCounterpart() const override;

  std::weak_ptr<NativeClassCounterpart> GetBlueprintCounterpart() const;

  void SetBlueprintCounterpart(
      std::weak_ptr<NativeClassCounterpart> BlueprintCounterpart);

  NativeClassType GetType() const;

  void SetType(NativeClassType Type);

  std::weak_ptr<NativeClassNode> GetInterfaceComplementary() const;

  void SetInterfaceComplementary(
      std::weak_ptr<NativeClassNode> InterfaceComplementary);

  bool IsFinal() const;

  void SetIsFinal(bool IsFinal);

  bool IsTemplate() const;

  void SetIsTemplate(bool IsTemplate);

  int Accept(DocNodeVisitor &Visitor) override;

  ~NativeClassNode() override = default;

 private:
  NativeClassType Type = NativeClassType::CLASS;
  std::weak_ptr<NativeClassCounterpart> BlueprintCounterpart;
  std::weak_ptr<NativeClassNode> InterfaceComplementary;
  bool mIsFinal = false;
  bool mIsTemplate = false;
};
}  // namespace udocs_processor

