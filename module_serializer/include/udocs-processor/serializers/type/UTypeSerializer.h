/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <string>
#include <unordered_map>
#include <memory>

namespace udocs_processor {
class BlueprintTypeNode;

class UTypeSerializer {
 public:
  virtual void Serialize(BlueprintTypeNode &Type) = 0;
};
}  // namespace udocs_processor

