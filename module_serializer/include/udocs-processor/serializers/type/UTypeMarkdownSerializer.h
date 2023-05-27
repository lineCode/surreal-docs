/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "udocs-processor/doctree/blueprints/types/BlueprintObjectTypeNode.h"
#include "udocs-processor/doctree/blueprints/types/BlueprintPrimitiveTypeNode.h"
#include "udocs-processor/doctree/blueprints/BlueprintFunctionNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"
#include "UTypeToStringSerializer.h"
#include "udocs-processor/DocPathsCache.h"

namespace udocs_processor {
class UTypeMarkdownSerializer : public UTypeToStringSerializer {
  class TypeExporter : public DocNodeVisitor {
   public:
    explicit TypeExporter(
        const DocPathsCache& DocPathCache,
        const std::unordered_map<std::string, std::string> &TypeImages);

    TypeExporter(
        const DocPathsCache& DocPathCache,
        const std::unordered_map<std::string, std::string> &TypeImages,
        std::function<void(TypeExporter&)> Callback);

    // Array of <element type>s
    int Visit(BlueprintArrayTypeNode &Node) override;

    // <name> Enum
    int Visit(BlueprintEnumTypeNode &Node) override;

    // <name> Interface
    int Visit(BlueprintInterfaceTypeNode &Node) override;

    // Map of <key type>s to <value type>s
    int Visit(BlueprintMapTypeNode &Node) override;

    /*
     * <name> Object Reference
     * <name> Weak Object Reference
     * <name> Lazy Object Reference
     * <name> Soft Object Reference
     * <name> Class Reference
     * <name> Soft Class Reference
     */
    int Visit(BlueprintObjectTypeNode &Node) override;

    // Integer, Integer64, Boolean, Byte, Float/Float, Name, String, Text
    int Visit(BlueprintPrimitiveTypeNode &Node) override;

    // Set of <element type>s
    int Visit(BlueprintSetTypeNode &Node) override;

    // Vector, Transform, Rotator, <name> Struct
    int Visit(BlueprintStructTypeNode &Node) override;

    // <name> Event Dispatcher, Event Dispatcher, <name> Event, Event
    int Visit(BlueprintDelegateTypeNode &Node) override;

    const std::string &GetType() const;

   private:
    std::string MakeNameLink(DocNode& Node, const std::string& Text) const;

    std::shared_ptr<DocNode> FindReferencedNode(DocNode& Node) const;

    constexpr static char TYPE_MAP[] = "Map of {}s to {}s";
    constexpr static char TYPE_SET[] = "Set of {}s";
    constexpr static char TYPE_ARRAY[] = "Array of {}s";
    constexpr static char TYPE_BOOL[] = "Boolean";
    constexpr static char TYPE_ANON_EVENT_DISPATCHER[] = "Event Dispatcher";
    constexpr static char TYPE_EVENT_DISPATCHER[] = "{} Event Dispatcher";
    constexpr static char TYPE_ANON_DELEGATE[] = "Event";
    constexpr static char TYPE_DELEGATE[] = "{} Event";
    constexpr static char TYPE_BYTE[] = "Byte";
    constexpr static char TYPE_INT[] = "Integer";
    constexpr static char TYPE_INT64[] = "Integer64";
    constexpr static char TYPE_FLOAT[] = "Float";
    constexpr static char TYPE_DOUBLE[] = "Float";
    constexpr static char TYPE_NAME[] = "Name";
    constexpr static char TYPE_STRING[] = "String";
    constexpr static char TYPE_TEXT[] = "Text";
    constexpr static char TYPE_VECTOR[] = "Vector";
    constexpr static char TYPE_ROTATOR[] = "Rotator";
    constexpr static char TYPE_TRANSFORM[] = "Transform";
    constexpr static char TYPE_STRUCT[] = "{} Struct";
    constexpr static char TYPE_INTERFACE[] = "{} Interface";
    constexpr static char TYPE_OBJECT[] = "{} Object Reference";
    constexpr static char TYPE_SOFT_OBJECT[] = "{} Soft Object Reference";
    constexpr static char TYPE_WEAK_OBJECT[] = "{} Weak Object Reference";
    constexpr static char TYPE_CLASS[] = "{} Class Reference";
    constexpr static char TYPE_SOFT_CLASS[] = "{} Soft Class Reference";
    constexpr static char TYPE_ENUM[] = "{} Enum";
    constexpr static char TYPE_DEFAULT[] = "default";

    std::map<ObjectReference, std::string> ObjectReferenceToString;
    std::map<PrimitiveType::PrimitiveType, std::string>
        PrimitiveTypeToString;

    std::function<void(TypeExporter&)> Callback;

    std::shared_ptr<DocNode> Referencee;
    std::string Type;

    const DocPathsCache& DocPathCache;
    const std::unordered_map<std::string, std::string> &TypeImages;
  };

 public:
  explicit UTypeMarkdownSerializer(const DocPathsCache& DocPathCache);

  void Serialize(BlueprintTypeNode &Type) override;

  void SetTypeImages(
      const std::unordered_map<std::string, std::string> &TypeImages);

 private:
  const DocPathsCache& DocPathCache;
  std::unordered_map<std::string, std::string> TypeImages;
};
}  // namespace udocs_processor
