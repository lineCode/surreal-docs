/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#define NOLINT()

#include "udocs-processor/treeloaders/DocCommentProcessor.h"
#include <regex> NOLINT()
#include <string>

udocs_processor::DocCommentProcessor::DocCommentProcessor(
    const std::string &Content) : Content(Content) {}

void udocs_processor::DocCommentProcessor::OnCommandEncountered(
    const std::string &Command, size_t NextSymbolPos) {}

void udocs_processor::DocCommentProcessor::OnCleanText(std::string Text) {}

void udocs_processor::DocCommentProcessor::AddSupportedCommand(
    std::string Name, CommandHandlerType Handler) {
  Handlers.insert(
      std::make_pair(std::move(Name), std::move(Handler)));
}

void udocs_processor::DocCommentProcessor::AddSupportedCommandPrefix(
    std::string Prefix) {
  Prefixes.emplace_back(std::move(Prefix));
}

void udocs_processor::DocCommentProcessor::SetPosition(size_t Pos) {
  CurrentPos = Pos;
}

std::string udocs_processor::DocCommentProcessor::GoUntilStandardEnd() {
  if (CurrentPos == std::string::npos) {
    return "";
  }

  size_t PrefixLength = 0;
  size_t NextCommand = FindNextCommand(CurrentPos, PrefixLength);
  size_t DoubleNewLine = Content.find(DOUBLE_NEWLINE, CurrentPos);

  size_t GoTo = std::min(NextCommand, DoubleNewLine);
  size_t AdditionalSkip = GoTo == DoubleNewLine
      ? strlen(DOUBLE_NEWLINE) : 0;

  return GoUntil(GoTo, AdditionalSkip);
}

std::string udocs_processor::DocCommentProcessor::GoUntilEndOfLine() {
  if (CurrentPos == std::string::npos) {
    return "";
  }

  size_t NewLine = Content.find(NEWLINE, CurrentPos);

  return GoUntil(NewLine, strlen(NEWLINE));
}

std::string udocs_processor::DocCommentProcessor::GoUntil(
    size_t LastSymbolOfContent, size_t AdditionalSkip) {
  if (LastSymbolOfContent != std::string::npos) {
    std::string ToReturn =
        Content.substr(CurrentPos, LastSymbolOfContent - CurrentPos);

    CurrentPos = LastSymbolOfContent + AdditionalSkip >= Content.size()
        ? std::string::npos : LastSymbolOfContent + AdditionalSkip;

    return std::move(ToReturn);
  } else {
    std::string ToReturn = Content.substr(CurrentPos);

    CurrentPos = std::string::npos;

    return std::move(ToReturn);
  }
}

void udocs_processor::DocCommentProcessor::Process() {
  if (Content.empty()) {
    return;
  }

  CurrentPos = 0;
  size_t CopyFrom = 0;
  size_t CommandPrefixLength = 0;

  while ((CurrentPos = FindNextCommand(
      CurrentPos, CommandPrefixLength)) != std::string::npos) {
    std::regex CommandName(COMMAND_NAME_PATTERN, std::regex::icase);
    std::smatch Match;

    size_t CommandWasFoundAt = CurrentPos;
    bool ProcessedCommand = false;
    if (CurrentPos + CommandPrefixLength < Content.size()) {
      auto NameBegin = Content.begin() + CurrentPos + CommandPrefixLength;
      bool Matched = std::regex_search(NameBegin,
          Content.end(), Match, CommandName);
      // if regex matches from start
      if (Matched && Match[1].first == NameBegin) {
        const std::string &Name = Match[1].str();
        auto It = Handlers.find(Name);
        if (It != Handlers.end()) {
          // copy text
          // start of the command (prefix)
          size_t CopyUntil = CommandWasFoundAt;

          OnCleanText(Content.substr(CopyFrom, CopyUntil - CopyFrom));

          // next symbol after command name and all the spaces
          size_t ArgumentsStart =
              CurrentPos + Match[0].length() + CommandPrefixLength;
          if (ArgumentsStart > Content.size()) {
            ArgumentsStart = std::string::npos;
          }
          CurrentPos = ArgumentsStart;
          OnCommandEncountered(Name, ArgumentsStart);
          It->second(ArgumentsStart);

          CopyFrom = CurrentPos < Content.size()
              ? CurrentPos : std::string::npos;

          ProcessedCommand = true;
        }
      }
    }

    if (!ProcessedCommand) {
      if (CurrentPos + CommandPrefixLength < Content.size()) {
        CurrentPos += CommandPrefixLength;
      } else {
        CurrentPos = std::string::npos;
      }
    }

    // if CurrentPos was changed inside the handler
    if (CurrentPos == std::string::npos) {
      break;
    }
  }

  if (CopyFrom != std::string::npos) {
    OnCleanText(Content.substr(CopyFrom));
  }
}

