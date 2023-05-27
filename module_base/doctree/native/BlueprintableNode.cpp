/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/native/BlueprintableNode.h"

bool udocs_processor::BlueprintableNode::IsUeEntity() const {
  return mIsUeEntity;
}

void udocs_processor::BlueprintableNode::SetIsUeEntity(bool IsUeEntity) {
  mIsUeEntity = IsUeEntity;
}
