/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include "BlueprintTypeNode.h"

namespace udocs_processor {
class BlueprintStructTypeNode : public BlueprintTypeNode {
 public:
  BlueprintStructTypeNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  int Accept(DocNodeVisitor &Visitor) override;

  ~BlueprintStructTypeNode() override = default;
};
}  // namespace udocs_processor
