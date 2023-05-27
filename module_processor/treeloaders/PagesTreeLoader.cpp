/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#define NOLINT()

#include <md4c.h>
#include <memory>
#include <utility>
#include <filesystem>
#include <regex> NOLINT()
#include <stringtoolbox.hpp>
#include "udocs-processor/StringHelper.h"
#include "udocs-processor/treeloaders/PagesTreeLoader.h"
#include "udocs-processor/report/ProcessingReport.h"
#include "udocs-processor/IdGenerator.h"
#include "udocs-processor/doctree/docs/DocsImageNode.h"
#include "udocs-processor/doctree/docs/DocsGroupNode.h"
#include "udocs-processor/doctree/docs/DocsIndexNode.h"
#include "udocs-processor/doctree/docs/DocsPageNode.h"

template<typename T, nlohmann::detail::value_t Type>
T j(const nlohmann::json& Json, const std::string& FieldName,
    T DefaultValue) {
  if (!Json.is_object()) {
    return DefaultValue;
  }

  auto It = Json.find(FieldName);
  if (It != Json.end() && It->type() == Type) {
    return *It;
  } else {
    return DefaultValue;
  }
}

inline std::string jstr(const nlohmann::json& Json,
    const std::string& FieldName, const std::string& DefaultValue) {
  return j<std::string, nlohmann::detail::value_t::string>(Json, FieldName,
      DefaultValue);
}

inline nlohmann::json jobj(const nlohmann::json& Json,
    const std::string& FieldName) {
  return j<nlohmann::json, nlohmann::detail::value_t::object>(Json, FieldName,
    nlohmann::json::object());
}

inline nlohmann::json jarr(const nlohmann::json& Json,
    const std::string& FieldName) {
  return j<nlohmann::json, nlohmann::detail::value_t::array>(Json, FieldName,
      nlohmann::json::array());
}

inline bool jbol(const nlohmann::json& Json,
    const std::string& FieldName, bool DefaultValue) {
  return j<bool, nlohmann::detail::value_t::boolean>(Json, FieldName,
      DefaultValue);
}

udocs_processor::PagesTreeLoader::PagesTreeLoader() {
  l = spdlog::get(UDOCS_PROCESSOR_LOGGER_NAME);

  ExtensionsToImageTypes.insert(
      std::make_pair(EXTENSION_PNG, ImageType::PNG));

  ImageTypeValidators.insert(
      std::make_pair(ImageType::PNG,
      [this](const std::string& Path) {
        return ValidatePng(Path);
      }));

  ApiToPageType.insert(
      std::make_pair(API_CPP, PageType::API_CPP));
  ApiToPageType.insert(
      std::make_pair(API_BLUEPRINT, PageType::API_BLUEPRINT));
}

void udocs_processor::PagesTreeLoader::SetProjectDirectory(
    std::string ProjectDirectory) {
  this->ProjectDirectory = std::move(ProjectDirectory);
}

std::string udocs_processor::PagesTreeLoader::ToLower(
    const std::string& String) {
  std::string Copy = String;
  std::transform(Copy.begin(), Copy.end(),
      Copy.begin(), ::tolower);
  return Copy;
}

void udocs_processor::PagesTreeLoader::SetDataLoader(
    std::unique_ptr<FileDataLoader> DataLoader) {
  this->DataLoader = std::move(DataLoader);
}

std::string_view udocs_processor::PagesTreeLoader::GetName() const {
  return TREELOADER_NAME;
}

void udocs_processor::PagesTreeLoader::LoadTree(
    IdGenerator &IdGen, std::weak_ptr<DocNode> Root) {
  if (!DataLoader) {
    l->error("Data loader is not set. Exiting");
    GetReport()->AddLogMessage(StructuredLogType::Fatal,
        "Couldn't process pages");

    return;
  }

  std::shared_ptr<DocNode> RootShared = Root.lock();

  std::shared_ptr<DocsIndexNode> Index = LoadIndex(IdGen, Root);
  RootShared->AddNode(Index);
}

void udocs_processor::PagesTreeLoader::SetImageMaxSideSize(
    size_t ImageMaxSideSize) {
  this->ImageMaxSideSize = ImageMaxSideSize;
}

void udocs_processor::PagesTreeLoader::SetMaxImageFileSize(
    size_t MaxImageFileSize) {
  this->MaxImageFileSize = MaxImageFileSize;
}

