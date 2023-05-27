/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <spdlog/logger.h>
#include <memory>
#include <utility>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include "Subprocessor.h"

namespace udocs_processor {
class CrossReferencer;

class ReferenceCache {
 public:
  using CacheType = std::multimap<std::string, std::weak_ptr<DocNode>>;
  using CacheEntryIterator = CacheType::iterator;

  void AddEntry(std::string Reference, std::weak_ptr<DocNode> Referencee);

  std::pair<CacheEntryIterator, CacheEntryIterator> GetReferencees(
      const std::string& Reference);

 private:
  CacheType Cache;
};
/*
 * Types of refs:
 * - usual native thing: ns1::ns2::name -> ns1::ns2::name, ns2::name, name
 * - native legacy UE enum ns1::EEnum::Type -> ns1::EEnum::Type, EEnum::Type,
 * ns1::EEnum, EEnum
 * - UE reflection-exposed thing: /Path/Dir/Package.Object -> Object, [P]Object,
 * /Path/Dir/Package.Object; [P] only for native-defined objects
 * - /Path/Dir/Package.Object:function -> function, function(), Object.function,
 * Object.function()
 * /Path/Dir/Package.Object:property ->
 * */
class ReferenceCacheBuilder : public Subprocessor {
 public:
  explicit ReferenceCacheBuilder(
      std::function<void(ReferenceCacheBuilder&)> Callback);

  ReferenceCache* GetCache();

  void After() override;

  std::string_view GetName() const override;

  int Visit(DelegateTypeNode &Node) override;

  int Visit(LogCategoryNode &Node) override;

  int Visit(NativeClassNode &Node) override;

  int Visit(NativeEnumElementNode &Node) override;

  int Visit(NativeEnumNode &Node) override;

  int Visit(NativeFunctionNode &Node) override;

  int Visit(NativeModuleNode &Node) override;

  int Visit(NativePropertyNode &Node) override;

  int Visit(BlueprintClassNode &Node) override;

  int Visit(BlueprintEnumElementNode &Node) override;

  int Visit(BlueprintEnumNode &Node) override;

  int Visit(BlueprintFunctionNode &Node) override;

  int Visit(BlueprintModuleNode &Node) override;

  int Visit(BlueprintPropertyNode &Node) override;

  int Visit(BlueprintStructNode &Node) override;

 private:
  void AddCacheEntry(std::string Reference, std::weak_ptr<DocNode> Referencee);

  std::vector<std::string> SplitNamespace(const std::string& Name) const;

  void GenerateAllReferenceForNativeNode(
      const std::vector<std::string>& Namespaces, const std::string& Name,
      std::weak_ptr<DocNode> Node);

  // function, property, enum element
  void GenerateAllReferencesForUMember(
      std::weak_ptr<BlueprintUNode> Referencee);

  void GenerateAllReferencesForUMember(
      std::weak_ptr<BlueprintUNode> Referencee,
      std::vector<std::string>& References,
      const std::vector<std::string>& Prefixes);

  // class, struct, enum
  void GenerateAllReferencesForUContainer(
      std::weak_ptr<BlueprintUNode> Referencee);

  void AddBulkOfReferences(
      std::weak_ptr<BlueprintUNode> Referencee,
      const std::string& Reference,
      std::vector<std::string>& References,
      const std::vector<std::string>& Prefixes);

  constexpr static char UDOCS_PROCESSOR_LOGGER_NAME[] = "udocs-processor";
  constexpr static char NAMESPACE_SEPARATOR[] = "::";
  constexpr static char FUNCTION_REFERENCE_POSTFIX[] = "()";
  constexpr static char SUBPROCESSOR_NAME[] = "Reference Cache Builder";
  constexpr static char UNODES_MEMBER_SEPARATOR[] = ".";
  constexpr static char UNODES_FULL_REFERENCE_MEMBER_SEPARATOR[] = ":";
  constexpr static char UPARAMETER_SEPARATOR[] = "#";

  std::function<void(ReferenceCacheBuilder&)> Callback;
  std::unique_ptr<ReferenceCache> Cache;
  CrossReferencer* mCrossReferencer = nullptr;
  std::shared_ptr<spdlog::logger> l;
};
}  // namespace udocs_processor
