/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <spdlog/spdlog.h>
#include <string>
#include <set>
#include <unordered_set>
#include <vector>
#include <map>
#include <memory>
#include "udocs-processor/doctree/AccessModifier.h"
#include "udocs-processor/doctree/native/NativeFunctionNode.h"
#include "udocs-processor/doctree/native/LogCategoryNode.h"
#include "udocs-processor/doctree/native/DelegateTypeNode.h"
#include "TreeLoader.h"
#include "DoxybookDataLoader.h"

namespace udocs_processor {
class NativeModuleNode;
class NativeClassNode;
class NativeFunctionNode;
class NativePropertyNode;
class NativeMemberNode;
class NativeEnumNode;
class NativeParameterNode;
class ReferencableWordNode;
class LogCategoryNode;
class DelegateTypeNode;
class NativeTypeNode;
class NativeInitializerNode;
class UeInfoNode;

class DoxybookTreeLoader : public TreeLoader {
 public:
  DoxybookTreeLoader();

  void SetJsonLoader(DoxybookDataLoader *JsonLoader);

  void LoadTree(IdGenerator &IdGen, std::weak_ptr<DocNode> Root) override;

  void SetProjectDirectory(std::string ProjectDirectory);

  std::string_view GetName() const override;

 private:
  void DetermineLayout(const nlohmann::json& Node,
      std::map<std::string, std::unordered_set<std::string>>& Layout,
      std::unordered_set<std::string>& CurrentModule) const;

  std::string StripToRefId(const std::string& Url) const;

  bool IsHeaderFileName(const std::string& FileName) const;

  bool IsBuildCsFileName(const std::string& FileName) const;

  nlohmann::json FindRulesClassInBuildCs(const nlohmann::json& Node) const;

  std::string ExtractNameFromFullName(
      const std::string& FullName) const;

  void ExtractNameAndNamespaceFromFullName(
      const std::string& FullName, std::string& Name,
      std::string& Namespace) const;

  std::string ExtractFileNameFromPath(const std::string& Path) const;

  void ExtractFileNameAndDirFromPath(const std::string& Path,
      std::string& Directory, std::string& FileName) const;

  std::string StripPathToRelative(const std::string& Path,
      const std::string& Directory) const;

  std::string ConvertHeaderPathToInclude(const std::string& Path) const;

  std::shared_ptr<NativeModuleNode> LoadModule(
      std::weak_ptr<DocNode> Root, IdGenerator &IdGen,
      const std::string& ModuleRefId,
      const std::unordered_set<std::string>& Headers);

  void LoadChunk(std::shared_ptr<DocNode> Parent, IdGenerator &IdGen,
      const std::string& RefId);

  void LoadChunk(std::shared_ptr<DocNode> Parent, IdGenerator &IdGen,
      nlohmann::json Chunk);

  /* Class chunk is always something that is struct/class in c++ code, but
   * it can represent:
   * - delegate
   * - log category
   * - UINTERFACE
   * - UCLASS
   * - USTRUCT
   * - or just c++ class/struct - NATIVE_CLASS
   * */
  enum class ClassChunkType {
    LOG_CATEGORY,
    DELEGATE,
    INTERFACE,
    NATIVE_CLASS,
    CLASS,
    STRUCT
  };

  std::shared_ptr<NativeMemberNode> LoadClassChunk(
      std::weak_ptr<DocNode> Parent, IdGenerator &IdGen,
      const std::string& RefId);

  std::shared_ptr<NativeFunctionNode> LoadFunction(
      std::weak_ptr<DocNode> Parent, IdGenerator &IdGen,
      const nlohmann::json &FunctionJson);

  std::shared_ptr<NativePropertyNode> LoadProperty(
      std::weak_ptr<DocNode> Parent, IdGenerator &IdGen,
      const nlohmann::json &PropertyJson);

  std::shared_ptr<NativeMemberNode> LoadType(
      std::weak_ptr<DocNode> Parent, IdGenerator &IdGen,
      const nlohmann::json &TypeJson, const nlohmann::json& ParentNamespace);

  std::shared_ptr<NativeEnumNode> LoadEnum(
      std::weak_ptr<DocNode> Parent, IdGenerator &IdGen,
      const nlohmann::json &EnumJson, const nlohmann::json& ParentNamespace);

