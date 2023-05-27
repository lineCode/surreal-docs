/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <spdlog/spdlog.h>
#include <md4c.h>
#include <string>
#include <set>
#include <map>
#include <unordered_set>
#include <vector>
#include <unordered_map>
#include <memory>
#include <nlohmann/json.hpp>
#include "TreeLoader.h"
#include "udocs-processor/doctree/docs/DocsImageNode.h"
#include "udocs-processor/doctree/docs/DocsPageNode.h"
#include "udocs-processor/serializers/markdown/SurrealMarkdown.h"
#include "udocs-processor/FileDataLoader.h"

namespace udocs_processor {
class FileDataLoader;
class DocsIndexNode;
class DocsImageNode;
class DocsPageNode;
class DocsGroupNode;

namespace extract_data {
int EnteredBlock(MD_BLOCKTYPE Type, void* Detail, void* Userdata);

int ExitedBlock(MD_BLOCKTYPE Type, void* Detail, void* Userdata);

int EnteredSpan(MD_SPANTYPE Type, void* Detail, void* Userdata);

int ExitedSpan(MD_SPANTYPE Type, void* Detail, void* Userdata);

int Text(MD_TEXTTYPE Type, const MD_CHAR* Text, MD_SIZE Size, void* Userdata);
}  // namespace extract_data

class PagesTreeLoader : public TreeLoader {
 public:
  class MarkdownDataExtractor : public SurrealMarkdown {
   public:
    friend int extract_data::EnteredBlock(
        MD_BLOCKTYPE Type, void *Detail, void *Userdata);
    friend int extract_data::ExitedBlock(
        MD_BLOCKTYPE Type, void *Detail, void *Userdata);
    friend int extract_data::EnteredSpan(
        MD_SPANTYPE Type, void *Detail, void *Userdata);
    friend int extract_data::ExitedSpan(
        MD_SPANTYPE Type, void *Detail, void *Userdata);
    friend int extract_data::Text(
        MD_TEXTTYPE Type, const MD_CHAR *Text, MD_SIZE Size, void *Userdata);

    void OnBlockEnter(const SurrealBlock &Block,
        ParametersValues Arguments) override;

    void OnMarkdown(const std::string &Markdown) override;

    void OnEOF() override;

    bool IsEmpty() const;

    bool DoExpand() const;

    std::string & GetTitle();

    std::vector<std::string> & GetContents();

   private:
    int ReturnCode() const;

    constexpr static char SPACE = ' ';
    constexpr static int TITLE_HEADING_LEVEL = 1;

    bool mIsEmpty = true;
    bool mDoExpand = false;
    bool mIsInH1 = false;
    bool IsTitleGathered = false;
    std::string Title;
    std::vector<std::string> Contents;
  };

  PagesTreeLoader();

  void SetDataLoader(std::unique_ptr<FileDataLoader> DataLoader);

  void LoadTree(IdGenerator &IdGen, std::weak_ptr<DocNode> Root) override;

  void SetProjectDirectory(std::string ProjectDirectory);

  std::string_view GetName() const override;

  void AllowImageType(ImageType Type);

  void SetImageMaxSideSize(size_t ImageMaxSideSize);

  void SetMaxImageFileSize(size_t MaxImageFileSize);

  void SetMaxPageFileSize(size_t MaxPageFileSize);

 private:
  static std::string ToLower(const std::string& String);

  std::shared_ptr<DocsIndexNode> LoadIndex(
      IdGenerator &IdGen, std::weak_ptr<DocNode> Root);

  std::shared_ptr<DocsGroupNode> LoadContentGroup(
      IdGenerator &IdGen, std::shared_ptr<DocNode> Parent,
      const nlohmann::json& Group);

  std::shared_ptr<DocsPageNode> LoadPage(
      IdGenerator &IdGen, std::shared_ptr<DocNode> Parent,
      const std::string& FilePath, const nlohmann::json& Page);

  std::shared_ptr<DocsPageNode> MakeExternal(
      IdGenerator &IdGen, std::shared_ptr<DocNode> Parent,
      const std::string& Url, const nlohmann::json& Page) const;

  std::shared_ptr<DocsPageNode> MakeApi(
      IdGenerator &IdGen, std::shared_ptr<DocNode> Parent,
      const nlohmann::json& Page) const;

  bool LoadImages(const std::string& Root,
      IdGenerator &IdGen, std::shared_ptr<DocNode> Parent) const;

  std::shared_ptr<DocsImageNode> LoadImage(IdGenerator &IdGen,
      std::shared_ptr<DocNode> Parent, const std::string& Path,
      const std::string& AbsoluteToImages) const;

