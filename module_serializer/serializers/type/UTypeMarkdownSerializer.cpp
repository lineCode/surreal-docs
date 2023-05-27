/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#define NOLINT()

#include "udocs-processor/serializers/type/UTypeMarkdownSerializer.h"

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
#include "udocs-processor/doctree/ReferencableWordNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"
#include "udocs-processor/StringHelper.h"
#include "udocs-processor/NodesHelper.h"

udocs_processor::UTypeMarkdownSerializer::UTypeMarkdownSerializer(
    const DocPathsCache& DocPathCache) : DocPathCache(DocPathCache) {}

void udocs_processor::UTypeMarkdownSerializer::Serialize(
    BlueprintTypeNode &Type) {
  std::unordered_map<std::string, std::string> Result;

  TypeExporter Exporter(DocPathCache, TypeImages);
  Type.Accept(Exporter);

  SetTypeMarkdown(Exporter.GetType());
}

void udocs_processor::UTypeMarkdownSerializer::SetTypeImages(
    const std::unordered_map<std::string, std::string> &TypeImages) {
  this->TypeImages = TypeImages;
}

int udocs_processor::UTypeMarkdownSerializer::TypeExporter::Visit(
    BlueprintEnumTypeNode &Node) {
  Type = fmt::format(TYPE_ENUM, MakeNameLink(
      Node, Node.GetDisplayName()));

  if (Callback) {
    Callback(*this);
  }

  return 0;
}

int udocs_processor::UTypeMarkdownSerializer::TypeExporter::Visit(
    BlueprintInterfaceTypeNode &Node) {
  Type = fmt::format(TYPE_INTERFACE,
      MakeNameLink(Node, Node.GetDisplayName()));

  if (Callback) {
    Callback(*this);
  }

  return 0;
}

int udocs_processor::UTypeMarkdownSerializer::TypeExporter::Visit(
    BlueprintObjectTypeNode &Node) {
  std::string Format = TYPE_OBJECT;
  auto It = ObjectReferenceToString.find(Node.GetObjectReference());
  if (It != ObjectReferenceToString.end()) {
    Format = It->second;
  }

  Type = fmt::format(Format, MakeNameLink(Node, Node.GetDisplayName()));

  if (Callback) {
    Callback(*this);
  }

  return 0;
}

int udocs_processor::UTypeMarkdownSerializer::TypeExporter::Visit(
    BlueprintPrimitiveTypeNode &Node) {
  PrimitiveType::PrimitiveType PrType = Node.GetPrimitiveType();

  Type = TYPE_BOOL;
  auto It = PrimitiveTypeToString.find(PrType);
  if (It != PrimitiveTypeToString.end()) {
    Type = It->second;
  }

  if (Callback) {
    Callback(*this);
  }

  return 0;
}

int udocs_processor::UTypeMarkdownSerializer::TypeExporter::Visit(
    BlueprintArrayTypeNode &Node) {
  // expecting one type child
  TypeExporter Exporter(DocPathCache, TypeImages);
  Node.IterateChildren(Exporter, false);

  Type = fmt::format(TYPE_ARRAY, Exporter.Type);

  if (Callback) {
    Callback(*this);
  }

  return 0;
}

int udocs_processor::UTypeMarkdownSerializer::TypeExporter::Visit(
    BlueprintMapTypeNode &Node) {
  // expecting two type children
  std::vector<std::string> Types;
  TypeExporter Exporter(DocPathCache, TypeImages,
    [&Types](TypeExporter& Exporter) {
      Types.emplace_back(std::move(Exporter.Type));
    });
  Node.IterateChildren(Exporter, false);

  std::string Key;
  std::string Value;
  if (!Types.empty()) {
    Key = std::move(Types[0]);

    if (Types.size() > 1) {
      Value = std::move(Types[1]);
    }
  }

  Type = fmt::format(TYPE_MAP, Key, Value);

  if (Callback) {
    Callback(*this);
  }

  return 0;
}

int udocs_processor::UTypeMarkdownSerializer::TypeExporter::Visit(
    BlueprintSetTypeNode &Node) {
  // expecting one type child
  TypeExporter Exporter(DocPathCache, TypeImages);
  Node.IterateChildren(Exporter, false);

  Type = fmt::format(TYPE_SET, Exporter.Type);

  if (Callback) {
    Callback(*this);
  }

  return 0;
}

