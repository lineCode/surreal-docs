/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/serializers/type/UTypeToStringSerializer.h"

const std::string &
    udocs_processor::UTypeToStringSerializer::GetTypeMarkdown() const {
  return TypeMarkdown;
}

void udocs_processor::UTypeToStringSerializer::SetTypeMarkdown(
    std::string TypeMarkdown) {
  this->TypeMarkdown = std::move(TypeMarkdown);
}
