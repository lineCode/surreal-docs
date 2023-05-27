/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include "udocs-processor/doctree/DocNode.h"

namespace udocs_processor {
class NativeInitializerNode : public DocNode {
 public:
  NativeInitializerNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  int Accept(DocNodeVisitor &Visitor) override;

  ~NativeInitializerNode() override = default;
};
}  // namespace udocs_processor