void udocs_processor::PagesTreeLoader::SetMaxPageFileSize(
    size_t MaxPageFileSize) {
  this->MaxPageFileSize = MaxPageFileSize;
}

void udocs_processor::PagesTreeLoader::AllowImageType(ImageType Type) {
  AllowedImageTypes.emplace(Type);
}

std::shared_ptr<udocs_processor::DocsIndexNode>
    udocs_processor::PagesTreeLoader::LoadIndex(
    IdGenerator &IdGen, std::weak_ptr<DocNode> Root) {
  std::string IndexString = DataLoader->LoadText(INDEX_FILE_NAME);

  std::shared_ptr<DocsIndexNode> Index =
      std::make_shared<DocsIndexNode>(Root, IdGen.ObtainId());

  nlohmann::json IndexJson = IndexString.empty()
      ? nlohmann::json{}
      : nlohmann::json::parse(IndexString);

  Index->SetLanguage(LANGUAGE_EN_US);  // currently, en-US only
  Index->SetTag(jstr(IndexJson, INDEX_TAG, ""));
  Index->SetVersion(
      jstr(IndexJson, INDEX_VERSION, DEFAULT_VERSION));
  Index->SetName(
      jstr(IndexJson, INDEX_NAME, DEFAULT_NAME));
  Index->SetWebSite(
      jstr(IndexJson, INDEX_WEBSITE, ""));
  Index->SetOrganization(
      jstr(IndexJson, INDEX_ORGANIZATION, DEFAULT_ORGANIZATION));
  Index->SetCopyright(
      jstr(IndexJson, INDEX_COPYRIGHT, ""));
  Index->SetCopyrightUrl(
      jstr(IndexJson, INDEX_COPYRIGHT_URL, ""));
  Index->SetDocsTitle(
      jstr(IndexJson, INDEX_DOCS_TITLE, DEFAULT_DOCS_TITLE));
  std::string MasterDoc =
      jstr(IndexJson, INDEX_MASTER, DEFAULT_MASTER_DOC);
  NormalizeDocLink(MasterDoc);
  Index->SetMasterPage(std::move(MasterDoc));

  if (IndexJson.contains(INDEX_IMAGES)) {
    std::string ImagesRoot = jstr(IndexJson, INDEX_IMAGES, "");

    bool IsValidRoot = LoadImages(ImagesRoot, IdGen, Index);
    if (IsValidRoot) {
      std::string Wd = DataLoader->GetWorkingDirectory();
      DataLoader->SetWorkingDirectory(DataLoader->ToAbsolute(ImagesRoot));

      ImagesRoot = DataLoader->ToRelative(ImagesRoot);
      NormalizeDocLink(ImagesRoot);

      std::string LogoPath = jstr(IndexJson, INDEX_LOGO, "");
      if (DataLoader->DoesExistAndUnderWorkingDirectory(LogoPath)) {
        DataLoader->SetWorkingDirectory(std::move(Wd));
        LogoPath = DataLoader->ToRelative(LogoPath);
        LogoPath = ImagesRoot + std::string(DIRECTORY_SEPARATOR + LogoPath);
        NormalizeDocLink(LogoPath);
        // logo path is normalized, relative to bundle/docs path that is under
        // images root
        Index->SetLogoPath(LogoPath);
      } else {
        DataLoader->SetWorkingDirectory(std::move(Wd));
      }

      std::string Message = fmt::format(
          "Images root is set to `{}`", ImagesRoot);
      l->warn(Message);
      GetReport()->AddLogMessage(StructuredLogType::Info,
          std::move(Message));

      Index->SetImagesRoot(std::move(ImagesRoot));
    }
  } else {
    l->warn("Images root is not provided");
  }

  for (const auto& ContentGroup : jarr(IndexJson, INDEX_CONTENTS)) {
    std::shared_ptr<DocsGroupNode> Group =
        LoadContentGroup(IdGen, Index, ContentGroup);
    Index->AddNode(Group);
  }

  std::string Report = fmt::format("Total {} pages were loaded", PagesCount);
  l->info(Report);
  GetReport()->AddLogMessage(StructuredLogType::Info,
      std::move(Report));

  return Index;
}

void udocs_processor::PagesTreeLoader::NormalizeDocLink(
    std::string& Link) const {
  // remove leading and trailing slashes
  if (!Link.empty()) {
    Link.erase(0, Link.find_first_not_of(DIRECTORY_SEPARATOR));
    Link.erase(Link.find_last_not_of(DIRECTORY_SEPARATOR) + 1);
  }
}

