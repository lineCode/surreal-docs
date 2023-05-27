/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <inja/inja.hpp>
#include <inja/environment.hpp>
#include "udocs-processor/doctree/blueprints/types/BlueprintObjectTypeNode.h"
#include "udocs-processor/doctree/blueprints/types/BlueprintPrimitiveTypeNode.h"
#include "udocs-processor/doctree/blueprints/BlueprintFunctionNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"
#include "UTypeToImagesSerializer.h"

namespace udocs_processor {
class UTypeHTMLSerializer : public UTypeToImagesSerializer {
  class TypeExporter : public DocNodeVisitor {
   public:
    struct ExportedType {
      std::string MainType;
      std::string SecondaryType;
      std::string Structure;
    };

    TypeExporter();

    explicit TypeExporter(std::function<void(TypeExporter&)> Callback);

    int Visit(BlueprintArrayTypeNode &Node) override;

    int Visit(BlueprintEnumTypeNode &Node) override;

    int Visit(BlueprintInterfaceTypeNode &Node) override;

    int Visit(BlueprintMapTypeNode &Node) override;

    int Visit(BlueprintObjectTypeNode &Node) override;

    int Visit(BlueprintPrimitiveTypeNode &Node) override;

    int Visit(BlueprintSetTypeNode &Node) override;

    int Visit(BlueprintStructTypeNode &Node) override;

    int Visit(BlueprintDelegateTypeNode &Node) override;

    const std::vector<ExportedType>& GetExportedTypes() const;

   private:
    constexpr static char TYPE_STRUCTURE_ARRAY[] = "array";
    constexpr static char TYPE_STRUCTURE_SET[] = "set";
    constexpr static char TYPE_STRUCTURE_MAP[] = "map";
    constexpr static char TYPE_STRUCTURE_MAPKEY[] = "mapkey";
    constexpr static char TYPE_STRUCTURE_MAPVALUE[] = "mapvalue";
    constexpr static char TYPE_STRUCTURE_PLAIN[] = "plain";
    constexpr static char TYPE_STRUCTURE_INTERFACE[] = "interface";
    constexpr static char TYPE_STRUCTURE_ACTOR[] = "actor";
    constexpr static char TYPE_STRUCTURE_COMPONENT[] = "component";
    constexpr static char TYPE_STRUCTURE_OBJECT[] = "object";
    constexpr static char TYPE_STRUCTURE_WIDGET[] = "object";
    constexpr static char TYPE_STRUCTURE_EVENT_DISPATCHER[] =
        "event-dispatcher";
    constexpr static char TYPE_STRUCTURE_DELEGATE[] = "delegate";

    constexpr static char TYPE_BOOL[] = "bool";
    constexpr static char TYPE_EVENT_DISPATCHER[] = "event-dispatcher";
    constexpr static char TYPE_BYTE[] = "byte";
    constexpr static char TYPE_INT[] = "integer";
    constexpr static char TYPE_INT64[] = "integer64";
    constexpr static char TYPE_FLOAT[] = "float";
    constexpr static char TYPE_DOUBLE[] = "double";
    constexpr static char TYPE_NAME[] = "name";
    constexpr static char TYPE_STRING[] = "string";
    constexpr static char TYPE_TEXT[] = "text";
    constexpr static char TYPE_VECTOR[] = "vector";
    constexpr static char TYPE_ROTATOR[] = "rotator";
    constexpr static char TYPE_TRANSFORM[] = "transform";
    constexpr static char TYPE_STRUCT[] = "struct";
    constexpr static char TYPE_INTERFACE[] = "interface";
    constexpr static char TYPE_OBJECT[] = "object";
    constexpr static char TYPE_SOFT_OBJECT[] = "soft-object";
    constexpr static char TYPE_CLASS[] = "class";
    constexpr static char TYPE_SOFT_CLASS[] = "soft-class";
    constexpr static char TYPE_ENUM[] = "enum";
    constexpr static char TYPE_DEFAULT[] = "default";

    std::map<ObjectReference, std::string> ObjectReferenceToString;
    std::map<ClassType, std::string> ClassTypeToString;
    std::map<PrimitiveType::PrimitiveType, std::string>
        PrimitiveTypeToString;

    std::function<void(TypeExporter&)> Callback;

    std::vector<ExportedType> ExportedTypes;
  };

 public:
  UTypeHTMLSerializer();

  void Serialize(BlueprintTypeNode &Type) override;

  const std::string &GetTypeTemplate() const;

  void SetTypeTemplate(std::string TypeTemplate);

  std::string GetFileExtension() const override;

 private:
  using InjaArgsStripped = std::vector<const nlohmann::json*>&;
  using InjaArgs = const InjaArgsStripped;

  nlohmann::json MakeResource(InjaArgs Args) const;

  constexpr static char STRUCTURE[] = "structure";
  constexpr static char MAIN[] = "main";
  constexpr static char SECONDARY[] = "secondary";

  constexpr static char RESOURCE_CALLABLE[] = "resource";
  constexpr static int RESOURCE_CALLABLE_ARGS_COUNT = 1;

  constexpr static char FILE_EXTENSION[] = ".html";
  constexpr static char NAME_SEPARATOR[] = "_";

  constexpr static char DIRECTORY_SEPARATOR = '/';

  std::string TypeTemplate;
  inja::Environment Environment{};
};
}  // namespace udocs_processor
