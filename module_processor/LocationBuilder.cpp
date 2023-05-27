/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/LocationBuilder.h"
#include <fmt/format.h>
#include <string>
#include "udocs-processor/doctree/NodeTypeChecker.h"
#include "udocs-processor/doctree/native/NativeModuleNode.h"
#include "udocs-processor/doctree/native/NativeClassNode.h"
#include "udocs-processor/doctree/native/NativePropertyNode.h"
#include "udocs-processor/doctree/native/NativeMemberNode.h"
#include "udocs-processor/doctree/native/NativeFunctionNode.h"
#include "udocs-processor/doctree/native/NativeEnumNode.h"
#include "udocs-processor/doctree/native/NativeEnumElementNode.h"
#include "udocs-processor/doctree/native/NativeParameterNode.h"
#include "udocs-processor/doctree/native/TemplateNode.h"
#include "udocs-processor/doctree/native/NativeTypeNode.h"
#include "udocs-processor/doctree/native/LogCategoryNode.h"
#include "udocs-processor/doctree/native/DelegateTypeNode.h"
#include "udocs-processor/doctree/SeeNode.h"
#include "udocs-processor/doctree/DocNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"
#include "udocs-processor/doctree/ReferencableWordNode.h"
#include "udocs-processor/doctree/native/UeInfoNode.h"
#include "udocs-processor/doctree/blueprints/BlueprintModuleNode.h"
#include "udocs-processor/doctree/blueprints/BlueprintClassNode.h"
#include "udocs-processor/doctree/blueprints/BlueprintEnumNode.h"
#include "udocs-processor/doctree/blueprints/BlueprintEnumElementNode.h"
#include "udocs-processor/doctree/blueprints/BlueprintFunctionNode.h"
#include "udocs-processor/doctree/blueprints/BlueprintParameterNode.h"
#include "udocs-processor/doctree/blueprints/BlueprintPropertyNode.h"
#include "udocs-processor/doctree/blueprints/BlueprintStructNode.h"
#include "udocs-processor/doctree/blueprints/BlueprintUNode.h"
#include "udocs-processor/doctree/blueprints/BlueprintClassParentNode.h"
#include "udocs-processor/doctree/blueprints/types/BlueprintTypeNode.h"
#include "udocs-processor/doctree/blueprints/types/BlueprintPrimitiveTypeNode.h"
#include "udocs-processor/doctree/blueprints/types/BlueprintStructTypeNode.h"
#include "udocs-processor/doctree/blueprints/types/BlueprintEnumTypeNode.h"
#include "udocs-processor/doctree/blueprints/types/BlueprintArrayTypeNode.h"
#include "udocs-processor/doctree/blueprints/types/BlueprintSetTypeNode.h"
#include "udocs-processor/doctree/blueprints/types/BlueprintMapTypeNode.h"
#include "udocs-processor/doctree/blueprints/types/BlueprintInterfaceTypeNode.h"
#include "udocs-processor/doctree/blueprints/types/BlueprintObjectTypeNode.h"
#include "udocs-processor/doctree/blueprints/types/BlueprintDelegateTypeNode.h"

void udocs_processor::LocationBuilder::VisitParent(DocNode &Node) {
  Node.GetParent().lock()->Accept(*this);
}

void udocs_processor::LocationBuilder::AddHeader(DocNode &Node) {
  Result += fmt::format("`{}` header", Node.GetPath());
}

void udocs_processor::LocationBuilder::VisitParentOrAddHeader(DocNode &Node) {
  if (auto ParentClass = IsParentClass(Node)) {
    ParentClass->Accept(*this);
  } else {
    AddHeader(Node);
  }
}

std::shared_ptr<udocs_processor::NativeClassNode>
    udocs_processor::LocationBuilder::IsParentClass(DocNode& Node) {
  if (auto Parent = Node.GetParent().lock()) {
    NodeTypeChecker<NativeClassNode> Type(*Parent);
    if (Type.Matched()) {
      return Type.GetNode();
    }
  }

  return nullptr;
}