bool udocs_processor::PagesTreeLoader::LoadImages(const std::string& Root,
    IdGenerator &IdGen, std::shared_ptr<DocNode> Parent) const {
  if (DataLoader->DoesExistAndUnderWorkingDirectory(Root)) {
    std::string WorkingDirectory = DataLoader->GetWorkingDirectory();
    std::string AbsoluteToImages = DataLoader->ToAbsolute(Root);
    DataLoader->SetWorkingDirectory(AbsoluteToImages);

    std::vector<std::string> Files = DataLoader->FindAllFiles("", true);

    for (const auto &File : Files) {
      auto It = ExtensionsToImageTypes.find(RetrieveExtension(File));
      if (It != ExtensionsToImageTypes.end()) {
        auto It2 = ImageTypeValidators.find(It->second);
        if (It2 != ImageTypeValidators.end()) {
          if (It2->second(File)) {
            std::shared_ptr<DocsImageNode> Image =
                LoadImage(IdGen, Parent,
                    Root + DIRECTORY_SEPARATOR + File,
                    DataLoader->Directory(AbsoluteToImages));
            Parent->AddNode(Image);
          } else {
            l->warn("`{}` failed the validation", File);
          }
        }
      }
    }

    DataLoader->SetWorkingDirectory(WorkingDirectory);

    return true;
  } else {
    return false;
  }
}

// Extension will be lowercased
std::string udocs_processor::PagesTreeLoader::RetrieveExtension(
    const std::string &FileName) const {
  std::string ActualExtension;

  if (size_t Pos = FileName.rfind(EXTENSION_SEPARATOR);
      Pos != std::string::npos &&
      /*Pos isn't last symbol*/ Pos + 1 < FileName.size()) {
      ActualExtension = FileName.substr(Pos + 1);

      std::transform(ActualExtension.begin(), ActualExtension.end(),
          ActualExtension.begin(), ::tolower);

      return ActualExtension;
  }

  return "";
}

std::shared_ptr<udocs_processor::DocsGroupNode>
    udocs_processor::PagesTreeLoader::LoadContentGroup(
        IdGenerator &IdGen, std::shared_ptr<DocNode> Parent,
        const nlohmann::json &Group) {
  std::shared_ptr<DocsGroupNode> DocsGroup =
      std::make_shared<DocsGroupNode>(Parent, IdGen.ObtainId());

  DocsGroup->SetTitle(
      jstr(Group, INDEX_CONTENTS_GROUP_TITLE, ""));

  for (const auto& Page : jarr(Group, INDEX_CONTENTS_GROUP_PAGES)) {
    std::string PageStr =
        jstr(Page, INDEX_CONTENTS_GROUP_PAGE_PAGE, "");
    if (!PageStr.empty()) {
      if (PageStr.find(PROTOCOL_MARKER) != std::string::npos) {
        std::shared_ptr<DocsPageNode> External =
            MakeExternal(IdGen, DocsGroup, PageStr, Page);
        DocsGroup->AddNode(External);
      } else {
        std::shared_ptr<DocsPageNode> PageNode =
            LoadPage(IdGen, DocsGroup, PageStr, Page);
        // PageNode can be null if the page isn't a valid page
        if (PageNode) {
          DocsGroup->AddNode(PageNode);
        }
      }
    } else {
      std::shared_ptr<DocsPageNode> Api =
          MakeApi(IdGen, DocsGroup, Page);
      // Api can be null if the page isn't an api ref
      if (Api) {
        DocsGroup->AddNode(Api);
      }
    }
  }

  return DocsGroup;
}

std::shared_ptr<udocs_processor::DocsPageNode>
    udocs_processor::PagesTreeLoader::MakeExternal(
    IdGenerator &IdGen, std::shared_ptr<DocNode> Parent,
    const std::string& Url, const nlohmann::json& Page) const {
  std::shared_ptr<DocsPageNode> External =
      std::make_shared<DocsPageNode>(Parent, IdGen.ObtainId());

  External->SetDocPath(Url);
  External->SetType(PageType::EXTERNAL);
  External->SetTitle(
      jstr(Page, INDEX_CONTENTS_GROUP_PAGE_TITLE, Url));

  return External;
}