  std::shared_ptr<NativeClassNode> LoadClass(
      std::shared_ptr<NativeMemberNode> BasicInfo,
      std::weak_ptr<DocNode> Parent, IdGenerator &IdGen,
      nlohmann::json Chunk, ClassChunkType IsInterface);

  std::shared_ptr<LogCategoryNode> LoadLogCategory(
      std::shared_ptr<NativeMemberNode> BasicInfo,
      std::weak_ptr<DocNode> Parent, IdGenerator &IdGen,
      nlohmann::json LogCategoryJson);

  std::shared_ptr<DelegateTypeNode> LoadDelegate(
      std::shared_ptr<NativeMemberNode> BasicInfo,
      std::weak_ptr<DocNode> Parent, IdGenerator &IdGen,
      nlohmann::json DelegateJson, bool IsUEntity);

  std::vector<std::shared_ptr<NativeParameterNode>> LoadParameters(
      std::weak_ptr<DocNode> Parent, IdGenerator &IdGen,
      const nlohmann::json &ParamsList, const nlohmann::json &Params);

  std::shared_ptr<NativeTypeNode> LoadNativeType(const std::string& Type,
      std::weak_ptr<DocNode> Parent, IdGenerator &IdGen);

  std::shared_ptr<NativeInitializerNode> LoadNativeInitializer(
      const std::string& Initializer, std::weak_ptr<DocNode> Parent,
      IdGenerator &IdGen);

  void AddRefs(std::shared_ptr<DocNode> Parent, const std::string& Text,
      IdGenerator &IdGen) const;

  bool LoadBasicInfo(IdGenerator &IdGen,
      std::shared_ptr<NativeMemberNode> NativeMember,
      const nlohmann::json& MemberJson);

  std::vector<std::shared_ptr<UeInfoNode>> ExtractUeInfo(
      const std::string& Text, std::weak_ptr<DocNode> Parent,
      IdGenerator &IdGen, std::string& CleanedText);

  std::shared_ptr<UeInfoNode> MakeUeInfo(
      std::weak_ptr<DocNode> Parent, IdGenerator &IdGen,
      std::string Name, std::string Value);

  std::shared_ptr<ReferencableWordNode> MakeWord(
      std::weak_ptr<DocNode> Parent, IdGenerator &IdGen,
      std::string Word, bool ShouldBeReferenced = false) const;

  std::string ExtractUeType(DocNode& Node) const;

  std::string Public(const std::string& MemberType) const;

  std::string Private(const std::string& MemberType) const;

  std::string Protected(const std::string& MemberType) const;

  std::vector<std::string> AllAccess(const std::string& MemberType) const;

  std::set<nlohmann::json> AllAccessJson(
      const nlohmann::json& Chunk, const std::string& MemberType) const;

  std::string Opening(const std::string& Tag) const;

  std::string Closing(const std::string& Tag) const;

  std::string Unquote(const std::string& Text) const;

  std::string ExtractDefaultValueFromAssignInitializer(
      const std::string& Initializer) const;

  std::shared_ptr<NativeModuleNode> FindModule(
      std::shared_ptr<DocNode> Node) const;

  std::set<std::string> GetAllRefIdsOfMemberType(
      const std::string& MemberType, const nlohmann::json& Container) const;

  AccessModifier ConvertStringToAccessModifier(const std::string& Access) const;

  udocs_processor::NativeFunctionType ConvertStringToFunctionType(
      const std::string &FunctionName, const std::string &ClassName,
      NativeFunctionNode &Function) const;

  // for functions and properties
  bool CheckIfUMemberIsDeprecated(const std::string& Name,
      std::string& CleanName) const;

  bool CheckIfUClassIsDeprecated(const std::string &Name) const;

  bool EndsWith(std::string const &FullString, std::string const &Ending) const;

  bool StartsWith(std::string const &FullString,
      std::string const &Starting) const;

  std::string MakeReturnDescription(const nlohmann::json& Return) const;

  bool IsAnonymous(const std::string& Name) const;

  bool IsBlueprintNativeEvent(NativeFunctionNode& Node) const;

  static bool StringToBool(const std::string& String);

  static std::string ToLower(const std::string& String);

