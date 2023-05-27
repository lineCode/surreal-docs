/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include "DocNode.h"

namespace udocs_processor {
class HasCounterpart {
 public:
  virtual std::weak_ptr<DocNode> GetCounterpart() const = 0;
};
}  // namespace udocs_processor
