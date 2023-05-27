/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <string>
#include <memory>
#include "DocsElementNode.h"

namespace udocs_processor {
enum class ImageType {
  PNG
};

class DocsImageNode : public DocsElementNode {
 public:
  DocsImageNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  ImageType GetType() const;

  void SetType(ImageType Type);

  int Accept(DocNodeVisitor &Visitor) override;

  ~DocsImageNode() override = default;

 private:
  ImageType Type = ImageType::PNG;
};
}  // namespace udocs_processor
