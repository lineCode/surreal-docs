/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <spdlog/spdlog.h>
#include <memory>
#include <vector>
#include <string>
#include <cstdint>
#include <set>
#include <unordered_set>
#include "Subprocessor.h"

namespace udocs_processor {
class ReferenceCache;
class NodeTypeCheckerBase;

/*
 * this class pretty freely operates raw and weak pointers because it's
 * guaranteed that no nodes will be freed while this class processes the tree
 * */
class CrossReferencer : public Subprocessor {
 public:
  enum class ReferenceTypeHint {
    NATIVE_FOR_SURE,
    BP_FOR_SURE,
    UNKNOWN
  };

  CrossReferencer();

  void SetCache(ReferenceCache* Cache);

  std::string_view GetName() const override;

  bool Before() override;

  void After() override;

  int Visit(ReferencableWordNode &Node) override;

  int Visit(DelegateTypeNode &Node) override;

  int Visit(LogCategoryNode &Node) override;

  int Visit(NativeClassNode &Node) override;

  int Visit(NativeEnumNode &Node) override;

  int Visit(NativeFunctionNode &Node) override;

  int Visit(NativeModuleNode &Node) override;

  int Visit(NativePropertyNode &Node) override;

  int Visit(BlueprintClassNode &Node) override;

  int Visit(BlueprintEnumNode &Node) override;

  int Visit(BlueprintFunctionNode &Node) override;

  int Visit(BlueprintModuleNode &Node) override;

  int Visit(BlueprintPropertyNode &Node) override;

  int Visit(BlueprintStructNode &Node) override;

  DocNode* ResolveReference(DocNode& Node,
      NodeTypeCheckerBase* TypeChecker, ReferenceTypeHint Type,
      bool IsNativeContext,
      const std::vector<DocNode*>& PriorityNodes,
      const std::vector<DocNode*>& PriorityParents,
      const std::string& RawName);

 private:
  std::string JoinSet(const std::unordered_set<std::string>& Set) const;

  // get parent class/struct (native/bp) of a node
  std::vector<DocNode*> GetParentClass(DocNode& Node) const;

  void ProcessInlineReferences(bool IsNativeContext,
      const std::vector<DocNode *> &PriorityNodes,
      const std::vector<DocNode *> &PriorityParents,
      DocNode& Node);

  bool TryToResolveInStandardWay(
      DocNode*& Return, bool IsNativeContext,
      const std::set<std::shared_ptr<DocNode>>& Candidates,
      DocNode& Node, const std::string& Name);

  // look for symbols in name that are specific to one of two types of name
  ReferenceTypeHint TryToSpecifyType(
      ReferenceTypeHint Type, const std::string& Name);

  std::string ProcessInlineReferences(DocNode& Node,
      ReferenceTypeHint Type, bool IsNativeContext,
      const std::vector<DocNode *> &PriorityNodes,
      const std::vector<DocNode *> &PriorityParents,
      const std::string& Text);

  enum class InvalidReferenceType {
    AMBIGUOUS,
    NOT_FOUND
  };

  DocNode * AdjustReference(DocNode *ReferencedNode,
      bool IsNativeContext) const;

  void ReportInvalidReference(InvalidReferenceType Reason,
      const std::string& Reference, DocNode& Node);

  bool IsNativeNode(DocNode& Node) const;

  std::string FindHeader(DocNode& Node) const;

  DocNode* FindModule(DocNode& Node) const;

  std::string MakeRef(uint64_t Id, std::string Text) const;

  bool TryToResolveBpEnumInitializer(ReferencableWordNode &Node);

  bool TryToResolveRepNotify(ReferencableWordNode &Node);

  bool TryToResolveSee(ReferencableWordNode &Node);

  constexpr static char REF_TAG_OPENING_1[] = "<ref refid=\"";
  constexpr static char REF_TAG_OPENING_2[] = "\">";
  constexpr static char REF_TAG_CLOSING[] = "</ref>";

  constexpr static char UDOCS_PROCESSOR_LOGGER_NAME[] = "udocs-processor";
  constexpr static char SUBPROCESSOR_NAME[] = "Cross-Referencer";
  constexpr static char NAMESPACE_SEPARATOR[] = "::";
  constexpr static char BP_MEMBER_SEPARATOR[] = ":";
  constexpr static char BP_OBJECT_SEPARATOR_1[] = ".";
  constexpr static char BP_OBJECT_SEPARATOR_2[] = "/";
  constexpr static char INLINE_REFERENCER_PATTERN[] =
      R"-(<ref\s+refc="(.+?)">(.*?)<\/ref>)-";
  constexpr static char INLINE_REFERENCE_PART1[] = R"(<ref refc=")";
  constexpr static char INLINE_REFERENCE_PART2[] = R"(">)";
  constexpr static char INLINE_REFERENCE_PART3[] = R"(</ref>)";

  constexpr static char IINTERFACE_PREFIX[] = "I";
  constexpr static char REPLICATED_USING[] = "replicatedusing";

  std::unordered_set<std::string> AmbiguousReferences;
  std::unordered_set<std::string> NotFoundReferences;
  ReferenceCache* Cache = nullptr;
  std::shared_ptr<spdlog::logger> l;
};
}  // namespace udocs_processor
