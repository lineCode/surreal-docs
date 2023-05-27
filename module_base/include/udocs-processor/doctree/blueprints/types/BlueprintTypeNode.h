/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include "udocs-processor/doctree/blueprints/BlueprintUNode.h"

namespace udocs_processor {
class BlueprintTypeNode : public BlueprintUNode {
 public:
  BlueprintTypeNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  bool IsReference() const;

  void SetIsReference(bool IsReference);

  bool IsConst() const;

  void SetIsConst(bool IsConst);

  BlueprintTypeNode& operator=(BlueprintTypeNode&& Node) noexcept;

  int Accept(DocNodeVisitor &Visitor) override;

  ~BlueprintTypeNode() override = default;

 private:
  bool mIsReference = false;
  bool mIsConst = false;
};
}  // namespace udocs_processor
