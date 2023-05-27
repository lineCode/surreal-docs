/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#define NOLINT()

#include "udocs-processor/serializers/UFunctionSerializer.h"

void udocs_processor::UFunctionSerializer::SetResourcesPath(
    std::string ResourcesPath) {
  this->ResourcesPath = std::move(ResourcesPath);
}

const std::string &
    udocs_processor::UFunctionSerializer::GetResourcesPath() const {
  return ResourcesPath;
}
