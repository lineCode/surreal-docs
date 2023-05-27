/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include <string>
#include "DocNode.h"

namespace udocs_processor {
class DocNodeVisitor;

class ReferencableWordNode : public DocNode {
 public:
  ReferencableWordNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  const std::string &GetWord() const;

  void SetWord(std::string Word);

  const std::weak_ptr<DocNode> &GetReference() const;

  void SetReference(std::weak_ptr<DocNode> Reference);

  bool ShouldBeReferenced() const;

  void SetShouldBeReferenced(bool ShouldBeReferenced);

  int Accept(DocNodeVisitor &Visitor) override;

  ~ReferencableWordNode() override = default;

 private:
  bool mShouldBeReferenced = true;
  std::string Word;
  std::weak_ptr<DocNode> Reference;
};
}  // namespace udocs_processor
