/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include "BlueprintUNode.h"

namespace udocs_processor {
class BlueprintEnumElementNode : public BlueprintUNode {
 public:
  BlueprintEnumElementNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  bool IsHidden() const;

  void SetIsHidden(bool IsHidden);

  int Accept(DocNodeVisitor &Visitor) override;

  ~BlueprintEnumElementNode() override = default;

 private:
  bool mIsHidden = false;
};
}  // namespace udocs_processor
