/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#define NOLINT()

#include <string>
#include <unordered_map>
#include <map>
#include <regex> NOLINT()
#include <stack>
#include <memory>
#include <any>

namespace udocs_processor {
class SurrealBlockParameter {
 public:
  enum class Type {
    BOOL,
    STRING,
    INTEGER
  };

  SurrealBlockParameter(std::string Name, Type ParameterType,
      bool IsOptional = false, std::any DefaultValue = {});

  const std::string &GetName() const;

  bool IsOptional() const;

  Type GetParameterType() const;

  const std::any &GetDefaultValue() const;

 private:
  std::string Name;
  bool mIsOptional = false;
  Type ParameterType = Type::STRING;
  std::any DefaultValue;
};

class SurrealBlock {
 public:
  const std::string &GetName() const;

  void SetName(std::string Name);

  void AddParameter(std::string Name, SurrealBlockParameter::Type Type,
      bool IsOptional = false, std::any DefaultValue = {});

  const std::map<std::string, SurrealBlockParameter> &
      GetParameters() const;

 private:
  std::string Name;
  std::map<std::string, SurrealBlockParameter> Parameters;
};

class SurrealMarkdownParser {
 public:
  using ParametersValues = std::map<std::string, std::any>;

  SurrealMarkdownParser();

  virtual bool DoEnterBlock(const SurrealBlock& Block,
      ParametersValues Arguments) = 0;

  virtual void OnBlockEnter(const SurrealBlock& Block,
      ParametersValues Arguments) = 0;

  virtual void OnBlockExit(const SurrealBlock& Block,
      ParametersValues Arguments) = 0;

  virtual void OnMarkdown(const std::string& Markdown) = 0;

  virtual void OnEOF() = 0;

  virtual void AddSupportedBlock(SurrealBlock BlockPrototype);

  virtual void Parse(const std::string& SurrealMarkdown);

 private:
  class StackEntry {
   public:
    StackEntry(const SurrealBlock &Block, ParametersValues Arguments);

    const SurrealBlock &GetBlock() const;

    const ParametersValues &GetArguments() const;

   private:
    const SurrealBlock& Block;
    ParametersValues Arguments;
  };

  size_t FindFirstNonSpaceSymbolOnThisLine(
      const std::string& String, size_t Index) const;

  void CopyMarkdownCode(const std::string& Content, size_t From, size_t Until);

  bool ParseSurrealBlock(const std::string& Content, size_t FirstNonSpace,
      size_t LineStart, size_t LineLength, size_t InsideBlocks,
      size_t& Length, bool& IsBlockStart,
      std::shared_ptr<StackEntry>& Entry) const;

  bool ParseParameters(const SurrealBlock& Block, const std::string& Content,
      size_t StartFrom, size_t& Length, ParametersValues& Arguments) const;

  bool ValidateArguments(
      const std::map<std::string, std::string>& ArgumentsAsStrings,
      ParametersValues& Arguments, const SurrealBlock& Block) const;

  bool CheckIfIsCodeBlock(const std::string& SurrealMarkdown,
      size_t FirstNonSpace, size_t LineLength, bool& IsInsideCode,
      char& CodeSymbol) const;

  void TryParseSurrealBlock(const std::string& SurrealMarkdown,
      size_t FirstNonSpace, size_t LineStart, size_t LineLength,
      size_t LastLineParsed, size_t& CopyFrom,
      std::stack<StackEntry>& Blocks, size_t& Pos);

  bool IsNameSymbol(char Symbol) const;

  bool IsSpace(char Symbol) const;

  std::string& TrimAllWhitespaces(std::string& Input) const;

  constexpr static char MARKDOWN_CODE_SYMBOL_1 = '`';
  constexpr static char MARKDOWN_CODE_SYMBOL_2 = '~';
  constexpr static int MARKDOWN_CODE_BLOCK_LEN = 3;
  constexpr static char BLOCK_START[] = "``";
  constexpr static char TRUE_STRING[] = "true";
  constexpr static char FALSE_STRING[] = "false";
  constexpr static char INTEGER_PATTERN[] = "-?[1-9][0-9]*";
  constexpr static char PARAMETER_VALUE_SEPARATOR = ':';
  constexpr static char NEWLINE = '\n';
  constexpr static char TAB = '\t';
  constexpr static char SPACE = ' ';
  constexpr static char WHITESPACES[] = " \n\t";

  constexpr static char NAME_SYMBOLS_RANGE_1_START = 'a';
  constexpr static char NAME_SYMBOLS_RANGE_1_END = 'z';
  constexpr static char NAME_SYMBOLS_RANGE_2_START = 'A';
  constexpr static char NAME_SYMBOLS_RANGE_2_END = 'Z';
  constexpr static char NAME_SYMBOLS_RANGE_3_START = '0';
  constexpr static char NAME_SYMBOLS_RANGE_3_END = '9';

  std::regex IntegerPattern;

  std::map<std::string, SurrealBlock> SupportedBlocks;
};
}  // namespace udocs_processor
