/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#define NOLINT()

#include <string>
#include <regex> NOLINT()

namespace udocs_processor {
class IsLinkInternalDecider {
 public:
  IsLinkInternalDecider();

  bool IsInternal(const std::string& Url) const;

  void SetDomain(std::string Domain);

 private:
  constexpr static char PROTOCOL_MARKER[] = "://";
  constexpr static char DOMAIN_PATTERN[] =
      R"(^(?:https?:\/\/)?(?:[^@\/\n]+@)?(?:www\.)?([^:\/?\n]+))";

  // like readthedocs.com
  std::string Domain;
  std::regex DomainPattern;
};
}  // namespace udocs_processor
