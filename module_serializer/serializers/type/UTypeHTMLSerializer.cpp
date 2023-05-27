/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#define NOLINT()

#include "udocs-processor/serializers/type/UTypeHTMLSerializer.h"

#include <fmt/format.h>
#include <utility>
#include <string>
#include <map>
#include <regex> NOLINT()
#include <inja/inja.hpp>
#include <nlohmann/json.hpp>
#include "udocs-processor/doctree/blueprints/BlueprintFunctionNode.h"
#include "udocs-processor/doctree/blueprints/BlueprintParameterNode.h"
#include "udocs-processor/doctree/blueprints/BlueprintClassNode.h"
#include "udocs-processor/doctree/blueprints/types/BlueprintTypeNode.h"
#include "udocs-processor/doctree/blueprints/types/BlueprintObjectTypeNode.h"
#include "udocs-processor/doctree/blueprints/types/BlueprintArrayTypeNode.h"
#include "udocs-processor/doctree/blueprints/types/BlueprintEnumTypeNode.h"
#include "udocs-processor/doctree/blueprints/types/BlueprintInterfaceTypeNode.h"
#include "udocs-processor/doctree/blueprints/types/BlueprintMapTypeNode.h"
#include "udocs-processor/doctree/blueprints/types/BlueprintPrimitiveTypeNode.h"
#include "udocs-processor/doctree/blueprints/types/BlueprintDelegateTypeNode.h"
#include "udocs-processor/doctree/blueprints/types/BlueprintSetTypeNode.h"
#include "udocs-processor/doctree/blueprints/types/BlueprintStructTypeNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"
#include "udocs-processor/NodesHelper.h"

void udocs_processor::UTypeHTMLSerializer::Serialize(
    BlueprintTypeNode &Type) {
  std::unordered_map<std::string, std::string> Result;

  TypeExporter Exporter;
  Type.Accept(Exporter);

  for (const auto& Type : Exporter.GetExportedTypes()) {
    nlohmann::json TypeData;

    TypeData[STRUCTURE] = Type.Structure;
    TypeData[MAIN] = Type.MainType;
    TypeData[SECONDARY] = Type.SecondaryType;

    std::string Name = Type.Structure + NAME_SEPARATOR + Type.MainType +
        NAME_SEPARATOR + Type.SecondaryType;

    if (!GetDecider() || GetDecider()(Name)) {
      Result.insert(std::make_pair(Name,
          Environment.render(TypeTemplate, TypeData)));
    } else {
      Result.insert(std::make_pair(Name, ""));
    }
  }

  GetTypeImagesNonConst() = std::move(Result);
}

const std::string &
  udocs_processor::UTypeHTMLSerializer::GetTypeTemplate() const {
  return TypeTemplate;
}

void udocs_processor::UTypeHTMLSerializer::SetTypeTemplate(
    std::string TypeTemplate) {
  this->TypeTemplate = std::move(TypeTemplate);
}

std::string udocs_processor::UTypeHTMLSerializer::GetFileExtension() const {
  return FILE_EXTENSION;
}

udocs_processor::UTypeHTMLSerializer::UTypeHTMLSerializer() {
  Environment.add_callback(RESOURCE_CALLABLE,
      RESOURCE_CALLABLE_ARGS_COUNT,
      [this](InjaArgsStripped Args) {
        return MakeResource(Args);
  });
}

nlohmann::json udocs_processor::UTypeHTMLSerializer::MakeResource(
    InjaArgs Args) const {
  if (!Args.empty() && Args[0]->is_string()) {
    return GetResourcesPath() + DIRECTORY_SEPARATOR +
        Args[0]->get<std::string>();
  }

  return "";
}

int udocs_processor::UTypeHTMLSerializer::TypeExporter::Visit(
    BlueprintEnumTypeNode &Node) {
  ExportedType Type;
  Type.MainType = TYPE_ENUM;
  Type.Structure = TYPE_STRUCTURE_PLAIN;
  ExportedTypes.emplace_back(std::move(Type));

  if (Callback) {
    Callback(*this);
  }

  return 0;
}

int udocs_processor::UTypeHTMLSerializer::TypeExporter::Visit(
    BlueprintInterfaceTypeNode &Node) {
  ExportedType Type;
  Type.MainType = TYPE_INTERFACE;
  Type.Structure = TYPE_STRUCTURE_INTERFACE;
  ExportedTypes.emplace_back(std::move(Type));

  if (Callback) {
    Callback(*this);
  }

  return 0;
}

