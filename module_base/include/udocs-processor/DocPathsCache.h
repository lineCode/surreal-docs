/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace udocs_processor {
class DocPathsCache {
 public:
  enum class ExportType {
    EXPORTED,  // exported as part of someone elses page
    SEPARATE_PAGE,  // exported on its own page
    IMAGE  // exported image (generated or user-provided), applies to
    // BlueprintFunctionNode and DocsImageNode only
  };

  using IdType = uint64_t;
  using Cache = std::unordered_map<IdType, std::string>;
  using CacheIt = Cache::iterator;

  const Cache &GetCache() const;

  void AddEntry(IdType Id, std::string DocPath);

  void AddForExport(IdType Id, ExportType Type);

  // [not on the separate page]
  bool IsExported(IdType Id) const;

  // [on the separate page]
  bool IsExportedOnSeparatePage(IdType Id) const;

  // generated or user-provided image
  bool IsImageExported(IdType Id) const;

  void Clear();

 private:
  Cache IdToDocPath;
  std::unordered_map<ExportType, std::unordered_set<IdType>> ToBeExported;
};
}  // namespace udocs_processor
