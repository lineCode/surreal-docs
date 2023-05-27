/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/blueprints/BlueprintParameterNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"

bool udocs_processor::BlueprintParameterNode::IsReturnParameter() const {
  return mIsReturnParameter;
}

void udocs_processor::BlueprintParameterNode::SetIsReturnParameter(
    bool IsReturnParameter) {
  mIsReturnParameter = IsReturnParameter;
}

int udocs_processor::BlueprintParameterNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

udocs_processor::BlueprintParameterNode::BlueprintParameterNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : BlueprintUNode(Parent, Id) {}
