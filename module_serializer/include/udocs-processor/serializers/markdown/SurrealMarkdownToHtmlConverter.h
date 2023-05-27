/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <md4c.h>
#include <spdlog/logger.h>
#include <string>
#include <unordered_map>
#include <map>
#include <stack>
#include <memory>
#include <functional>
#include <queue>
#include "SurrealMarkdownConverter.h"
#include "SurrealMarkdown.h"
#include "udocs-processor/serializers/util/IsLinkInternalDecider.h"

namespace udocs_processor {
// this processing could be in a bunch of templates but for maximal performance
// it was left in C++
// also html generated here is supposed to be theme-independent

namespace surreal_markdown_to_html_converter {
int EnteredBlock(MD_BLOCKTYPE Type, void* Detail, void* Userdata);

int ExitedBlock(MD_BLOCKTYPE Type, void* Detail, void* Userdata);

int EnteredSpan(MD_SPANTYPE Type, void* Detail, void* Userdata);

int ExitedSpan(MD_SPANTYPE Type, void* Detail, void* Userdata);

int Text(MD_TEXTTYPE Type, const MD_CHAR* Text, MD_SIZE Size, void* Userdata);
}  // namespace surreal_markdown_to_html_converter

class SurrealMarkdownToHtmlConverter : public SurrealMarkdown,
    public SurrealMarkdownConverter {
 public:
  friend int surreal_markdown_to_html_converter::EnteredBlock(
      MD_BLOCKTYPE Type, void *Detail, void *Userdata);
  friend int surreal_markdown_to_html_converter::ExitedBlock(
      MD_BLOCKTYPE Type, void *Detail, void *Userdata);
  friend int surreal_markdown_to_html_converter::EnteredSpan(
      MD_SPANTYPE Type, void *Detail, void *Userdata);
  friend int surreal_markdown_to_html_converter::ExitedSpan(
      MD_SPANTYPE Type, void *Detail, void *Userdata);
  friend int surreal_markdown_to_html_converter::Text(
      MD_TEXTTYPE Type, const MD_CHAR *Text, MD_SIZE Size, void *Userdata);

  SurrealMarkdownToHtmlConverter();

  std::string Convert(const std::string &SurrealMarkdown) override;

  bool IsInline() const;

  void SetIsInline(bool IsInline);

  void SetIsLocalBuild(bool IsLocalBuild);

  void SetIsInternalUrlDecider(
      std::unique_ptr<IsLinkInternalDecider> IsUrlInternalDecider);

  // add link URL preprocessing
  void AddLinkPrefix(const std::string& Prefix,
      const std::string& Replacement);

  // add image URL preprocessing
  void AddImagePrefix(const std::string& Prefix,
      const std::string& Replacement);

  void ResetLinkPrefixes();

  void ResetImagePrefixes();

 private:
  enum class EscapeAs {
    URL,
    HTML,
    NONE
  };

  enum class MetaMarkType {
    HTML,
    INVALID
  };

  struct HtmlReplacement {
    bool IsStart = false;
    std::string Replacement;
  };

  struct RenderStackEntry {
    std::string Rendered;
    bool DoRenderIt = false;
  };

  void OnEOF() override;

  void ResetRenderStack();

  void Reset();

  void OnBlockEnter(const SurrealBlock &Block,
      ParametersValues Arguments) override;

  void OnBlockExit(const SurrealBlock &Block,
      ParametersValues Arguments) override;

  void OnMarkdown(const std::string &Markdown) override;

  void RenderVerbatim(const std::string& Text);

  void ProcessSurrealMarkdown(const std::string &SurrealMarkdown);

  void RenderMarkdownToHtml(const std::string &Markdown);

  void RenderTabs(const SurrealBlock& Block,
      const ParametersValues& Arguments, bool IsEntered);

  void RenderPage(const SurrealBlock& Block,
      const ParametersValues& Arguments, bool IsEntered);

  void RenderSpoiler(const SurrealBlock& Block,
      const ParametersValues& Arguments, bool IsEntered);

  void RenderAdmonition(const SurrealBlock& Block,
      const ParametersValues& Arguments, bool IsEntered);

  void RenderFigure(const SurrealBlock& Block,
      const ParametersValues& Arguments, bool IsEntered);

  void EnteredBlock(MD_BLOCKTYPE Type, void* Detail);

  void ExitedBlock(MD_BLOCKTYPE Type, void* Detail);

  void EnteredSpan(MD_SPANTYPE Type, void* Detail);

  void ExitedSpan(MD_SPANTYPE Type, void* Detail);

  void Text(MD_TEXTTYPE Type, const MD_CHAR* Text, MD_SIZE Size);

  void RenderOpeningOl(const MD_BLOCK_OL_DETAIL* Detail);

  void RenderOpeningCode(const MD_BLOCK_CODE_DETAIL* Detail);

  void RenderOpeningTd(const std::string& Tag,
      const MD_BLOCK_TD_DETAIL* Detail);

  void RenderOpeningHeading(const MD_BLOCK_H_DETAIL* Detail);

  void RenderClosingHeading(const MD_BLOCK_H_DETAIL* Detail);

  void RenderOpeningA(const MD_SPAN_A_DETAIL* Detail);

  void RenderOpeningImg(const MD_SPAN_IMG_DETAIL* Detail);

  void RenderClosingImg(const MD_SPAN_IMG_DETAIL* Detail);

  std::string EscapeHtml(const std::string& Text) const;

  std::string EscapeUrl(const std::string& Url) const;

  void RenderMetaMark(
      const std::string& GeneralType, const std::string& SubType);

  void RenderHtml(std::string Html, bool IsBeginning);

  MetaMarkType IsMetaMark(const MD_SPAN_A_DETAIL* Detail);

  void ProcessMetaMark(MetaMarkType Type, const MD_SPAN_A_DETAIL* Detail);

  std::string RenderAttributeToString(const MD_ATTRIBUTE* Attribute,
      EscapeAs EscapeMode);

  void RenderAttribute(const MD_ATTRIBUTE* Attribute, EscapeAs EscapeMode);

  void FinalizeHeadings(int Level);

  std::string MarkdownLink(const std::string& Text,
      const std::string& Url) const;

  std::string MakeHeadingId(const std::string& Text) const;

  std::string ConvertSubMarkdown(const std::string& SurrealMarkdown) const;

  std::map<std::string, std::function<void(const SurrealBlock& Block,
    const ParametersValues& Arguments, bool IsEntered)>> SurrealBlockHandlers;

  constexpr static char HTML_SERIALIZER_LOGGER_NAME[] = "html-serializer";

  constexpr static char ADMONITION_TITLE_ADMONITION[] = "Admonition";
  constexpr static char ADMONITION_TITLE_TOPIC[] = "Topic";
  constexpr static char ADMONITION_TITLE_CAUTION[] = "Caution";
  constexpr static char ADMONITION_TITLE_WARNING[] = "Warning";
  constexpr static char ADMONITION_TITLE_DANGER[] = "Danger";
  constexpr static char ADMONITION_TITLE_ATTENTION[] = "Attention";
  constexpr static char ADMONITION_TITLE_ERROR[] = "Error";
  constexpr static char ADMONITION_TITLE_HINT[] = "Hint";
  constexpr static char ADMONITION_TITLE_TIP[] = "Tip";
  constexpr static char ADMONITION_TITLE_IMPORTANT[] = "Important";
  constexpr static char ADMONITION_TITLE_NOTE[] = "Note";
  constexpr static char ADMONITION_TITLE_SEE_ALSO[] = "See also";
  constexpr static char ADMONITION_TITLE_TODO[] = "TODO";

  constexpr static char HTML_META_MARK[] = "!~+00z1p";

  constexpr static char NEWLINE[] = "\n";
  constexpr static char DOUBLE_NEWLINE[] = "\n\n";
  constexpr static char URL_SOFTBREAK_AT[] = "/";
  constexpr static char DIRECTORY_SEPARATOR = '/';
  constexpr static char URL_SOFTBROKE[] = "/<wbr/>";
  constexpr static char SPACE[] = " ";

  constexpr static int HEADING_ID_HTML_START = -1;
  constexpr static int FINALIZE_HEADINGS_IN_CURRENT_SCOPE = -1;

  constexpr static int ATTRIBUTE_STRING_RESERVE = 64;


  std::map<std::string, std::string> AdmonitionTitles;
  std::map<std::string, std::string> LinksPrefixes;
  std::map<std::string, std::string> ImagePrefixes;

  std::stack<RenderStackEntry> RenderStack;
  int TabSetId = 0;
  std::stack<int> TabSets;
  int CurrentTabSetPage = 0;
  int PageId = 0;
  int ImageNestingLevel = 0;
  std::stack<int> Headings;
  bool IsInitialized = false;
  bool IsInLink = false;
  bool IsInCode = false;

  std::unordered_map<std::string, HtmlReplacement> HtmlMetaMarkerReplacements;

  MD_PARSER Parser;

  bool mIsInline = false;
  bool mIsLocalBuild = false;

  std::unique_ptr<IsLinkInternalDecider> IsUrlInternalDecider;

  std::shared_ptr<spdlog::logger> l;
};
}  // namespace udocs_processor
