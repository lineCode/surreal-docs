/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/cli/commands/generate/rendering/UDocsDocumentRenderer.h" NOLINT()
#include <libbase64.h>
#include <udocs-processor/serializers/HTMLDocTreeSerializer.h>
#include "udocs-processor/serializers/JSONDocTreeSerializer.h"
#include "udocs-processor/cli/commands/generate/rendering/Templater.h"
#include "udocs-processor/StringHelper.h"

udocs_processor::UDocsDocumentRenderer::UDocsDocumentRenderer(
    std::shared_ptr<spdlog::sinks::sink> Sink) : Sink(Sink) {}

// UrlPrefix: isntance/project/ver/lang
std::string udocs_processor::UDocsDocumentRenderer::Render(
    const std::string& Content, const nlohmann::json& IndexData,
    ManifestEntry::Type Type, ManifestEntry::Format Format) {
  switch (Type) {
    case udocs_processor::ManifestEntry::Type::PAGE:
      if (Format == udocs_processor::ManifestEntry::Format::JSON_V1) {
        return RenderPageJsonV1(Content, IndexData);
      } else {
        throw std::invalid_argument{"Unsupported format of page document"};
      }
    default:
      throw std::invalid_argument{"Invalid type of document"};
  }
}

std::string udocs_processor::UDocsDocumentRenderer::RenderPageJsonV1(
    const std::string& Content, const nlohmann::json &IndexData) const {
  nlohmann::json Page = nlohmann::json::parse(Content);

  std::unique_ptr<udocs_processor::HTMLDocTreeSerializer> HtmlSerializer =
      std::make_unique<udocs_processor::HTMLDocTreeSerializer>(Sink);

  // trailing slash just in case
  HtmlSerializer->SetPageTemplatePath(ResourcesPath +
      DIRECTORY_SEPARATOR + PAGE_TEMPLATE_PATH);
  HtmlSerializer->SetUrlPrefix(LINKS_URL_PREFIX);
  HtmlSerializer->SetTreeTemplatePath(ResourcesPath +
      DIRECTORY_SEPARATOR + TREE_TEMPLATE_PATH);
  HtmlSerializer->SetTimeAgoTemplatePath(ResourcesPath +
      DIRECTORY_SEPARATOR + TIME_AGO_TEMPLATE_PATH);

  std::string Version;
  if (IndexData.contains(INDEX_VERSION)) {
    Version = IndexData[INDEX_VERSION];
  }

  udocs_processor::DynamicPageData Dynamic;
  Dynamic.SetLatestVersion(Version);

  std::string DocPath = Page[PAGE_DOCPATH];
  std::vector<std::string> VersionsAvailable;
  Dynamic.SetAvailableVersions(std::move(VersionsAvailable));
  std::pair<udocs_processor::DynamicPageData::LinkData,
            udocs_processor::DynamicPageData::LinkData> PrevAndNext =
      GetPrevAndNext(IndexData, DocPath);
  Dynamic.SetPreviousPage(PrevAndNext.first);
  Dynamic.SetNextPage(PrevAndNext.second);
  // set now
  Dynamic.SetLastUpdated(std::chrono::duration_cast<std::chrono::seconds>(
      std::chrono::system_clock::now().time_since_epoch()).count());

  std::unique_ptr<udocs_processor::SurrealMarkdownToHtmlConverter>
      MarkdownProcessor =
      std::make_unique<udocs_processor::SurrealMarkdownToHtmlConverter>();

  std::unique_ptr<udocs_processor::IsLinkInternalDecider> IsInternalDecider1 =
      std::make_unique<udocs_processor::IsLinkInternalDecider>();

  std::unique_ptr<udocs_processor::IsLinkInternalDecider> IsInternalDecider2 =
      std::make_unique<udocs_processor::IsLinkInternalDecider>(
          *IsInternalDecider1);

  HtmlSerializer->SetIsLocalBuild(true);
  HtmlSerializer->SetResourcesPath(LOCALRES_PATH);
  HtmlSerializer->SetIsInternalUrlDecider(
      std::move(IsInternalDecider1));
  MarkdownProcessor->SetIsInternalUrlDecider(
      std::move(IsInternalDecider2));
  MarkdownProcessor->SetIsLocalBuild(true);
  HtmlSerializer->SetSurrealMarkdownToHtmlConverter(
      std::move(MarkdownProcessor));

  return HtmlSerializer->Serialize(Page, IndexData, Dynamic);
}

std::pair<udocs_processor::DynamicPageData::LinkData,
          udocs_processor::DynamicPageData::LinkData>
          udocs_processor::UDocsDocumentRenderer::GetPrevAndNext(
    const nlohmann::json& Index, const std::string& CurrentDocPath) const {
  std::vector<udocs_processor::DynamicPageData::LinkData> FlattenTree;
  size_t CurrentPageIndex = ~0;

  for (const auto& Tree : Index[INDEX_TREES]) {
    size_t CurrentIndex = ~0;
    AppendFlattenTree(FlattenTree, Tree, CurrentDocPath, CurrentIndex);

    if (CurrentIndex != ~0 && CurrentPageIndex == ~0) {
      CurrentPageIndex = CurrentIndex;
    }
  }

  std::pair<udocs_processor::DynamicPageData::LinkData,
            udocs_processor::DynamicPageData::LinkData> Data;
  if (CurrentPageIndex != ~0) {
    if (CurrentPageIndex > 0) {
      Data.first = std::move(FlattenTree[CurrentPageIndex - 1]);
    }

    if (CurrentPageIndex < FlattenTree.size() - 1) {
      Data.second = std::move(FlattenTree[CurrentPageIndex + 1]);
    }
  }

  return Data;
}

void udocs_processor::UDocsDocumentRenderer::AppendFlattenTree(
    std::vector<udocs_processor::DynamicPageData::LinkData>& FlattenTree,
    const nlohmann::json& Tree, const std::string& CurrentDocPath,
    size_t& CurrentPage) const {
  bool SetCurrent = false;
  for (const auto& Child : Tree[INDEX_TREE_CHILDREN]) {
    size_t CurrentIndex = ~0;
    TraverseTreeChild(FlattenTree, Child, CurrentDocPath,
        CurrentIndex, FlattenTree.size());

    if (CurrentIndex != ~0 && !SetCurrent) {
      CurrentPage = CurrentIndex;
      SetCurrent = true;
    }
  }
}

// expects CurrentPage to be ~0
void udocs_processor::UDocsDocumentRenderer::TraverseTreeChild(
    std::vector<udocs_processor::DynamicPageData::LinkData>&
    FlattenTree, const nlohmann::json& Child,
    const std::string& CurrentDocPath, size_t& CurrentPage,
    size_t CurrentLength) const {
  std::string DocPath = Child[INDEX_TREE_CHILD_URL];
  if (DocPath == CurrentDocPath &&
      Child[INDEX_TREE_CHILD_IS_CANONICAL].get<bool>() &&
      CurrentPage == ~0) {
    CurrentPage = CurrentLength;
  }

  FlattenTree.emplace_back(udocs_processor::DynamicPageData::LinkData{
    Child[INDEX_TREE_CHILD_TITLE], DocPath});

  for (const auto& SubChild : Child[INDEX_TREE_CHILD_CHILDREN]) {
    TraverseTreeChild(FlattenTree, SubChild, CurrentDocPath, CurrentPage,
        FlattenTree.size());
  }
}

void udocs_processor::UDocsDocumentRenderer::SetResourcesPath(
    std::string ResourcesPath) {
  this->ResourcesPath = StringHelper::Normalize(ResourcesPath);
}
