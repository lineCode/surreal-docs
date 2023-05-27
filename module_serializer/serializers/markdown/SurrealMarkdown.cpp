/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/serializers/markdown/SurrealMarkdown.h"

#include <stack>
#include <stringtoolbox.hpp>
#include "udocs-processor/StringHelper.h"

void udocs_processor::SurrealMarkdown::Initialize() {
  SurrealBlock Tabs;
  Tabs.SetName(BLOCK_TABS);
  AddSupportedBlock(std::move(Tabs));

  SurrealBlock Page;
  Page.SetName(BLOCK_PAGE);
  Page.AddParameter(PAGE_TITLE,
      SurrealBlockParameter::Type::STRING,
      true, std::string());
  AddSupportedBlock(std::move(Page));

  SurrealBlock Spoiler;
  Spoiler.SetName(BLOCK_SPOILER);
  Spoiler.AddParameter(SPOILER_OPENED,
      SurrealBlockParameter::Type::BOOL, true,
      false);
  Spoiler.AddParameter(SPOILER_TITLE,
      SurrealBlockParameter::Type::STRING, true,
      std::string());
  AddSupportedBlock(std::move(Spoiler));

  std::vector<std::string> Admonitions = {
      BLOCK_ADMONITION_TOPIC,
      BLOCK_ADMONITION_ADMONITION,
      BLOCK_ADMONITION_CAUTION,
      BLOCK_ADMONITION_WARNING,
      BLOCK_ADMONITION_DANGER,
      BLOCK_ADMONITION_ATTENTION,
      BLOCK_ADMONITION_ERROR,
      BLOCK_ADMONITION_HINT,
      BLOCK_ADMONITION_TIP,
      BLOCK_ADMONITION_IMPORTANT,
      BLOCK_ADMONITION_NOTE,
      BLOCK_ADMONITION_SEE_ALSO,
      BLOCK_ADMONITION_TODO
  };

  for (const std::string& Admonition : Admonitions) {
    SurrealBlock AdmonitionBlock;
    AdmonitionBlock.SetName(Admonition);
    AdmonitionBlock.AddParameter(ADMONITION_TITLE,
        SurrealBlockParameter::Type::STRING, true,
        std::string());
    AddSupportedBlock(std::move(AdmonitionBlock));
  }

  SurrealBlock FigureBlock;
  FigureBlock.SetName(BLOCK_FIGURE);
  FigureBlock.AddParameter(FIGURE_URL,
      SurrealBlockParameter::Type::STRING, true,
      std::string());
  FigureBlock.AddParameter(FIGURE_ALIGN,
      SurrealBlockParameter::Type::STRING, true,
      std::string(FIGURE_ALIGN_DEFAULT_VALUE));
  AddSupportedBlock(std::move(FigureBlock));

  SurrealBlock ContentsBlock;
  ContentsBlock.SetName(BLOCK_CONTENTS);
  ContentsBlock.AddParameter(CONTENTS_DO_EXPAND,
      SurrealBlockParameter::Type::BOOL, true,
      false);
  AddSupportedBlock(std::move(ContentsBlock));

  NameToBlock.insert(std::make_pair(BLOCK_TABS, BlockType::TABS));
  NameToBlock.insert(std::make_pair(BLOCK_CONTENTS, BlockType::CONTENTS));
}

// the idea is to discard any pages that are not direct children of a tabs block
void udocs_processor::SurrealMarkdown::OnBlockEnter(
    const SurrealBlock &Block,
    SurrealMarkdownParser::ParametersValues Arguments) {
  BlockType Id = BlockType::OTHER;

  auto It = NameToBlock.find(Block.GetName());
  if (It != NameToBlock.end()) {
    Id = It->second;
  }

  BlocksStack.push(Id);
}

void udocs_processor::SurrealMarkdown::OnBlockExit(const SurrealBlock &Block,
    SurrealMarkdownParser::ParametersValues Arguments) {
  BlocksStack.pop();
}

bool udocs_processor::SurrealMarkdown::DoEnterBlock(
    const SurrealBlock &Block, ParametersValues Arguments) {
  // no blocks allowed inside contents
  if (!BlocksStack.empty() && BlocksStack.top() == BlockType::CONTENTS) {
    return false;
  }

  if (Block.GetName() == BLOCK_PAGE) {
    if (BlocksStack.empty() || BlocksStack.top() != BlockType::TABS) {
      return false;
    }
  } else if (!BlocksStack.empty() && BlocksStack.top() == BlockType::TABS) {
    // refuse anything inside tabs except page
    return false;
  }

  return true;
}

bool udocs_processor::SurrealMarkdown::DoAcceptMarkdown() const {
  return BlocksStack.empty() || (BlocksStack.top() != BlockType::TABS &&
      BlocksStack.top() != BlockType::CONTENTS);
}

void udocs_processor::SurrealMarkdown::Parse(
    const std::string &SurrealMarkdown) {
  BlocksStack = {};
  Contents = {};

  SurrealMarkdownParser::Parse(SurrealMarkdown);
}

void udocs_processor::SurrealMarkdown::OnMarkdown(const std::string &Markdown) {
  if (!BlocksStack.empty() && BlocksStack.top() == BlockType::CONTENTS) {
    Contents += Markdown;
  }
}

std::vector<std::string>
    udocs_processor::SurrealMarkdown::ExtractContents() const {
  std::vector<std::string> Result;

  Result = stringtoolbox::split(Contents, CONTENTS_SPLIT_BY);
  for (auto It = Result.begin(); It != Result.end(); ) {
    if (It->empty() || StringHelper::TrimAllWhitespaces(*It).empty()) {
      It = Result.erase(It);
    } else {
      ++It;
    }
  }

  return Result;
}