  constexpr static char UDOCS_PROCESSOR_LOGGER_NAME[] = "udocs-processor";
  constexpr static char KIND_NAMESPACE[] = "namespace";
  constexpr static char KIND_CLASS[] = "class";
  constexpr static char KIND_STRUCT[] = "struct";
  constexpr static char KIND_ENUM[] = "enum";
  constexpr static char KIND_FILE[] = "file";
  constexpr static char CHUNK_KIND[] = "kind";
  constexpr static char MANIFEST_KIND[] = "kind";
  constexpr static char MANIFEST_URL[] = "url";
  constexpr static char MANIFEST_NAME[] = "name";
  constexpr static char MANIFEST_CHILDREN[] = "children";

  constexpr static char EXTENSION_SEPARATOR[] = ".";

  constexpr static char BUILD_CS[] = ".build.cs";

  constexpr static char HEADER_H[] = ".h";
  constexpr static char HEADER_HPP[] = ".hpp";
  constexpr static char HEADER_HXX[] = ".hxx";
  constexpr static char HEADER_HH[] = ".hh";
  constexpr static char HEADER_HPLPL[] = ".h++";

  constexpr static char BUILD_CS_FULLNAME[] = "fullname";
  constexpr static char CLASS_DEPRECATED[] = "deprecated";
  constexpr static char CLASS_FULLNAME[] = "fullname";
  constexpr static char CLASS_NAME[] = "name";
  constexpr static char CLASS_REFID[] = "refid";
  constexpr static char CLASS_DETAILS[] = "details";
  constexpr static char CLASS_BASE_CLASSES[] = "baseClasses";
  constexpr static char CLASS_BASE_CLASS_NAME[] = "name";
  constexpr static char CLASS_BASE_CLASS_VIRTUAL[] = "virtual";
  constexpr static char CLASS_IS_FINAL[] = "final";
  constexpr static char CLASS_VISIBILITY[] = "visibility";
  constexpr static char CLASS_TEMPLATE_PARAMETERS[] = "templateParams";
  constexpr static char CLASS_PARAMETERS_LIST[] = "paramList";
  constexpr static char CLASS_RETURN[] = "returns";

  constexpr static char ENUM_IS_STRONG[] = "strong";
  constexpr static char ENUM_VISIBILITY[] = "visibility";
  constexpr static char ENUM_TYPE[] = "type";
  constexpr static char ENUM_ELEMENTS[] = "enumvalues";
  constexpr static char ENUM_ELEMENT_INITIALIZER[] = "initializer";

  constexpr static char DIRECTORY_SEPARATOR[] = "/";

  constexpr static char PUBLIC_MEMBER[] = "public";
  constexpr static char PROTECTED_MEMBER[] = "protected";
  constexpr static char PRIVATE_MEMBER[] = "private";

  constexpr static char MEMBERS_CLASSES[] = "Classes";
  constexpr static char MEMBERS_ATTRIBUTES[] = "Attributes";
  constexpr static char MEMBERS_FUNCTIONS[] = "Functions";
  constexpr static char MEMBERS_TYPES[] = "Types";
  constexpr static char MEMBERS_NAMESPACES[] = "namespaces";

  constexpr static char MEMBER_FULLNAME[] = "fullname";
  constexpr static char MEMBER_BRIEF[] = "brief";
  constexpr static char MEMBER_DETAILS[] = "details";
  constexpr static char MEMBER_REFID[] = "refid";
  constexpr static char MEMBER_API[] = "api";
  constexpr static char MEMBER_SEE[] = "see";
  constexpr static char MEMBER_LOCATION[] = "location";
  constexpr static char MEMBER_ACCESS[] = "visibility";
  constexpr static char MEMBER_LOCATION_FILE[] = "file";
  constexpr static char MEMBER_DEPRECATED[] = "deprecated";
  constexpr static char MEMBER_SINCE[] = "since";