int udocs_processor::UTypeHTMLSerializer::TypeExporter::Visit(
    BlueprintObjectTypeNode &Node) {
  ExportedType Type;

  Type.MainType = TYPE_OBJECT;
  auto It = ObjectReferenceToString.find(Node.GetObjectReference());
  if (It != ObjectReferenceToString.end()) {
    Type.MainType = It->second;
  }

  Type.Structure = TYPE_STRUCTURE_OBJECT;
  auto It2 = ClassTypeToString.find(Node.GetClassType());
  if (It2 != ClassTypeToString.end()) {
    Type.Structure = It2->second;
  }

  ExportedTypes.emplace_back(std::move(Type));

  if (Callback) {
    Callback(*this);
  }

  return 0;
}

int udocs_processor::UTypeHTMLSerializer::TypeExporter::Visit(
    BlueprintPrimitiveTypeNode &Node) {
  ExportedType Type;
  PrimitiveType::PrimitiveType PrType = Node.GetPrimitiveType();

  Type.MainType = TYPE_BOOL;
  auto It = PrimitiveTypeToString.find(PrType);
  if (It != PrimitiveTypeToString.end()) {
    Type.MainType = It->second;
  }

  Type.Structure = TYPE_STRUCTURE_PLAIN;
  ExportedTypes.emplace_back(std::move(Type));

  if (Callback) {
    Callback(*this);
  }

  return 0;
}

int udocs_processor::UTypeHTMLSerializer::TypeExporter::Visit(
    BlueprintArrayTypeNode &Node) {
  // expecting one type child
  TypeExporter Exporter;
  Node.IterateChildren(Exporter, false);

  ExportedType Type;
  Type.MainType = TYPE_BOOL;
  if (!Exporter.GetExportedTypes().empty()) {
    Type.MainType = Exporter.GetExportedTypes()[0].MainType;
  }

  Type.Structure = TYPE_STRUCTURE_ARRAY;
  ExportedTypes.emplace_back(std::move(Type));

  if (Callback) {
    Callback(*this);
  }

  return 0;
}

int udocs_processor::UTypeHTMLSerializer::TypeExporter::Visit(
    BlueprintMapTypeNode &Node) {
  // expecting one type child
  TypeExporter Exporter;
  Node.IterateChildren(Exporter, false);

  ExportedType Type;
  Type.MainType = TYPE_BOOL;
  Type.SecondaryType = TYPE_BOOL;
  Type.Structure = TYPE_STRUCTURE_MAP;
  if (!Exporter.GetExportedTypes().empty()) {
    Type.MainType = Exporter.GetExportedTypes()[0].MainType;

    if (Exporter.GetExportedTypes().size() > 1) {
      Type.SecondaryType = Exporter.GetExportedTypes()[1].MainType;
    }
  }

  ExportedType KeyType;
  KeyType.MainType = Type.MainType;
  KeyType.Structure = TYPE_STRUCTURE_MAPKEY;
  ExportedTypes.emplace_back(std::move(KeyType));

  ExportedType ValueType;
  ValueType.MainType = Type.SecondaryType;
  ValueType.Structure = TYPE_STRUCTURE_MAPVALUE;
  ExportedTypes.emplace_back(std::move(ValueType));

  ExportedTypes.emplace_back(std::move(Type));

  if (Callback) {
    Callback(*this);
  }

  return 0;
}

int udocs_processor::UTypeHTMLSerializer::TypeExporter::Visit(
    BlueprintSetTypeNode &Node) {
  // expecting one type child
  TypeExporter Exporter;
  Node.IterateChildren(Exporter, false);

  ExportedType Type;
  Type.MainType = TYPE_BOOL;
  if (!Exporter.GetExportedTypes().empty()) {
    Type.MainType = Exporter.GetExportedTypes()[0].MainType;
  }

  Type.Structure = TYPE_STRUCTURE_SET;
  ExportedTypes.emplace_back(std::move(Type));

  if (Callback) {
    Callback(*this);
  }

  return 0;
}

