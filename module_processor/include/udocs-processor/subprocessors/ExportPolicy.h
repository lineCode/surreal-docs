/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <spdlog/logger.h>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <memory>
#include "Subprocessor.h"
#include "udocs-processor/DocPathsCache.h"

namespace udocs_processor {
// this polict determine what entities should be in the final documentation
class ExportPolicy : public Subprocessor {
 public:
  explicit ExportPolicy(DocPathsCache &DocPathCache);

  int Visit(DelegateTypeNode &Node) override;

  int Visit(LogCategoryNode &Node) override;

  int Visit(NativeClassNode &Node) override;

  int Visit(NativeEnumNode &Node) override;

  int Visit(NativeFunctionNode &Node) override;

  int Visit(NativeParameterNode &Node) override;

  int Visit(NativeEnumElementNode &Node) override;

  int Visit(NativeModuleNode &Node) override;

  int Visit(NativePropertyNode &Node) override;

  int Visit(BlueprintEnumElementNode &Node) override;

  int Visit(BlueprintPropertyNode &Node) override;

  int Visit(BlueprintClassNode &Node) override;

  int Visit(BlueprintEnumNode &Node) override;

  int Visit(BlueprintFunctionNode &Node) override;

  int Visit(BlueprintParameterNode &Node) override;

  int Visit(BlueprintModuleNode &Node) override;

  int Visit(BlueprintStructNode &Node) override;

  int Visit(BlueprintDelegateTypeNode &Node) override;

  int Visit(DocsPageNode &Node) override;

  int Visit(DocsImageNode &Node) override;

  void SetDoExportPrivate(bool DoExportPrivate);

  void After() override;

  std::string_view GetName() const override;

 private:
  constexpr static char SUBPROCESSOR_NAME[] = "Export Policy";

  constexpr static char UDOCS_PROCESSOR_LOGGER_NAME[] = "udocs-processor";
  constexpr static char UINT8_TYPE[] = "uint8";
  constexpr static char MAX_POSTFIX[] = " MAX";

  // ids of native delegates corresponding to blueprint delegate types
  std::unordered_set<DocPathsCache::IdType> ProcessedDelegateSignatures;

  bool mDoExportPrivate = false;
  DocPathsCache &DocPathCache;
  mutable std::shared_ptr<spdlog::logger> l;
};
}  // namespace udocs_processor