void udocs_processor::LocationBuilder::AddPath(BlueprintUNode &Node) {
  Result += fmt::format("path `{}`", Node.GetPath());
}

int udocs_processor::LocationBuilder::Visit(SeeNode &Node) {
  Result += fmt::format("\"see\" section in ");
  VisitParent(Node);

  return 0;
}

int udocs_processor::LocationBuilder::Visit(ReferencableWordNode &Node) {
  Result += fmt::format("`{}` in ", Node.GetWord());
  VisitParent(Node);

  return 0;
}

int udocs_processor::LocationBuilder::Visit(ClassParentNode &Node) {
  Result += fmt::format("inherited class `{}` in ", Node.GetName());
  VisitParent(Node);

  return 0;
}

int udocs_processor::LocationBuilder::Visit(DelegateTypeNode &Node) {
  Result += fmt::format("delegate `{}` in ", Node.GetName());
  VisitParentOrAddHeader(Node);

  return 0;
}

int udocs_processor::LocationBuilder::Visit(EventOwningClassNode &Node) {
  Result += fmt::format("event owning class `{}` in ", Node.GetName());
  VisitParent(Node);

  return 0;
}

int udocs_processor::LocationBuilder::Visit(EventParentClassNode &Node) {
  Result += fmt::format("event parent class `{}` in ", Node.GetName());
  VisitParent(Node);

  return 0;
}

int udocs_processor::LocationBuilder::Visit(LogCategoryNode &Node) {
  Result += fmt::format("log category `{}` in ", Node.GetName());
  AddHeader(Node);

  return 0;
}

int udocs_processor::LocationBuilder::Visit(NativeClassNode &Node) {
  Result += fmt::format("class `{}` in ", Node.GetName());
  VisitParentOrAddHeader(Node);

  return 0;
}

int udocs_processor::LocationBuilder::Visit(NativeEnumElementNode &Node) {
  Result += fmt::format("element `{}` in ", Node.GetName());
  VisitParent(Node);

  return 0;
}

int udocs_processor::LocationBuilder::Visit(NativeEnumNode &Node) {
  Result += fmt::format("enum `{}` in ", Node.GetName());
  VisitParentOrAddHeader(Node);

  return 0;
}

int udocs_processor::LocationBuilder::Visit(NativeFunctionNode &Node) {
  std::string Name = "function";
  if (IsParentClass(Node)) {
    Name = "method";
  }

  Result += fmt::format("{} `{}` in ", Name, Node.GetName());
  VisitParentOrAddHeader(Node);

  return 0;
}

int udocs_processor::LocationBuilder::Visit(NativeModuleNode &Node) {
  Result += fmt::format("module `{}`", Node.GetName());

  return 0;
}

int udocs_processor::LocationBuilder::Visit(NativeParameterNode &Node) {
  Result += fmt::format("parameter `{}` of ", Node.GetName());
  VisitParent(Node);

  return 0;
}

int udocs_processor::LocationBuilder::Visit(NativePropertyNode &Node) {
  std::string Name = "variable";
  if (IsParentClass(Node)) {
    Name = "field";
  }

  Result += fmt::format("{} `{}` in ", Name, Node.GetName());
  VisitParentOrAddHeader(Node);

  return 0;
}

int udocs_processor::LocationBuilder::Visit(NativeTypeNode &Node) {
  Result += fmt::format("type of ");
  VisitParent(Node);

  return 0;
}

int udocs_processor::LocationBuilder::Visit(TemplateNode &Node) {
  Result += fmt::format("template of ");
  VisitParent(Node);

  return 0;
}

int udocs_processor::LocationBuilder::Visit(BlueprintClassParentNode &Node) {
  std::string Name = "inherited class";
  if (Node.IsInterface()) {
    Name = "implemented interface";
  }

  Result += fmt::format("{} `{}` in ", Name, Node.GetName());
  VisitParent(Node);

  return 0;
}

