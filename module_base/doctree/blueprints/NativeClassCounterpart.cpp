/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/blueprints/NativeClassCounterpart.h"

udocs_processor::NativeClassCounterpart::NativeClassCounterpart(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : BlueprintUNode(Parent, Id) {}