std::shared_ptr<udocs_processor::DocsPageNode>
    udocs_processor::PagesTreeLoader::MakeApi(
    IdGenerator &IdGen, std::shared_ptr<DocNode> Parent,
    const nlohmann::json& Page) const {
  std::string Api =
      jstr(Page, INDEX_CONTENTS_GROUP_PAGE_API, "");

  if (!Api.empty()) {
    if (auto It = ApiToPageType.find(Api); It != ApiToPageType.end()) {
      std::shared_ptr<DocsPageNode> ApiNode =
          std::make_shared<DocsPageNode>(Parent, IdGen.ObtainId());

      ApiNode->SetType(It->second);

      return ApiNode;
    }
  }

  return nullptr;
}

std::shared_ptr<udocs_processor::DocsPageNode>
    udocs_processor::PagesTreeLoader::LoadPage(
        IdGenerator &IdGen, std::shared_ptr<DocNode> Parent,
        const std::string& FilePath /* path as in contents*/,
        const nlohmann::json& Page) {
  // full path to the file
  std::string PageFilePath = FilePath + EXTENSION_SEPARATOR + EXTENSION_MD;
  if (DataLoader->DoesExistAndUnderWorkingDirectory(PageFilePath) &&
      DataLoader->IsFile(PageFilePath) &&
      DataLoader->LoadSizeOfFile(PageFilePath) <= MaxPageFileSize) {
    // extract name
    // full absolute path to file
    std::string PagePath = DataLoader->ToAbsolute(PageFilePath);
    std::string OriginalPageName = DataLoader->FileName(PagePath, false);

    std::filesystem::path DirectoryOfPage = DataLoader->Directory(PagePath);

    std::shared_ptr<DocsPageNode> PageNode =
        std::make_shared<DocsPageNode>(Parent, IdGen.ObtainId());

    // basic data
    PageNode->SetName(OriginalPageName);
    bool IsPinned =
        jbol(Page, INDEX_CONTENTS_GROUP_PAGE_PINNED, false);
    PageNode->SetIsPinned(IsPinned);
    PageNode->SetFileReference(PagePath);
    // not really a doc path but more
    // of doc path part
    // PageNode->SetDocPath(std::move(OriginalPageName));

    // is canonical
    bool IsCanonical = false;
    if (PagesWhoseCanonicalIsDetermined.find(PagePath) ==
        PagesWhoseCanonicalIsDetermined.end()) {
      // no true canonical was found
      IsCanonical = true;
      auto CurrentCanonical = CurrentCanonicals.find(PagePath);
      if (IsPinned) {
        if (CurrentCanonical == CurrentCanonicals.end()) {
          // first pinned
          CurrentCanonicals.insert(
              std::make_pair(PagePath, PageNode));
        } else {
          // second+ pinned
          IsCanonical = false;
        }
      } else {
        // first non-pinned - true canonical
        PagesWhoseCanonicalIsDetermined.emplace(std::move(PagePath));

        if (CurrentCanonical != CurrentCanonicals.end()) {
          CurrentCanonical->second->SetIsCanonical(false);
        }
      }
    }
    PageNode->SetIsCanonical(IsCanonical);

    // data from content
    bool IsEmpty = false;
    bool DoExpandContents = false;
    std::string Title;
    std::vector<std::string> Contents;

    bool IsLoaded = LoadInfoFromContent(PageFilePath, Contents, Title,
        DoExpandContents, IsEmpty);

    if (IsLoaded) {
      PageNode->SetTitle(std::move(Title));
      PageNode->SetDoExpandContents(DoExpandContents);
      PageNode->SetIsEmpty(IsEmpty);

      // go subpages
      if (!IsPinned) {  // only single pages can be pinned
        for (const std::string &Entry : Contents) {
          std::string PathToSubPage = (DirectoryOfPage / Entry).string();

          std::shared_ptr<DocsPageNode> SubPage =
              LoadPage(IdGen, PageNode, PathToSubPage, {});
          if (SubPage) {
            PageNode->AddNode(SubPage);
          }
        }
      }
    } else {
      l->error("Unable to load info from MD page `{}`", PageFilePath);
    }

    ++PagesCount;
    return PageNode;
  } else {
    std::string Message = fmt::format(
        "Refusing to load page with file at `{}`. It doesn't exist, not "
        "a file, is too big or isn't located under working directory",
        PageFilePath);
    l->warn(Message);
    GetReport()->AddLogMessage(StructuredLogType::Warning,
        std::move(Message));
  }

  return nullptr;
}