  bool LoadInfoFromContent(const std::string& FilePath,
      std::vector<std::string>& Contents, std::string& Title,
      bool& DoExpandContents, bool& IsEmpty) const;

  bool ValidatePng(const std::string& Path) const;

  std::string RetrieveExtension(const std::string &FileName) const;

  void NormalizeDocLink(std::string& Link) const;

  constexpr static char UDOCS_PROCESSOR_LOGGER_NAME[] = "udocs-processor";
  constexpr static char DIRECTORY_SEPARATOR[] = "/";

  constexpr static char SPACE_SYMBOLS[] = " \t\r\n";
  constexpr static char NEWLINE[] = "\n";
  constexpr static char WIN_NEWLINE[] = "\r\n";
  constexpr static char QUOTES[] = "\"";
  constexpr static char EXTENSION_SEPARATOR = '.';

  constexpr static char INDEX_FILE_NAME[] = "surreal.json";

  constexpr static char INDEX_LOGO[] = "logo";
  constexpr static char INDEX_TAG[] = "tag";
  constexpr static char INDEX_VERSION[] = "version";
  constexpr static char INDEX_NAME[] = "name";
  constexpr static char INDEX_WEBSITE[] = "website";
  constexpr static char INDEX_ORGANIZATION[] = "organization";
  constexpr static char INDEX_COPYRIGHT[] = "copyright";
  constexpr static char INDEX_COPYRIGHT_URL[] = "copyright_url";
  constexpr static char INDEX_DOCS_TITLE[] = "docs_title";
  constexpr static char INDEX_LANGUAGE[] = "language";
  constexpr static char INDEX_IMAGES[] = "images_root";
  constexpr static char INDEX_MASTER[] = "master";
  constexpr static char INDEX_CONTENTS[] = "contents";
  constexpr static char INDEX_CONTENTS_GROUP_TITLE[] = "title";
  constexpr static char INDEX_CONTENTS_GROUP_PAGES[] = "pages";
  constexpr static char INDEX_CONTENTS_GROUP_PAGE_PAGE[] = "page";
  constexpr static char INDEX_CONTENTS_GROUP_PAGE_TITLE[] = "title";
  constexpr static char INDEX_CONTENTS_GROUP_PAGE_PINNED[] = "pinned";
  constexpr static char INDEX_CONTENTS_GROUP_PAGE_API[] = "api";

  constexpr static char API_BLUEPRINT[] = "blueprint";
  constexpr static char API_CPP[] = "cpp";

  constexpr static char LANGUAGE_EN_US[] = "en-US";
  constexpr static char DEFAULT_VERSION[] = "1.0.0";
  constexpr static char DEFAULT_NAME[] = "Unnamed Project";
  constexpr static char DEFAULT_MASTER_DOC[] = "index";
  constexpr static char DEFAULT_ORGANIZATION[] = "Unnamed Organization";
  constexpr static char DEFAULT_DOCS_TITLE[] = "Docs";

  constexpr static char EXTENSION_PNG[] = "png";
  constexpr static char EXTENSION_MD[] = "md";

  constexpr static char PROTOCOL_MARKER[] = "://";

  constexpr static char TREELOADER_NAME[] = "Pages Tree Loader";

  constexpr static int PNG_SIGNATURE[] =
      {0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a};
  constexpr static int PNG_SIGNATURE_OFFSET = 0;
  constexpr static int PNG_IHDR_OFFSET = 12;
  constexpr static int PNG_WIDTH_OFFSET = 16;
  constexpr static int PNG_HEIGHT_OFFSET = 20;
  constexpr static int PNG_SIDE_SIZE = 4;
  constexpr static unsigned BITS_IN_BYTE = 8;
  constexpr static int PNG_HEADER_SIZE = 24;
  constexpr static int PNG_IHDR_TYPE[] = {0x49, 0x48, 0x44, 0x52};

  // key is filepath
  std::unordered_set<std::string> PagesWhoseCanonicalIsDetermined;
  std::unordered_map<std::string, std::shared_ptr<DocsPageNode>>
      CurrentCanonicals;

  size_t PagesCount = 0;

  std::string ProjectDirectory;

  std::unique_ptr<FileDataLoader> DataLoader;
  mutable std::shared_ptr<spdlog::logger> l;

  std::map<std::string, PageType> ApiToPageType;
  std::map<std::string, ImageType> ExtensionsToImageTypes;
  std::map<ImageType, std::function<bool(const std::string&)>>
      ImageTypeValidators;

  // constraints
  size_t ImageMaxSideSize = 0;
  std::set<ImageType> AllowedImageTypes;
  size_t MaxImageFileSize = 0;
  size_t MaxPageFileSize = 0;
};
}  // namespace udocs_processor

