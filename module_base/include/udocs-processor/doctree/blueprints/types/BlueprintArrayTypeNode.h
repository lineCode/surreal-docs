/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include "BlueprintTypeNode.h"

namespace udocs_processor {
class BlueprintArrayTypeNode : public BlueprintTypeNode {
 public:
  BlueprintArrayTypeNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  int Accept(DocNodeVisitor &Visitor) override;

  ~BlueprintArrayTypeNode() override = default;
};
}  // namespace udocs_processor
