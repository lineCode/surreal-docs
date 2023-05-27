/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include <string>
#include "NativeMemberNode.h"
#include "BlueprintableNode.h"
#include "udocs-processor/doctree/AccessModifier.h"
#include "udocs-processor/doctree/HasCounterpart.h"

namespace udocs_processor {
class BlueprintPropertyNode;

class NativePropertyNode : public NativeMemberNode,
    public HasCounterpart, public BlueprintableNode {
 public:
  NativePropertyNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  std::weak_ptr<DocNode> GetCounterpart() const override;

  std::weak_ptr<BlueprintPropertyNode> GetBlueprintCounterpart() const;

  void SetBlueprintCounterpart(
      std::weak_ptr<BlueprintPropertyNode> BlueprintCounterpart);

  bool IsStatic() const;

  void SetIsStatic(bool IsStatic);

  bool IsVolatile() const;

  void SetIsVolatile(bool IsVolatile);

  bool IsMutable() const;

  void SetIsMutable(bool IsMutable);

  bool IsConst() const;

  void SetIsConst(bool IsConst);

  int Accept(DocNodeVisitor &Visitor) override;

  ~NativePropertyNode() override = default;

 private:
  std::weak_ptr<BlueprintPropertyNode> BlueprintCounterpart;
  bool mIsStatic = false;
  bool mIsVolatile = false;
  bool mIsMutable = false;
  bool mIsConst = false;
};
}  // namespace udocs_processor

