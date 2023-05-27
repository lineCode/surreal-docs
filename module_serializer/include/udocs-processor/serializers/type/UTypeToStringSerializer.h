/* Copyright © 2022, Medelfor, Limited. All rights reserved. */
#pragma once

#include "UTypeSerializer.h"
#include <string>

namespace udocs_processor {
class UTypeToStringSerializer : public UTypeSerializer {
 public:
  const std::string &GetTypeMarkdown() const;

 protected:
  void SetTypeMarkdown(std::string TypeMarkdown);

 private:
  std::string TypeMarkdown;
};
}  // namespace udocs_processor
