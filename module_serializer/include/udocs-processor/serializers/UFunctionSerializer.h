/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <string>
#include <memory>

namespace udocs_processor {
class BlueprintFunctionNode;

class UFunctionSerializer {
 public:
  virtual std::string Serialize(
      BlueprintFunctionNode &Function) = 0;

  virtual std::string GetFileExtension() const = 0;

  void SetResourcesPath(std::string ResourcesPath);

  const std::string &GetResourcesPath() const;

 private:
  std::string ResourcesPath;
};
}  // namespace udocs_processor

