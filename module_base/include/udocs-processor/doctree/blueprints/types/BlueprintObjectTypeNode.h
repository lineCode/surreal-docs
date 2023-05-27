/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include "BlueprintTypeNode.h"
#include "udocs-processor/doctree/blueprints/BlueprintClassNode.h"

namespace udocs_processor {
enum class ObjectReference {
    Object,
    WeakObject,
    LazyObject,
    SoftObject,
    Class,
    SoftClass,
    Undefined
};

class BlueprintObjectTypeNode : public BlueprintTypeNode {
 public:
  BlueprintObjectTypeNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  ObjectReference GetObjectReference() const;

  void SetObjectReference(ObjectReference Reference);

  ClassType GetClassType() const;

  void SetClassType(ClassType Type);

  int Accept(DocNodeVisitor &Visitor) override;

  ~BlueprintObjectTypeNode() override = default;

 private:
    ObjectReference Reference = ObjectReference::Class;
    ClassType Type = ClassType::Class;
};
}  // namespace udocs_processor