size_t udocs_processor::DocCommentProcessor::FindNextCommand(
    size_t StartFrom, size_t &CommandPrefixLength) const {
  CommandPrefixLength = 0;
  size_t Pos = std::string::npos;

  for (const auto& Prefix : Prefixes) {
    size_t PrefixPos = Content.find(Prefix, StartFrom);

    if (PrefixPos != std::string::npos && PrefixPos < Pos) {
      Pos = PrefixPos;
      CommandPrefixLength = Prefix.size();
    }
  }

  return Pos < Content.size() ? Pos : std::string::npos;
}

const std::string &udocs_processor::DocCommentProcessor::GetContent() const {
  return Content;
}

#define MEMBER_FUNCTION(Name) [this](size_t p) {Name(p);}

udocs_processor::RefCommentProcessor::RefCommentProcessor(
    const std::string &Content)
    : DocCommentProcessor(Content)  {
  l = spdlog::get(UDOCS_PROCESSOR_LOGGER_NAME);

  AddSupportedCommand(COMMAND_REF, MEMBER_FUNCTION(OnRef));

  AddSupportedCommandPrefix(COMMAND_PREFIX_JAVADOC_STYLE);
  AddSupportedCommandPrefix(COMMAND_PREFIX_TRADITIONAL);
}

std::string udocs_processor::RefCommentProcessor::MakeRef(
    std::string Referencee, std::string Text) const {
  return REF_TAG_OPENING_1 + std::move(Referencee) + REF_TAG_OPENING_2 +
    std::move(Text) + REF_TAG_CLOSING;
}

std::string&& udocs_processor::RefCommentProcessor::ClaimCleanText() {
  return std::move(CleanText);
}

void udocs_processor::RefCommentProcessor::OnCleanText(std::string Text) {
  CleanText += Text;
}

void udocs_processor::RefCommentProcessor::OnRef(size_t NextSymbolPos) {
  // two formats:
  // 1) "referencee with spaces" ["text"]
  // 2) referencee ["text"]

  if (NextSymbolPos != std::string::npos) {
    const std::string& Content = GetContent();
    auto NextSymbol = Content.begin() + NextSymbolPos;
    std::string Referencee;
    std::string Text;
    bool DoApply = false;

    std::regex RefPattern(REF_PATTERN, std::regex::icase);
    std::smatch Match;
    if (std::regex_search(NextSymbol, Content.end(), Match, RefPattern)) {
      // either 1 or 2 must be matched
      Referencee = Match[1].matched ? Match[1].str() : Match[2].str();
      Text = Match[3].matched ? Match[3].str() : Referencee;

      DoApply = true;

      size_t NextPos = NextSymbolPos + Match[0].length();
      SetPosition(NextPos >= Content.size() ? std::string::npos : NextPos);
    }

    if (DoApply) {
      CleanText += MakeRef(
          std::move(Referencee), std::move(Text));
    }
  }
}

void udocs_processor::RefCommentProcessor::Process() {
  DocCommentProcessor::Process();
}
