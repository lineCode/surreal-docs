/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

namespace udocs_processor {
class DocNode;
class SeeNode;
class ReferencableWordNode;

class ClassParentNode;
class DelegateTypeNode;
class EventOwningClassNode;
class EventParentClassNode;
class LogCategoryNode;
class UeInfoNode;
class NativeClassNode;
class NativeEnumElementNode;
class NativeEnumNode;
class NativeFunctionNode;
class NativeMemberNode;
class NativeModuleNode;
class NativeParameterNode;
class NativePropertyNode;
class NativeTypeNode;
class NativeInitializerNode;
class TemplateNode;

class BlueprintClassNode;
class BlueprintClassParentNode;
class BlueprintEnumElementNode;
class BlueprintEnumNode;
class BlueprintFunctionNode;
class BlueprintModuleNode;
class BlueprintParameterNode;
class BlueprintPropertyNode;
class BlueprintStructNode;
class BlueprintUNode;
class BlueprintInitializerNode;
class BlueprintArrayTypeNode;
class BlueprintDelegateTypeNode;
class BlueprintEnumTypeNode;
class BlueprintInterfaceTypeNode;
class BlueprintMapTypeNode;
class BlueprintObjectTypeNode;
class BlueprintPrimitiveTypeNode;
class BlueprintSetTypeNode;
class BlueprintStructTypeNode;
class BlueprintTypeNode;

class DocsGroupNode;
class DocsImageNode;
class DocsIndexNode;
class DocsPageNode;
class DocsElementNode;

class DocNodeVisitor {
 public:
  DocNodeVisitor() = default;

  explicit DocNodeVisitor(int DefaultReturnCode)
      : DefaultReturnCode(DefaultReturnCode) {}

  virtual int Visit(DocNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(SeeNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(ReferencableWordNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(ClassParentNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(DelegateTypeNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(EventOwningClassNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(EventParentClassNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(LogCategoryNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(UeInfoNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(NativeClassNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(NativeEnumElementNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(NativeEnumNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(NativeFunctionNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(NativeMemberNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(NativeModuleNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(NativeParameterNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(NativePropertyNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(NativeTypeNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(NativeInitializerNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(TemplateNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(BlueprintClassNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(BlueprintClassParentNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(BlueprintEnumElementNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(BlueprintEnumNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(BlueprintFunctionNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(BlueprintModuleNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(BlueprintParameterNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(BlueprintPropertyNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(BlueprintStructNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(BlueprintUNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(BlueprintArrayTypeNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(BlueprintInitializerNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(BlueprintDelegateTypeNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(BlueprintEnumTypeNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(BlueprintInterfaceTypeNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(BlueprintMapTypeNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(BlueprintObjectTypeNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(BlueprintPrimitiveTypeNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(BlueprintSetTypeNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(BlueprintStructTypeNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(BlueprintTypeNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(DocsGroupNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(DocsPageNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(DocsImageNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(DocsIndexNode& Node) {
    return DefaultReturnCode;
  }

  virtual int Visit(DocsElementNode& Node) {
    return DefaultReturnCode;
  }

 private:
  int DefaultReturnCode = 0;
};
}  // namespace udocs_processor