  constexpr static char FUNCTION_NAME[] = "name";
  constexpr static char FUNCTION_TEMPLATE_PARAMETERS[] = "templateParams";
  constexpr static char FUNCTION_PARAMETERS_LIST[] = "paramList";
  constexpr static char FUNCTION_PARAMETERS[] = "params";
  constexpr static char FUNCTION_RETURN_TYPE[] = "type";
  constexpr static char FUNCTION_RETURN[] = "returns";
  constexpr static char FUNCTION_IS_DEFAULTED[] = "default";
  constexpr static char FUNCTION_IS_DELETED[] = "deleted";
  constexpr static char FUNCTION_ARGS_STRING[] = "argsString";
  constexpr static char FUNCTION_VIRTUAL[] = "virtual";
  constexpr static char FUNCTION_PURE_VIRTUAL[] = "pureVirtual";
  constexpr static char FUNCTION_CONST[] = "const";
  constexpr static char FUNCTION_INLINE[] = "inline";
  constexpr static char FUNCTION_FINAL[] = "final";
  constexpr static char FUNCTION_STATIC[] = "static";
  constexpr static char FUNCTION_EXPLICIT[] = "explicit";

  constexpr static char PROPERTY_STATIC[] = "static";
  constexpr static char PROPERTY_CONST[] = "const";
  constexpr static char PROPERTY_INITIALIZER[] = "initializer";
  constexpr static char PROPERTY_TYPE[] = "type";

  constexpr static char VIRTUALITY_NON_VIRTUAL[] = "non-virtual";
  constexpr static char VIRTUALITY_VIRTUAL[] = "virtual";

  constexpr static char TAG_UE[] = "ue";
  constexpr static char TAG_UEITEM[] = "ue-item";
  constexpr static char TAG_UEITEM_NAME[] = "name";
  constexpr static char TAG_UEITEM_VALUE[] = "value";
  constexpr static char TAG_REF[] = "ref";

  constexpr static char OPENING_TAG_BEGINNING[] = "<";
  constexpr static char OPENING_TAG_ENDING[] = ">";
  constexpr static char CLOSING_TAG_BEGINNING[] = "</";
  constexpr static char CLOSING_TAG_ENDING[] = ">";

  constexpr static char PARAM_NAME[] = "name";
  constexpr static char PARAM_TYPE[] = "type";
  constexpr static char PARAM_DEFAULT_VALUE[] = "defval";
  constexpr static char PARAM_DESCRIPTION[] = "description";

  constexpr static char PARAM_FROM_LIST_NAME[] = "name";
  constexpr static char PARAM_FROM_LIST_DESCRIPTION[] = "text";

  constexpr static char UE_INFO_NAME[] = "name";
  constexpr static char UE_INFO_MACRO[] = "macro";
  constexpr static char UE_INFO_TYPE[] = "type";
  constexpr static char UE_INFO_TYPE_INTERFACE[] = "interface";
  constexpr static char UE_INFO_TYPE_DELEGATE[] = "delegate";
  constexpr static char UE_INFO_TYPE_LOG_CATEGORY[] = "log_category";
  constexpr static char UE_INFO_TYPE_ENUM[] = "enum";
  constexpr static char UE_INFO_TYPE_CLASS[] = "class";
  constexpr static char UE_INFO_TYPE_STRUCT[] = "struct";
  constexpr static char UE_INFO_TYPE_FUNCTION[] = "function";
  constexpr static char UE_INFO_TYPE_PROPERTY[] = "property";
  constexpr static char UE_INFO_TYPE_PARAMETER[] = "param";

  constexpr static char MACRO_UMETA[] = "umeta";
  constexpr static char MACRO_UPARAM[] = "uparam";
  constexpr static char MACRO_UENUM[] = "uenum";
  constexpr static char MACRO_UINTERFACE[] = "uinterface";
  constexpr static char MACRO_UPROPERTY[] = "uproperty";
  constexpr static char MACRO_UFUNCTION[] = "ufunction";
  constexpr static char MACRO_UDELEGATE[] = "udelegate";
  constexpr static char MACRO_USTRUCT[] = "ustruct";
  constexpr static char MACRO_UCLASS[] = "uclass";

