/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include <string>
#include "NativeMemberNode.h"
#include "BlueprintableNode.h"

namespace udocs_processor {
class NativeClassNode;

class EventOwningClassNode : public NativeMemberNode {
 public:
  EventOwningClassNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  std::weak_ptr<NativeClassNode> GetNode() const;

  void SetNode(std::weak_ptr<NativeClassNode> Node);

  int Accept(DocNodeVisitor &Visitor) override;

  ~EventOwningClassNode() override = default;

 private:
  std::weak_ptr<NativeClassNode> Node;
};

class EventParentClassNode : public NativeMemberNode {
 public:
  EventParentClassNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  std::weak_ptr<NativeClassNode> GetNode() const;

  void SetNode(std::weak_ptr<NativeClassNode> Node);

  int Accept(DocNodeVisitor &Visitor) override;

  ~EventParentClassNode() override = default;

 private:
  std::weak_ptr<NativeClassNode> Node;
};

class DelegateTypeNode : public NativeMemberNode, public BlueprintableNode {
 public:
  DelegateTypeNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  bool IsEvent() const;

  void SetIsEvent(bool IsEvent);

  bool IsDerivedEvent() const;

  void SetIsDerivedEvent(bool IsDerivedEvent);

  bool IsMulticast() const;

  void SetIsMulticast(bool IsMulticast);

  bool IsDynamic() const;

  void SetIsDynamic(bool IsDynamic);

  bool HasReturnParameter() const;

  void SetHasReturnParameter(bool HasReturnParameter);

  const std::string &GetReturnDescription() const;

  void SetReturnDescription(std::string ReturnDescription);

  int Accept(DocNodeVisitor &Visitor) override;

  ~DelegateTypeNode() override = default;

 private:
  bool mIsEvent = false;
  bool mIsDerivedEvent = false;
  bool mIsMulticast = false;
  bool mIsDynamic = false;
  bool mHasReturnParameter = false;
  std::string ReturnDescription;
};
}  // namespace udocs_processor

