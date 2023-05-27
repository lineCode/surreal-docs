/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include "BlueprintUNode.h"

namespace udocs_processor {
class NativeClassCounterpart : public BlueprintUNode {
 public:
  NativeClassCounterpart(std::weak_ptr<DocNode> Parent, uint64_t Id);

  ~NativeClassCounterpart() override = default;
};
}  // namespace udocs_processor
