/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include "DocNode.h"

namespace udocs_processor {
class DocNodeVisitor;

class SeeNode : public DocNode {
 public:
  SeeNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  int Accept(DocNodeVisitor& Visitor) override;

  ~SeeNode() override = default;
};
}  // namespace udocs_processor