// expects that file path exists
bool udocs_processor::PagesTreeLoader::LoadInfoFromContent(
    const std::string &FilePath, std::vector<std::string>& Contents,
    std::string& Title, bool& DoExpandContents, bool& IsEmpty) const {
  std::string Content = DataLoader->LoadText(FilePath);
  StringHelper::ReplaceAll(Content, WIN_NEWLINE, NEWLINE);

  MarkdownDataExtractor Extractor;
  Extractor.Initialize();
  Extractor.Parse(Content);

  Contents = std::move(Extractor.GetContents());
  Title = std::move(Extractor.GetTitle());
  DoExpandContents = Extractor.DoExpand();
  IsEmpty = Extractor.IsEmpty();

  return true;
}

// expects AbsoluteToImages to be the path to a dir that contains images_root
std::shared_ptr<udocs_processor::DocsImageNode>
    udocs_processor::PagesTreeLoader::LoadImage(IdGenerator &IdGen,
        std::shared_ptr<DocNode> Parent, const std::string &Path,
        const std::string& AbsoluteToImages) const {
  std::shared_ptr<DocsImageNode> Image =
      std::make_shared<DocsImageNode>(Parent, IdGen.ObtainId());

  Image->SetFileReference(AbsoluteToImages + DIRECTORY_SEPARATOR + Path);
  Image->SetDocPath(Path);

  return Image;
}

bool udocs_processor::PagesTreeLoader::ValidatePng(
    const std::string &Path) const {
  if (DataLoader->DoesExistAndUnderWorkingDirectory(Path) &&
      DataLoader->IsFile(Path)) {
    size_t FileSize = DataLoader->LoadSizeOfFile(Path);
    if (FileSize <= MaxImageFileSize && FileSize >= PNG_HEADER_SIZE) {
      size_t LoadedBytes = 0;
      std::unique_ptr<char[]> Header(
          DataLoader->LoadBytes(Path, PNG_HEADER_SIZE, LoadedBytes));

      if (LoadedBytes >= PNG_HEADER_SIZE) {
        for (size_t i = 0; i < sizeof(PNG_SIGNATURE) / sizeof(int); ++i) {
          if (static_cast<unsigned char>(Header[PNG_SIGNATURE_OFFSET + i]) !=
              PNG_SIGNATURE[i]) {
            return false;
          }
        }

        for (size_t i = 0; i < sizeof(PNG_IHDR_TYPE) / sizeof(int); ++i) {
          if (Header[PNG_IHDR_OFFSET + i] != PNG_IHDR_TYPE[i]) {
            return false;
          }
        }

        size_t Width = 0;
        size_t Height = 0;

        for (size_t i = 0; i < PNG_SIDE_SIZE; ++i) {
          Width <<= BITS_IN_BYTE;
          Height <<= BITS_IN_BYTE;

          Width |= static_cast<unsigned char>(Header[PNG_WIDTH_OFFSET + i]);
          Height |= static_cast<unsigned char>(Header[PNG_HEIGHT_OFFSET + i]);
        }

        if (Width >= ImageMaxSideSize || Height >= ImageMaxSideSize) {
          return false;
        }

        return true;
      }
    }
  }

  return false;
}

void udocs_processor::PagesTreeLoader::MarkdownDataExtractor::OnBlockEnter(
    const SurrealBlock &Block, ParametersValues Arguments) {
  SurrealMarkdown::OnBlockEnter(Block, Arguments);

  if (Block.GetName() != INDEX_CONTENTS) {
    // if found any block except contents then consider page non-empty
    mIsEmpty = false;
  } else {
    auto It = Arguments.find(CONTENTS_DO_EXPAND);
    if (It != Arguments.end() && std::any_cast<bool>(It->second)) {
      mDoExpand = true;
    }
  }
}

void udocs_processor::PagesTreeLoader::MarkdownDataExtractor::OnMarkdown(
    const std::string &Markdown) {
  SurrealMarkdown::OnMarkdown(Markdown);

  if (DoAcceptMarkdown() && (!mIsEmpty || !IsTitleGathered)) {
    MD_PARSER Parser = {
        0,
        RecommendedMd4cParserFlags,
        extract_data::EnteredBlock,
        extract_data::ExitedBlock,
        extract_data::EnteredSpan,
        extract_data::ExitedSpan,
        extract_data::Text,
        nullptr,
        nullptr
    };

    md_parse(Markdown.c_str(), Markdown.size(), &Parser,
        static_cast<void*>(this));
  }
}