int udocs_processor::UTypeMarkdownSerializer::TypeExporter::Visit(
    BlueprintStructTypeNode &Node) {

  if (Node.GetPath() == NodesHelper::VECTOR_PATH) {
    Type = TYPE_VECTOR;
  } else if (Node.GetPath() == NodesHelper::ROTATOR_PATH) {
    Type = TYPE_ROTATOR;
  } else if (Node.GetPath() == NodesHelper::TRANSFORM_PATH) {
    Type = TYPE_TRANSFORM;
  } else {
    Type = fmt::format(TYPE_STRUCT,
        MakeNameLink(Node, Node.GetDisplayName()));
  }

  if (Callback) {
    Callback(*this);
  }

  return 0;
}

int udocs_processor::UTypeMarkdownSerializer::TypeExporter::Visit(
    BlueprintDelegateTypeNode &Node) {
  // content defined delegates are always anonymous ones
  auto Counterpart = Node.GetNativeCounterpart().lock();
  bool IsAnonymous = !Counterpart;

  std::string Link;
  if (!IsAnonymous) {
    auto It = DocPathCache.GetCache().find(Node.GetId());
    if (It != DocPathCache.GetCache().end()) {
      std::string DisplayName = NodesHelper::MakeDisplayName(Node);

      Link = StringHelper::MakeMdLink(DisplayName, DisplayName, It->second);
    }
  }

  if (Node.IsMulticast()) {
    Type = IsAnonymous
        ? TYPE_ANON_EVENT_DISPATCHER : fmt::format(TYPE_EVENT_DISPATCHER, Link);
  } else {
    Type = IsAnonymous
        ? TYPE_ANON_DELEGATE : fmt::format(TYPE_DELEGATE, Link);
  }

  if (Callback) {
    Callback(*this);
  }

  return 0;
}

udocs_processor::UTypeMarkdownSerializer::TypeExporter::TypeExporter(
    const DocPathsCache& DocPathCache,
    const std::unordered_map<std::string, std::string> &TypeImages)
    : TypeExporter(DocPathCache, TypeImages, {}) {
  // lazy object refs are not supported by BP
  ObjectReferenceToString.insert(
      std::make_pair(ObjectReference::Class, TYPE_CLASS));
  ObjectReferenceToString.insert(
      std::make_pair(ObjectReference::SoftClass, TYPE_SOFT_CLASS));
  ObjectReferenceToString.insert(
      std::make_pair(ObjectReference::Object, TYPE_OBJECT));
  ObjectReferenceToString.insert(
      std::make_pair(ObjectReference::WeakObject, TYPE_WEAK_OBJECT));
  ObjectReferenceToString.insert(
      std::make_pair(ObjectReference::SoftObject, TYPE_SOFT_OBJECT));

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

udocs_processor::UTypeMarkdownSerializer::TypeExporter::TypeExporter(
    const DocPathsCache& DocPathCache,
    const std::unordered_map<std::string, std::string> &TypeImages,
    std::function<void(TypeExporter&)> Callback)
    : DocPathCache(DocPathCache), TypeImages(TypeImages) {
  this->Callback = std::move(Callback);
}

const std::string &udocs_processor::UTypeMarkdownSerializer::
    TypeExporter::GetType() const {
  return Type;
}

std::shared_ptr<udocs_processor::DocNode> udocs_processor::
    UTypeMarkdownSerializer::TypeExporter::FindReferencedNode(
        DocNode& Node) const {
  class RefWordFinder : public DocNodeVisitor {
   public:
    int Visit(ReferencableWordNode &Node) override {
      ReferencedNode = Node.GetReference().lock();

      return 1;
    }

    std::shared_ptr<DocNode> ReferencedNode;
  } RefWordFinderInstance;
  Node.IterateChildren(RefWordFinderInstance, false);

  return RefWordFinderInstance.ReferencedNode;
}

std::string
    udocs_processor::UTypeMarkdownSerializer::TypeExporter::MakeNameLink(
    DocNode &Node, const std::string& Text) const {
  if (std::shared_ptr<DocNode> ReferencedNode = FindReferencedNode(Node)) {
    auto It = DocPathCache.GetCache().find(ReferencedNode->GetId());
    if (It != DocPathCache.GetCache().end()) {
      return StringHelper::MakeMdLink(Text, Text, It->second);
    }
  }

  return Text;
}
