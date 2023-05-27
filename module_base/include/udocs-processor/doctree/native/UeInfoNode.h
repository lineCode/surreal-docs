/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <string>
#include <memory>
#include "udocs-processor/doctree/DocNode.h"

namespace udocs_processor {
enum class UeInfoType {
  SPECIFIER,  // <ueitem> whose name is "specifier"
  META_SPECIFIER,  // <ueitem> whose name is "meta-specifier"
  UE_ITEM  // <ueitem> whose name is neither "specifier" nor "meta-specifier"
};

class UeInfoNode : public DocNode {
 public:
  UeInfoNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  UeInfoType GetType() const;

  void SetType(UeInfoType Type);

  const std::string &GetSpecifierName() const;

  void SetSpecifierName(std::string SpecifierName);

  const std::string &GetSpecifierValue() const;

  void SetSpecifierValue(std::string SpecifierValue);

  void CopyInfoTo(UeInfoNode &Node) const;

  int Accept(DocNodeVisitor &Visitor) override;

  ~UeInfoNode() override = default;

 private:
  UeInfoType Type = UeInfoType::UE_ITEM;
  std::string SpecifierName;
  std::string SpecifierValue;
};
}  // namespace udocs_processor
