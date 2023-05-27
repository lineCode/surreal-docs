/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>
#include <inja/inja.hpp>
#include <inja/environment.hpp>
#include "udocs-processor/doctree/blueprints/types/BlueprintObjectTypeNode.h"
#include "udocs-processor/doctree/blueprints/types/BlueprintPrimitiveTypeNode.h"
#include "UFunctionSerializer.h"
#include "udocs-processor/doctree/blueprints/BlueprintFunctionNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"

namespace udocs_processor {
class TypeExporter : public DocNodeVisitor {
 public:
  explicit TypeExporter(const std::string& DefaultValue);

  TypeExporter(const std::string& DefaultValue,
      std::function<void(TypeExporter&)> Callback);

  int Visit(BlueprintArrayTypeNode &Node) override;

  int Visit(BlueprintEnumTypeNode &Node) override;

  int Visit(BlueprintInterfaceTypeNode &Node) override;

  int Visit(BlueprintMapTypeNode &Node) override;

  int Visit(BlueprintObjectTypeNode &Node) override;

  int Visit(BlueprintPrimitiveTypeNode &Node) override;

  int Visit(BlueprintSetTypeNode &Node) override;

  int Visit(BlueprintStructTypeNode &Node) override;

  int Visit(BlueprintDelegateTypeNode &Node) override;

  const std::string &GetMainType() const;

  const std::string &GetSecondaryType() const;

  const std::string &GetStructure() const;

  bool IsChecked() const;

  const nlohmann::json &GetVector() const;

  const std::string &GetText() const;

  bool IsReference() const;

 private:
  nlohmann::json MakeVector(double X, double Y, double Z) const;

  std::string GetObjectNameFromPath(const std::string& Path) const;

  void ExtractBaseInfo(BlueprintTypeNode& Node);

  constexpr static char TYPE_STRUCTURE_ARRAY[] = "array";
  constexpr static char TYPE_STRUCTURE_SET[] = "set";
  constexpr static char TYPE_STRUCTURE_MAP[] = "map";
  constexpr static char TYPE_STRUCTURE_DEFAULT[] = "default";
  constexpr static char TYPE_STRUCTURE_BOOL[] = "bool";
  constexpr static char TYPE_STRUCTURE_CLASS[] = "class";
  constexpr static char TYPE_STRUCTURE_ENUM[] = "enum";
  constexpr static char TYPE_STRUCTURE_VECTOR[] = "xyz";
  constexpr static char TYPE_STRUCTURE_TEXT[] = "text";
  constexpr static char TYPE_STRUCTURE_DELEGATE[] = "delegate";

  constexpr static char TYPE_BOOL[] = "bool";
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

  constexpr static char PARAM_VALUE_VECTOR_X[] = "x";
  constexpr static char PARAM_VALUE_VECTOR_Y[] = "y";
  constexpr static char PARAM_VALUE_VECTOR_Z[] = "z";

  constexpr static char TRUE_LITERAL[] = "true";

  constexpr static char PATH_OBJECT_SEPARATOR[] = ".";

  std::map<ObjectReference, std::string> ObjectReferenceToString;
  std::map<PrimitiveType::PrimitiveType, std::string>
      PrimitiveTypeToString;

  std::function<void(TypeExporter&)> Callback;

  const std::string& DefaultValue;
  bool mIsReference = false;
  std::string MainType;
  std::string SecondaryType;
  std::string Structure;
  bool mIsChecked = false;
  nlohmann::json Vector;
  std::string Text;
};

class UFunctionHTMLSerializer : public UFunctionSerializer {
 public:
  UFunctionHTMLSerializer();

  std::string Serialize(BlueprintFunctionNode &Function) override;

  const std::string &GetFunctionTemplate() const;

  void SetFunctionTemplate(std::string FunctionTemplate);

  bool IsTrial() const;

  void SetIsTrial(bool IsTrial);

  std::string GetFileExtension() const override;

 private:
  using InjaArgsStripped = std::vector<const nlohmann::json*>&;
  using InjaArgs = const InjaArgsStripped;

  nlohmann::json ExportParameters(BlueprintFunctionNode &Function,
      bool ShouldBeReturn) const;

  nlohmann::json MakeResource(InjaArgs Args) const;

  constexpr static char IS_COMPACT_FUNCTION[] = "is_compact";
  constexpr static char IS_DELEGATE[] = "is_delegate";
  constexpr static char IS_BP_DELEGATE[] = "is_bp_delegate";
  constexpr static char IS_EVENT[] = "is_event";
  constexpr static char IS_CONSTRUCTION_SCRIPT[] = "is_ctor";
  constexpr static char IS_FUNCTION[] = "is_function";
  constexpr static char IS_COSMETIC[] = "is_cosmetic";
  constexpr static char IS_AUTHORITY_ONLY[] = "is_authority_only";
  constexpr static char IS_RELIABLE[] = "is_reliable";
  constexpr static char IS_FUNCTION_LIBRARY[] = "is_function_library";
  constexpr static char IS_PURE[] = "is_pure";
  constexpr static char RPC[] = "rpc";
  constexpr static char RPC_CLIENT[] = "client";
  constexpr static char RPC_SERVER[] = "server";
  constexpr static char RPC_MULTICAST[] = "multicast";
  constexpr static char RPC_NONE[] = "none";
  constexpr static char CLASS[] = "class";
  constexpr static char COMPACT_TITLE[] = "compact_title";
  constexpr static char IS_TRIAL[] = "is_trial";
  constexpr static char PARAMS[] = "params";
  constexpr static char OUT_PARAMS[] = "out_params";
  constexpr static char NAME[] = "name";

  constexpr static char ANY_PARAM_TYPE[] = "type";
  constexpr static char ANY_PARAM_TYPE_STRUCTURE[] = "structure";
  constexpr static char ANY_PARAM_TYPE_IS_REFERENCE[] = "is_ref";
  constexpr static char ANY_PARAM_TYPE_MAIN[] = "main";
  constexpr static char ANY_PARAM_TYPE_SECONDARY[] = "secondary";
  constexpr static char ANY_PARAM_NAME[] = "name";
  constexpr static char PARAM_VALUE[] = "value";
  constexpr static char PARAM_VALUE_IS_CHECKED[] = "is_checked";
  constexpr static char PARAM_VALUE_TEXT[] = "text";
  constexpr static char PARAM_VALUE_VECTOR[] = "vector";

  constexpr static char RESOURCE_CALLABLE[] = "resource";
  constexpr static int RESOURCE_CALLABLE_ARGS_COUNT = 1;

  constexpr static char DIRECTORY_SEPARATOR = '/';

  constexpr static char CONSTRUCTION_SCRIPT_NAME[] = "ConstructionScript";

  constexpr static char FILE_EXTENSION[] = ".html";

  std::map<RpcMode, std::string> RpcModeToString;
  std::string FunctionTemplate;
  bool mIsTrial = false;
  inja::Environment Environment{};
};
}  // namespace udocs_processor
