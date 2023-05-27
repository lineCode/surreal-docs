/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include "BlueprintTypeNode.h"

namespace udocs_processor {
namespace PrimitiveType {
    enum class PrimitiveType {
        Float,
        Double,
        Byte,
        Int,
        Int64,
        Bool,
        String,
        Text,
        Name,
        Undefined
    };
}  // namespace PrimitiveType

class BlueprintPrimitiveTypeNode : public BlueprintTypeNode {
 public:
  BlueprintPrimitiveTypeNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  PrimitiveType::PrimitiveType GetPrimitiveType() const;

  void SetPrimitiveType(PrimitiveType::PrimitiveType PrimitiveType);

  int Accept(DocNodeVisitor &Visitor) override;

  ~BlueprintPrimitiveTypeNode() override = default;

 private:
    PrimitiveType::PrimitiveType PrimitiveType =
        PrimitiveType::PrimitiveType::Undefined;
};
}  // namespace udocs_processor