int udocs_processor::UTypeHTMLSerializer::TypeExporter::Visit(
    BlueprintStructTypeNode &Node) {
  ExportedType Type;
  Type.MainType = TYPE_STRUCT;
  Type.Structure = TYPE_STRUCTURE_PLAIN;

  if (Node.GetPath() == NodesHelper::VECTOR_PATH) {
    Type.MainType = TYPE_VECTOR;
  } else if (Node.GetPath() == NodesHelper::ROTATOR_PATH) {
    Type.MainType = TYPE_ROTATOR;
  } else {
    if (Node.GetPath() == NodesHelper::TRANSFORM_PATH) {
      Type.MainType = TYPE_TRANSFORM;
    }
  }

  ExportedTypes.emplace_back(std::move(Type));

  if (Callback) {
    Callback(*this);
  }

  return 0;
}

int udocs_processor::UTypeHTMLSerializer::TypeExporter::Visit(
    BlueprintDelegateTypeNode &Node) {
  ExportedType Type;
  if (Node.IsMulticast()) {
    Type.MainType = TYPE_EVENT_DISPATCHER;
    Type.Structure = TYPE_STRUCTURE_EVENT_DISPATCHER;
  } else {
    Type.MainType = TYPE_DEFAULT;
    Type.Structure = TYPE_STRUCTURE_DELEGATE;
  }

  ExportedTypes.emplace_back(std::move(Type));

  if (Callback) {
    Callback(*this);
  }

  return 0;
}

const std::vector<udocs_processor::UTypeHTMLSerializer::
    TypeExporter::ExportedType>& udocs_processor::UTypeHTMLSerializer::
    TypeExporter::GetExportedTypes() const {
  return ExportedTypes;
}

udocs_processor::UTypeHTMLSerializer::TypeExporter::TypeExporter() {
  ObjectReferenceToString.insert(
      std::make_pair(ObjectReference::Class, TYPE_CLASS));
  ObjectReferenceToString.insert(
      std::make_pair(ObjectReference::SoftClass, TYPE_SOFT_CLASS));
  ObjectReferenceToString.insert(
      std::make_pair(ObjectReference::Object, TYPE_OBJECT));
  ObjectReferenceToString.insert(
      std::make_pair(ObjectReference::WeakObject, TYPE_OBJECT));
  ObjectReferenceToString.insert(
      std::make_pair(ObjectReference::SoftObject, TYPE_SOFT_OBJECT));

  ClassTypeToString.insert(
      std::make_pair(ClassType::Class, TYPE_STRUCTURE_OBJECT));
  ClassTypeToString.insert(
      std::make_pair(ClassType::Actor, TYPE_STRUCTURE_ACTOR));
  ClassTypeToString.insert(
      std::make_pair(ClassType::Component, TYPE_STRUCTURE_COMPONENT));
  ClassTypeToString.insert(
      std::make_pair(ClassType::Widget, TYPE_STRUCTURE_WIDGET));

  PrimitiveTypeToString.insert(
      std::make_pair(PrimitiveType::PrimitiveType::Bool, TYPE_BOOL));
  PrimitiveTypeToString.insert(
      std::make_pair(PrimitiveType::PrimitiveType::Byte, TYPE_BYTE));
  PrimitiveTypeToString.insert(
      std::make_pair(PrimitiveType::PrimitiveType::Int, TYPE_INT));
  PrimitiveTypeToString.insert(
      std::make_pair(PrimitiveType::PrimitiveType::Int64, TYPE_INT64));
  PrimitiveTypeToString.insert(
      std::make_pair(PrimitiveType::PrimitiveType::Double, TYPE_DOUBLE));
  PrimitiveTypeToString.insert(
      std::make_pair(PrimitiveType::PrimitiveType::Float, TYPE_FLOAT));
  PrimitiveTypeToString.insert(
      std::make_pair(PrimitiveType::PrimitiveType::Text, TYPE_TEXT));
  PrimitiveTypeToString.insert(
      std::make_pair(PrimitiveType::PrimitiveType::Name, TYPE_NAME));
  PrimitiveTypeToString.insert(
      std::make_pair(PrimitiveType::PrimitiveType::String, TYPE_STRING));
}

udocs_processor::UTypeHTMLSerializer::TypeExporter::TypeExporter(
    std::function<void(TypeExporter&)> Callback)
    : TypeExporter() {
  this->Callback = std::move(Callback);
}
