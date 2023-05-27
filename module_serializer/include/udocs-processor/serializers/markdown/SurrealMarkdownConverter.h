/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <string>

namespace udocs_processor {
class SurrealMarkdownConverter {
 public:
  virtual std::string Convert(const std::string &SurrealMarkdown) = 0;
};
}  // namespace udocs_processor
