/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <string>
#include <memory>
#include "udocs-processor/doctree/DocNode.h"

namespace udocs_processor {
class DocsElementNode : public DocNode {
 public:
  DocsElementNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  // absolute path
  const std::string &GetFileReference() const;

  void SetFileReference(std::string FileReference);

  const std::string &GetDocPath() const;

  void SetDocPath(std::string DocPath);

  int Accept(DocNodeVisitor &Visitor) override;

  ~DocsElementNode() override = default;

 private:
  std::string FileReference;
  std::string DocPath;
};
}  // namespace udocs_processor
