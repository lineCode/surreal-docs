/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/DocPathsCache.h"

const udocs_processor::DocPathsCache::Cache &
    udocs_processor::DocPathsCache::GetCache() const {
  return IdToDocPath;
}

void udocs_processor::DocPathsCache::AddEntry(
    IdType Id, std::string DocPath) {
  IdToDocPath.insert(std::make_pair(Id, std::move(DocPath)));
}

void udocs_processor::DocPathsCache::Clear() {
  IdToDocPath.clear();
}

void udocs_processor::DocPathsCache::AddForExport(DocPathsCache::IdType Id,
    ExportType Type) {
  if (auto It = ToBeExported.find(Type);
      It != ToBeExported.end()) {
    It->second.emplace(Id);
  } else {
    ToBeExported.insert(
        std::make_pair(Type, std::unordered_set<IdType>{Id}));
  }
}

bool udocs_processor::DocPathsCache::IsExported(
    DocPathsCache::IdType Id) const {
  if (auto It = ToBeExported.find(ExportType::EXPORTED);
      It != ToBeExported.end() && It->second.find(Id) != It->second.end()) {
    return true;
  }

  return false;
}

bool udocs_processor::DocPathsCache::IsExportedOnSeparatePage(
    DocPathsCache::IdType Id) const {
  if (auto It = ToBeExported.find(ExportType::SEPARATE_PAGE);
      It != ToBeExported.end() && It->second.find(Id) != It->second.end()) {
    return true;
  }

  return false;
}

bool udocs_processor::DocPathsCache::IsImageExported(
    DocPathsCache::IdType Id) const {
  if (auto It = ToBeExported.find(ExportType::IMAGE);
      It != ToBeExported.end() && It->second.find(Id) != It->second.end()) {
    return true;
  }

  return false;
}
