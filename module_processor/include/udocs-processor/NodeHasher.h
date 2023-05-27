/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#define NOLINT()

#include <regex> NOLINT()
#include <string>
#include "udocs-processor/doctree/DocNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"

namespace udocs_processor {
class NodeHasher : private DocNodeVisitor {
 public:
  NodeHasher();

  virtual uint32_t HashNode(DocNode& Node);

 private:
  std::string HashByPath(DocNode& Node);

  void HashIt(const std::string& ToHash);

  int Visit(DelegateTypeNode &Node) override;

  int Visit(LogCategoryNode &Node) override;

  int Visit(NativeClassNode &Node) override;

  int Visit(NativeEnumNode &Node) override;

  int Visit(NativeFunctionNode &Node) override;

  int Visit(NativeModuleNode &Node) override;

  int Visit(NativePropertyNode &Node) override;

  int Visit(BlueprintClassNode &Node) override;

  int Visit(BlueprintEnumNode &Node) override;

  int Visit(BlueprintFunctionNode &Node) override;

  int Visit(BlueprintModuleNode &Node) override;

  int Visit(BlueprintPropertyNode &Node) override;

  int Visit(BlueprintDelegateTypeNode &Node) override;

  int Visit(BlueprintStructNode &Node) override;

  // template params + usual params
  std::string HashByFullSignature(NativeFunctionNode& Node) const;

  std::string HashByTemplate(DocNode& Node) const;

  // expects parent of params as Node
  template<typename T>
  std::string HashBySignature(T& Node) const;

  std::string StringifyType(DocNode& Node) const;

  std::string FullStringifyType(DocNode& Node) const;

  constexpr static char SIGNATURE_SEPARATION_PATTERN[] =
      R"(([a-z0-9_])\s+([a-z0-9_]))";
  constexpr static char SIGNATURE_SEPARATION_REPLACE_PATTERN[] = R"($1#$2)";
  constexpr static char SPACE[] = " ";
  constexpr static char SIGNATURE_TYPE_DELIMITER[] = ",";
  constexpr static char SIGNATURE_TEMPLATE_DELIMITER[] = "---";
  constexpr static char SIGNATURE_RET_TYPE_DELIMITER[] = "---";

  constexpr static char PATH_AND_NAME_SEPARATOR[] = "///";

  std::regex Separation;

  uint32_t Hash = 0;
};
}  // namespace udocs_processor
