/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include <utility>

#include "udocs-processor/serializers/type/UTypeToImagesSerializer.h"

const std::unordered_map<std::string, std::string> &
    udocs_processor::UTypeToImagesSerializer::GetTypeImages() const {
  return TypeImages;
}

void udocs_processor::UTypeToImagesSerializer::
    SetLambdaDeciderWhetherToSerialize(DoSerializeDeciderType Decider) {
  this->Decider = std::move(Decider);
}

std::unordered_map<std::string, std::string> &
    udocs_processor::UTypeToImagesSerializer::GetTypeImagesNonConst() {
  return TypeImages;
}

const udocs_processor::UTypeToImagesSerializer::DoSerializeDeciderType&
    udocs_processor::UTypeToImagesSerializer::GetDecider() const {
  return Decider;
}

void udocs_processor::UTypeToImagesSerializer::SetResourcesPath(
    std::string ResourcesPath) {
  this->ResourcesPath = std::move(ResourcesPath);
}

const std::string &
    udocs_processor::UTypeToImagesSerializer::GetResourcesPath() const {
  return ResourcesPath;
}
