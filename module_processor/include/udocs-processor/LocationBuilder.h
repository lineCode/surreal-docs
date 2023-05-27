/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <string>
#include <memory>
#include "udocs-processor/doctree/DocNodeVisitor.h"

namespace udocs_processor {
class LocationBuilder : public DocNodeVisitor {
 public:
  explicit LocationBuilder(DocNode& Node);

  int Visit(ReferencableWordNode &Node) override;

  int Visit(ClassParentNode &Node) override;

  int Visit(DelegateTypeNode &Node) override;

  int Visit(EventOwningClassNode &Node) override;

  int Visit(EventParentClassNode &Node) override;

  int Visit(LogCategoryNode &Node) override;

  int Visit(NativeClassNode &Node) override;

  int Visit(NativeEnumElementNode &Node) override;

  int Visit(NativeEnumNode &Node) override;

  int Visit(NativeFunctionNode &Node) override;

  int Visit(NativeModuleNode &Node) override;

  int Visit(NativeParameterNode &Node) override;

  int Visit(NativePropertyNode &Node) override;

  int Visit(BlueprintClassNode &Node) override;

  int Visit(BlueprintClassParentNode &Node) override;

  int Visit(BlueprintEnumElementNode &Node) override;

  int Visit(BlueprintEnumNode &Node) override;

  int Visit(BlueprintFunctionNode &Node) override;

  int Visit(BlueprintModuleNode &Node) override;

  int Visit(BlueprintParameterNode &Node) override;

  int Visit(BlueprintPropertyNode &Node) override;

  int Visit(BlueprintStructNode &Node) override;

  int Visit(BlueprintEnumTypeNode &Node) override;

  int Visit(BlueprintInterfaceTypeNode &Node) override;

  int Visit(BlueprintObjectTypeNode &Node) override;

  int Visit(BlueprintStructTypeNode &Node) override;

  int Visit(SeeNode &Node) override;

  int Visit(NativeTypeNode &Node) override;

  int Visit(TemplateNode &Node) override;

  int Visit(BlueprintArrayTypeNode &Node) override;

  int Visit(BlueprintDelegateTypeNode &Node) override;

  int Visit(BlueprintMapTypeNode &Node) override;

  int Visit(BlueprintPrimitiveTypeNode &Node) override;

  int Visit(BlueprintSetTypeNode &Node) override;

  int Visit(BlueprintTypeNode &Node) override;

  const std::string& GetResult() const;

  std::string BuildLocation(DocNode& Node);

 private:
  std::shared_ptr<NativeClassNode> IsParentClass(DocNode& Node);

  void VisitParent(DocNode& Node);

  void VisitParentOrAddHeader(DocNode& Node);

  void AddHeader(DocNode& Node);

  void AddPath(BlueprintUNode& Node);

  std::string Result;
};
}  // namespace udocs_processor
