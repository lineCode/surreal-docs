/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include <string>
#include "NativeMemberNode.h"
#include "BlueprintableNode.h"

namespace udocs_processor {
class NativeParameterNode : public NativeMemberNode, public BlueprintableNode {
 public:
  NativeParameterNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  int Accept(DocNodeVisitor &Visitor) override;

  ~NativeParameterNode() override = default;
};
}  // namespace udocs_processor

