/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <md4c.h>
#include <vector>
#include <stack>
#include <map>
#include <string>
#include "SurrealMarkdownParser.h"

namespace udocs_processor {
// defines standard blocks
class SurrealMarkdown : public SurrealMarkdownParser {
 public:
  virtual void Initialize();

  void Parse(const std::string &SurrealMarkdown) override;

  bool DoEnterBlock(const SurrealBlock &Block,
      ParametersValues Arguments) override;

  void OnBlockEnter(const SurrealBlock &Block,
      ParametersValues Arguments) override;

  void OnBlockExit(const SurrealBlock &Block,
      ParametersValues Arguments) override;

  void OnMarkdown(const std::string &Markdown) override;

  std::vector<std::string> ExtractContents() const;

  virtual bool DoAcceptMarkdown() const;

  constexpr static char BLOCK_TABS[] = "tabs";
  constexpr static char BLOCK_PAGE[] = "page";
  constexpr static char PAGE_TITLE[] = "title";

  constexpr static char BLOCK_SPOILER[] = "spoiler";
  constexpr static char SPOILER_OPENED[] = "opened";
  constexpr static char SPOILER_TITLE[] = "title";

  constexpr static char BLOCK_FIGURE[] = "figure";
  constexpr static char FIGURE_URL[] = "url";
  constexpr static char FIGURE_ALIGN[] = "align";
  constexpr static char FIGURE_ALIGN_DEFAULT_VALUE[] = "default";
  // todo add checks
  constexpr static char FIGURE_ALIGN_VALUE_DEFAULT[] = "default";
  constexpr static char FIGURE_ALIGN_VALUE_LEFT[] = "left";
  constexpr static char FIGURE_ALIGN_VALUE_RIGHT[] = "right";

  constexpr static char BLOCK_CONTENTS[] = "contents";
  constexpr static char CONTENTS_DO_EXPAND[] = "expand";

  constexpr static char BLOCK_ADMONITION_CAUTION[] = "caution";
  constexpr static char BLOCK_ADMONITION_ADMONITION[] = "admonition";
  constexpr static char BLOCK_ADMONITION_TOPIC[] = "topic";
  constexpr static char BLOCK_ADMONITION_WARNING[] = "warning";
  constexpr static char BLOCK_ADMONITION_DANGER[] = "danger";
  constexpr static char BLOCK_ADMONITION_ATTENTION[] = "attention";
  constexpr static char BLOCK_ADMONITION_ERROR[] = "error";
  constexpr static char BLOCK_ADMONITION_HINT[] = "hint";
  constexpr static char BLOCK_ADMONITION_TIP[] = "tip";
  constexpr static char BLOCK_ADMONITION_IMPORTANT[] = "important";
  constexpr static char BLOCK_ADMONITION_NOTE[] = "note";
  constexpr static char BLOCK_ADMONITION_SEE_ALSO[] = "seealso";
  constexpr static char BLOCK_ADMONITION_TODO[] = "todo";

  constexpr static char ADMONITION_TITLE[] = "title";

  constexpr static unsigned RecommendedMd4cParserFlags =
      MD_FLAG_COLLAPSEWHITESPACE |
      MD_FLAG_PERMISSIVEATXHEADERS |
      MD_FLAG_PERMISSIVEURLAUTOLINKS |
      MD_FLAG_PERMISSIVEEMAILAUTOLINKS |
      MD_FLAG_NOINDENTEDCODEBLOCKS |
      MD_FLAG_TABLES |
      MD_FLAG_STRIKETHROUGH |
      MD_FLAG_PERMISSIVEWWWAUTOLINKS |
      MD_FLAG_LATEXMATHSPANS |
      MD_FLAG_UNDERLINE |
      MD_FLAG_NOHTML;

 private:
  constexpr static char CONTENTS_SPLIT_BY = '\n';

  enum class BlockType {
    TABS,
    CONTENTS,
    OTHER
  };

  std::map<std::string, BlockType> NameToBlock;
  std::stack<BlockType> BlocksStack;

  std::string Contents;
};
}  // namespace udocs_processor
