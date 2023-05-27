/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <udocs-processor/serializers/DynamicPageData.h>
#include <utility>
#include <memory>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include "udocs-processor/serializers/JSONDocTreeSerializer.h"

namespace udocs_processor {
class UDocsDocumentRenderer {
 public:
  explicit UDocsDocumentRenderer(std::shared_ptr<spdlog::sinks::sink> Sink);

  std::string Render(const std::string& Content,
      const nlohmann::json& IndexData, ManifestEntry::Type Type,
      ManifestEntry::Format Format);

  void SetResourcesPath(std::string ResourcesPath);

 private:
  std::string RenderPageJsonV1(const std::string& Content,
      const nlohmann::json& IndexData) const;

  std::pair<udocs_processor::DynamicPageData::LinkData,
      udocs_processor::DynamicPageData::LinkData> GetPrevAndNext(
      const nlohmann::json& Index, const std::string& CurrentDocPath) const;

  void AppendFlattenTree(
      std::vector<udocs_processor::DynamicPageData::LinkData>&
          FlattenTree, const nlohmann::json& Tree,
          const std::string& CurrentDocPath, size_t& CurrentPage) const;

  void TraverseTreeChild(
      std::vector<udocs_processor::DynamicPageData::LinkData>&
          FlattenTree, const nlohmann::json& Child,
          const std::string& CurrentDocPath, size_t& CurrentPage,
          size_t CurrentLength) const;

  constexpr static char MIME_RESOURCE_IMAGE_PNG[] = "image/png";
  constexpr static char LOCALRES_PATH[] = "localres/";
  constexpr static char MIME_PAGE[] = "text/html";
  constexpr static char MIME_TEXT[] = "text/plain";
  constexpr static char UNSUPPORTED_FORMAT[] = "Unsupported format";
  constexpr static char UNSUPPORTED_DOCUMENT_TYPE[] =
      "Unsupported type of document";
  constexpr static char INDEX_VERSION[] = "current_version";
  constexpr static char INDEX_TREES[] = "trees";
  constexpr static char INDEX_TREE_CHILDREN[] = "children";
  constexpr static char INDEX_TREE_CHILD_CHILDREN[] = "children";
  constexpr static char INDEX_TREE_CHILD_TITLE[] = "title";
  constexpr static char INDEX_TREE_CHILD_URL[] = "url";
  constexpr static char INDEX_TREE_CHILD_IS_CANONICAL[] = "is_canonical";
  constexpr static char PAGE_DOCPATH[] = "docpath";
  constexpr static char PROTOCOL_SEPARATOR[] = "://";
  constexpr static char URL_SEPARATOR = '/';
  constexpr static char DIRECTORY_SEPARATOR = '/';
  constexpr static char LINKS_URL_PREFIX[] = "";
  constexpr static char PAGE_TEMPLATE_PATH[] =
      R"(html/udocs_processor/theme/theme1/page.html)";
  constexpr static char TREE_TEMPLATE_PATH[] =
      R"(html/udocs_processor/theme/theme1/sidebar/tree.html)";
  constexpr static char TIME_AGO_TEMPLATE_PATH[] =
      R"(html/udocs_processor/theme/theme1/util/time_ago.html)";

  std::string ResourcesPath;
  std::shared_ptr<spdlog::sinks::sink> Sink;
};
}  // namespace udocs_processor
