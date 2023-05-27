/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include <string>
#include "udocs-processor/doctree/DocNode.h"
#include "udocs-processor/doctree/AccessModifier.h"

namespace udocs_processor {
class NativeMemberNode : public DocNode {
 public:
  NativeMemberNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  const std::string &GetInclude() const;

  void SetInclude(std::string Include);

  const std::string &GetNamespace() const;

  void SetNamespace(std::string Namespace);

  // for USTRUCT, UCLASS, UINTERFACE, _Validate, _Implementation classes
  const std::string &GetCleanName() const;

  void SetCleanName(std::string CleanName);

  const std::string &GetApi() const;

  void SetApi(std::string Api);

  AccessModifier GetAccess() const;

  void SetAccess(AccessModifier Access);

  NativeMemberNode& operator=(NativeMemberNode&& Node) noexcept;

  int Accept(DocNodeVisitor &Visitor) override;

  ~NativeMemberNode() override = default;

 private:
  std::string CleanName;
  std::string Namespace;
  std::string Include;
  std::string Api;
  AccessModifier Access = AccessModifier::PRIVATE;
};
}  // namespace udocs_processor
