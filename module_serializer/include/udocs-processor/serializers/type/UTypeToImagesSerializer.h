/* Copyright © 2022, Medelfor, Limited. All rights reserved. */
#pragma once

#include "UTypeSerializer.h"
#include <functional>
#include <unordered_map>
#include <string>

namespace udocs_processor {
class UTypeToImagesSerializer : public UTypeSerializer {
 public:
  using DoSerializeDeciderType = std::function<bool(const std::string&)>;

  const std::unordered_map<std::string, std::string> &GetTypeImages() const;

  void SetLambdaDeciderWhetherToSerialize(
      DoSerializeDeciderType Decider);

  virtual std::string GetFileExtension() const = 0;

  void SetResourcesPath(std::string ResourcesPath);

  const std::string &GetResourcesPath() const;

 protected:
  std::unordered_map<std::string, std::string>& GetTypeImagesNonConst();

  const DoSerializeDeciderType& GetDecider() const;

 private:
  DoSerializeDeciderType Decider;
  std::unordered_map<std::string, std::string> TypeImages;
  std::string ResourcesPath;
};
}  // namespace udocs_processor
