/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include <string>
#include "NativeMemberNode.h"
#include "BlueprintableNode.h"

namespace udocs_processor {
class NativeEnumElementNode : public NativeMemberNode,
    public BlueprintableNode {
 public:
  NativeEnumElementNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  int Accept(DocNodeVisitor &Visitor) override;

  ~NativeEnumElementNode() override = default;
};
}  // namespace udocs_processor