void udocs_processor::PagesTreeLoader::MarkdownDataExtractor::OnEOF() {
  Contents = ExtractContents();
}

namespace udocs_processor::extract_data {
int EnteredBlock(MD_BLOCKTYPE Type, void* Detail, void* Userdata) {
  using ExtractorType = PagesTreeLoader::MarkdownDataExtractor;
  if (auto Extractor = static_cast<ExtractorType*>(Userdata)) {
    if (!Extractor->IsTitleGathered && Type == MD_BLOCK_H) {
      auto* Heading = static_cast<MD_BLOCK_H_DETAIL*>(Detail);

      if (Heading->level == ExtractorType::TITLE_HEADING_LEVEL) {
        Extractor->mIsInH1 = true;
      }
    }

    return Extractor->ReturnCode();
  }

  return 0;
}

int ExitedBlock(MD_BLOCKTYPE Type, void* Detail, void* Userdata) {
  using ExtractorType = PagesTreeLoader::MarkdownDataExtractor;
  if (auto Extractor = static_cast<ExtractorType*>(Userdata)) {
    if (Type == MD_BLOCK_H) {
      auto* Heading = static_cast<MD_BLOCK_H_DETAIL*>(Detail);

      if (Heading->level == ExtractorType::TITLE_HEADING_LEVEL) {
        Extractor->mIsInH1 = false;
        Extractor->IsTitleGathered = true;
      }
    } else {
      Extractor->mIsEmpty = false;
    }

    return Extractor->ReturnCode();
  }

  return 0;
}

int EnteredSpan(MD_SPANTYPE Type, void* Detail, void* Userdata) {
  using ExtractorType = PagesTreeLoader::MarkdownDataExtractor;
  if (auto Extractor = static_cast<ExtractorType*>(Userdata)) {
    if (!Extractor->mIsInH1) {
      Extractor->mIsEmpty = false;
    }

    return Extractor->ReturnCode();
  }

  return 0;
}

int ExitedSpan(MD_SPANTYPE Type, void* Detail, void* Userdata) {
  using ExtractorType = PagesTreeLoader::MarkdownDataExtractor;
  if (auto Extractor = static_cast<ExtractorType*>(Userdata)) {
    return Extractor->ReturnCode();
  }

  return 0;
}

int Text(MD_TEXTTYPE Type, const MD_CHAR* Text, MD_SIZE Size, void* Userdata) {
  using ExtractorType = PagesTreeLoader::MarkdownDataExtractor;
  if (auto Extractor = static_cast<ExtractorType*>(Userdata)) {
    if (Extractor->mIsInH1) {
      switch (Type) {
        case MD_TEXT_NULLCHAR:
          // replacement character
          Extractor->Title += StringHelper::REPLACEMENT_HTML_CODE;
          break;
        case MD_TEXT_BR:
        case MD_TEXT_SOFTBR:
          Extractor->Title += ExtractorType::SPACE;
          break;
        case MD_TEXT_HTML:
        case MD_TEXT_ENTITY:
          Extractor->Title += std::string(Text, Size);
          break;
        default:
          Extractor->Title +=
              StringHelper::EscapeHtml(std::string(Text, Size));
          break;
      }
    } else {
      Extractor->mIsEmpty = false;
    }

    return Extractor->ReturnCode();
  }

  return 0;
}
}  // namespace udocs_processor::extract_data

bool udocs_processor::PagesTreeLoader::MarkdownDataExtractor::IsEmpty() const {
  return mIsEmpty;
}

bool udocs_processor::PagesTreeLoader::MarkdownDataExtractor::DoExpand() const {
  return mDoExpand;
}

std::string &
udocs_processor::PagesTreeLoader::MarkdownDataExtractor::GetTitle() {
  return Title;
}

std::vector<std::string> &
udocs_processor::PagesTreeLoader::MarkdownDataExtractor::GetContents() {
  return Contents;
}

int udocs_processor::PagesTreeLoader::
    MarkdownDataExtractor::ReturnCode() const {
  // if either not empty or title wasn't yet gathered -> keep searching
  // otherwise -> stop parsing
  return !mIsEmpty || !IsTitleGathered ? 0 : 1;
}
