/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <string>
#include <memory>
#include "DocsElementNode.h"

namespace udocs_processor {
enum class PageType {
  PAGE,
  API_CPP,
  API_BLUEPRINT,
  EXTERNAL
};

enum class PageFormat {
  SURREAL_MD
};

class DocsPageNode : public DocsElementNode {
 public:
  DocsPageNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  PageType GetType() const;

  void SetType(PageType Type);

  PageFormat GetFormat() const;

  void SetFormat(PageFormat Format);

  const std::string &GetTitle() const;

  void SetTitle(std::string Title);

  bool IsCanonical() const;

  void SetIsCanonical(bool IsCanonical);

  bool IsEmpty() const;

  void SetIsEmpty(bool IsEmpty);

  bool DoExpandContents() const;

  void SetDoExpandContents(bool DoExpandContents);

  bool IsPinned() const;

  void SetIsPinned(bool IsPinned);

  void SetCanonical(std::weak_ptr<DocsPageNode> Canonical);

  std::weak_ptr<DocsPageNode> GetCanonical() const;

  int Accept(DocNodeVisitor &Visitor) override;

  ~DocsPageNode() override = default;

 private:
  std::weak_ptr<DocsPageNode> Canonical;

  bool mIsCanonical = false;
  bool mIsEmpty = false;
  bool mDoExpandContents = false;
  bool mIsPinned = false;
  PageType Type = PageType::PAGE;
  PageFormat Format = PageFormat::SURREAL_MD;
  std::string Title;
};
}  // namespace udocs_processor
