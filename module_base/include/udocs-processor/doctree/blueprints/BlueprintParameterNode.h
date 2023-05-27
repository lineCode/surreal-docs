/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include <string>
#include "BlueprintUNode.h"

namespace udocs_processor {
class BlueprintParameterNode : public BlueprintUNode {
 public:
  BlueprintParameterNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  bool IsReturnParameter() const;

  void SetIsReturnParameter(bool IsReturnParameter);

  int Accept(DocNodeVisitor &Visitor) override;

  ~BlueprintParameterNode() override = default;

 private:
  bool mIsReturnParameter = false;
};
}  // namespace udocs_processor