  constexpr static char UE_INFO_SPECIFIER[] = "specifier";
  constexpr static char UE_INFO_SPECIFIER_NATIVE_EVENT[] =
      "blueprintnativeevent";
  constexpr static char UE_INFO_META_SPECIFIER[] = "meta-specifier";
  constexpr static char UE_INFO_DEPRECATION_MESSAGE[] = "deprecated_message";
  constexpr static char UE_INFO_DEPRECATION_VERSION[] = "deprecated_version";
  constexpr static char UE_INFO_DELEGATE_IS_EVENT[] = "is_event";
  constexpr static char UE_INFO_DELEGATE_IS_MULTICAST[] = "is_multicast";
  constexpr static char UE_INFO_DELEGATE_IS_DYNAMIC[] = "is_dynamic";
  constexpr static char UE_INFO_DELEGATE_IS_DERIVED_EVENT[] =
      "is_derived_event";
  constexpr static char UE_INFO_DELEGATE_HAS_RETURN_TYPE[] = "has_return_type";
  constexpr static char UE_INFO_DELEGATE_OWNING_TYPE[] = "owning_type";
  constexpr static char UE_INFO_DELEGATE_SUPER_TYPE[] = "supertype";
  constexpr static char UE_INFO_LOG_NAME[] = "name";
  constexpr static char UE_INFO_LOG_TYPE[] = "log_category_type";
  constexpr static char UE_INFO_LOG_VERBOSITY[] = "default_verbosity";
  constexpr static char UE_INFO_LOG_COMPILE_VERBOSITY[] =
      "compile_time_verbosity";

  constexpr static char NAMESPACE_SEPARATOR[] = "::";
  constexpr static char BUILD_CS_MODULE_RULES_CLASS[] = "ModuleRules";
  constexpr static char ENUM_FIRST_LETTER = 'E';
  constexpr static char ARGUMENTS_ENDING[] = ")";
  constexpr static char ANONYMOUS_MEMBER_NAME_PREFIX[] = "@";
  constexpr static char TREELOADER_NAME[] = "Doxybook Tree Loader";
  constexpr static char UCLASS_DEPRECATED_PREFIX[] = "UDEPRECATED_";
  constexpr static char ACTOR_DEPRECATED_PREFIX[] = "ADEPRECATED_";

  // function(?), property
  constexpr static char UMEMBER_DEPRECATED_POSTFIX[] = "_DEPRECATED";

  constexpr static char SPACE_SYMBOLS[] = " \t\r\n";
  constexpr static char NEWLINE[] = "\n";
  constexpr static char DESCRIPTION_CONCAT_GLUE[] = "\n";
  constexpr static char QUOTES[] = "\"";

  constexpr static char ACCESS_PRIVATE[] = "private";
  constexpr static char ACCESS_PROTECTED[] = "protected";
  constexpr static char ACCESS_PUBLIC[] = "public";

  constexpr static char DESTRUCTOR_STARTS_WITH[] = "~";
  constexpr static char OPERATOR_STARTS_WITH[] = "operator[^0-9a-z_]";
  constexpr static char VALIDATION_ENDS_WITH[] = "_Validate";
  constexpr static char IMPLEMENTATION_ENDS_WITH[] = "_Implementation";
  constexpr static char DELEGATE_STUB_FUNCTION_POSTFIX[] = "_delegate";

  constexpr static char LOG_TYPE_CLASS[] = "class";
  constexpr static char LOG_TYPE_EXTERN[] = "extern";

  constexpr static char LOG_VERBOSITY_FATAL[] = "fatal";
  constexpr static char LOG_VERBOSITY_ERROR[] = "error";
  constexpr static char LOG_VERBOSITY_WARNING[] = "warning";
  constexpr static char LOG_VERBOSITY_DISPLAY[] = "display";
  constexpr static char LOG_VERBOSITY_LOG[] = "log";
  constexpr static char LOG_VERBOSITY_VERBOSE[] = "verbose";
  constexpr static char LOG_VERBOSITY_VERY_VERBOSE[] = "veryverbose";
  constexpr static char LOG_VERBOSITY_ALL[] = "all";

  constexpr static char TRUE_LITERAL[] = "true";
  constexpr static char FALSE_LITERAL[] = "false";

  constexpr static char REPLICATED_USING[] = "replicatedusing";

  std::set<std::string> HeaderExtensions;
  std::set<std::string> ReflectionMacros;
  std::map<std::string, AccessModifier> StringToAccessModifier;
  std::map<std::string, LogCategoryType> StringToLogCategoryType;
  std::map<std::string, LogVerbosity> StringToLogVerbosity;
  std::map<std::string, ClassChunkType> StringToClassChunkType;

  std::unordered_set<std::string> LoadedRefIds;

  std::string ProjectDirectory;

  DoxybookDataLoader* JsonLoader = nullptr;
  std::shared_ptr<spdlog::logger> l;
};
}  // namespace udocs_processor

