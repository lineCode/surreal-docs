/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <spdlog/spdlog.h>
#include <string>
#include <memory>
#include <unordered_map>
#include <map>
#include <vector>
#include <functional>
#include "udocs-processor/IdGenerator.h"
#include "udocs-processor/doctree/blueprints/BlueprintUNode.h"
#include "udocs-processor/report/ProcessingReport.h"

namespace udocs_processor {
class DocCommentProcessor {
 public:
  using CommandHandlerType = std::function<void(size_t)>;

  explicit DocCommentProcessor(const std::string& Content);

  virtual void OnCommandEncountered(
      const std::string& Command, size_t NextSymbolPos);

  virtual void OnCleanText(std::string Text);

  void AddSupportedCommand(std::string Name, CommandHandlerType Handler);

  void AddSupportedCommandPrefix(std::string Prefix);

  // go until next command or \n\n or EOF
  std::string GoUntilStandardEnd();

  // go until \n or EOF
  std::string GoUntilEndOfLine();

  void SetPosition(size_t Pos);

  virtual void Process();

  const std::string& GetContent() const;

 private:
  constexpr static char COMMAND_NAME_PATTERN[] = R"(^([-_a-z0-9]+)\s+)";
  constexpr static char DOUBLE_NEWLINE[] = "\n\n";
  constexpr static char NEWLINE[] = "\n";

  // returns pos of symbol after command prefix or npos if it's EOF
  size_t FindNextCommand(size_t StartFrom, size_t& CommandPrefixLength) const;

  std::string GoUntil(size_t LastSymbolOfContent, size_t AdditionalSkip);

  std::vector<std::string> Prefixes;
  std::map<std::string, CommandHandlerType> Handlers;
  const std::string& Content;
  size_t CurrentPos;
};

class RefCommentProcessor : public DocCommentProcessor {
 public:
  explicit RefCommentProcessor(const std::string& Content);

  void OnRef(size_t NextSymbolPos);

  std::string&& ClaimCleanText();

  void OnCleanText(std::string Text) override;

  void Process() override;

 protected:
  std::string MakeRef(std::string Referencee, std::string Text) const;

  constexpr static char COMMAND_PREFIX_JAVADOC_STYLE[] = "@";
  constexpr static char COMMAND_PREFIX_TRADITIONAL[] = "\\";

  mutable std::shared_ptr<spdlog::logger> l;

 private:
  std::string CleanText;

  constexpr static char UDOCS_PROCESSOR_LOGGER_NAME[] = "udocs-processor";

  constexpr static char COMMAND_REF[] = "ref";

  constexpr static char REF_TAG_OPENING_1[] = "<ref refc=\"";
  constexpr static char REF_TAG_OPENING_2[] = "\">";
  constexpr static char REF_TAG_CLOSING[] = "</ref>";

  constexpr static char REF_PATTERN[] =
      R"(^(?:"([^"]+)+"|([^\s]+))(?:\s+"([^"]+)+")?)";
};
}  // namespace udocs_processor
