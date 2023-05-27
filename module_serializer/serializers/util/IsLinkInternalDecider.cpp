/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/serializers/util/IsLinkInternalDecider.h"

udocs_processor::IsLinkInternalDecider::IsLinkInternalDecider() {
  DomainPattern = std::regex(DOMAIN_PATTERN, std::regex::icase);
}

bool udocs_processor::IsLinkInternalDecider::IsInternal(
    const std::string &Url) const {
  // either it doesn't have protocol, or placed on the domain
  if (Url.find(PROTOCOL_MARKER) == std::string::npos) {
    return true;
  } else {
    std::smatch Match;
    if (std::regex_search(Url, Match, DomainPattern)) {
      return Match[1].str() == Domain;
    }
  }

  return false;
}
void udocs_processor::IsLinkInternalDecider::SetDomain(
    std::string Domain) {
  this->Domain = std::move(Domain);
}
