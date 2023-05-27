/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <string>
#include <memory>
#include "udocs-processor/doctree/DocNode.h"

namespace udocs_processor {
class DocsGroupNode : public DocNode {
 public:
  DocsGroupNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  const std::string &GetTitle() const;

  void SetTitle(std::string Title);

  int Accept(DocNodeVisitor &Visitor) override;

  ~DocsGroupNode() override = default;

 private:
  std::string Title;
};
}  // namespace udocs_processor