int udocs_processor::LocationBuilder::Visit(BlueprintEnumElementNode &Node) {
  Result += fmt::format("element `{}` in ", Node.GetName());
  VisitParent(Node);

  return 0;
}

int udocs_processor::LocationBuilder::Visit(BlueprintClassNode &Node) {
  std::string Name = "BP class";
  if (Node.IsInterface()) {
    Name = "BP interface";
  }

  Result += fmt::format("{} `{}` at ", Name, Node.GetName());
  AddPath(Node);

  return 0;
}

int udocs_processor::LocationBuilder::Visit(BlueprintEnumNode &Node) {
  Result += fmt::format("BP enum `{}` at ", Node.GetName());
  AddPath(Node);

  return 0;
}

int udocs_processor::LocationBuilder::Visit(BlueprintStructNode &Node) {
  Result += fmt::format("BP struct `{}` at ", Node.GetName());
  AddPath(Node);

  return 0;
}

int udocs_processor::LocationBuilder::Visit(BlueprintFunctionNode &Node) {
  std::string Name = "BP function";
  if (Node.IsEvent()) {
    Name = "event";
  }

  Result += fmt::format("{} `{}` in ", Name, Node.GetName());
  VisitParent(Node);

  return 0;
}

int udocs_processor::LocationBuilder::Visit(BlueprintModuleNode &Node) {
  Result += fmt::format("module `{}`", Node.GetName());

  return 0;
}

int udocs_processor::LocationBuilder::Visit(BlueprintParameterNode &Node) {
  Result += fmt::format("parameter `{}` of ", Node.GetName());
  VisitParent(Node);

  return 0;
}

int udocs_processor::LocationBuilder::Visit(BlueprintPropertyNode &Node) {
  // todo(artsiom.drapun): add special case for event dispatchers
  Result += fmt::format("BP property `{}` in ", Node.GetName());
  VisitParent(Node);

  return 0;
}

int udocs_processor::LocationBuilder::Visit(BlueprintTypeNode &Node) {
  Result += fmt::format("type of ");
  VisitParent(Node);

  return 0;
}

int udocs_processor::LocationBuilder::Visit(BlueprintEnumTypeNode &Node) {
  return Visit(static_cast<BlueprintTypeNode&>(Node));
}

int udocs_processor::LocationBuilder::Visit(BlueprintInterfaceTypeNode &Node) {
  return Visit(static_cast<BlueprintTypeNode&>(Node));
}

int udocs_processor::LocationBuilder::Visit(BlueprintObjectTypeNode &Node) {
  return Visit(static_cast<BlueprintTypeNode&>(Node));
}

int udocs_processor::LocationBuilder::Visit(BlueprintStructTypeNode &Node) {
  return Visit(static_cast<BlueprintTypeNode&>(Node));
}

int udocs_processor::LocationBuilder::Visit(BlueprintArrayTypeNode &Node) {
  return Visit(static_cast<BlueprintTypeNode&>(Node));
}

int udocs_processor::LocationBuilder::Visit(BlueprintDelegateTypeNode &Node) {
  return Visit(static_cast<BlueprintTypeNode&>(Node));
}

int udocs_processor::LocationBuilder::Visit(BlueprintMapTypeNode &Node) {
  return Visit(static_cast<BlueprintTypeNode&>(Node));
}

int udocs_processor::LocationBuilder::Visit(BlueprintPrimitiveTypeNode &Node) {
  return Visit(static_cast<BlueprintTypeNode&>(Node));
}

int udocs_processor::LocationBuilder::Visit(BlueprintSetTypeNode &Node) {
  return Visit(static_cast<BlueprintTypeNode&>(Node));
}

const std::string& udocs_processor::LocationBuilder::GetResult() const {
  return Result;
}

std::string udocs_processor::LocationBuilder::BuildLocation(DocNode& Node) {
  Result = {};

  Node.Accept(*this);

  return Result;
}

udocs_processor::LocationBuilder::LocationBuilder(DocNode &Node) {
  BuildLocation(Node);
}
