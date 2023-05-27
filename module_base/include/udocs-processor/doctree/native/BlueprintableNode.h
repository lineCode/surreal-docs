/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>

namespace udocs_processor {
class BlueprintableNode {
 public:
  virtual bool IsUeEntity() const;

  virtual void SetIsUeEntity(bool IsUeEntity);

 private:
  bool mIsUeEntity = false;
};
}  // namespace udocs_processor
