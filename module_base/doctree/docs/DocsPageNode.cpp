/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/docs/DocsPageNode.h"

#include <utility>
#include "udocs-processor/doctree/DocNodeVisitor.h"

udocs_processor::PageType udocs_processor::DocsPageNode::GetType() const {
  return Type;
}

void udocs_processor::DocsPageNode::SetType(PageType Type) {
  this->Type = Type;
}

udocs_processor::PageFormat udocs_processor::DocsPageNode::GetFormat() const {
  return Format;
}

void udocs_processor::DocsPageNode::SetFormat(PageFormat Format) {
  this->Format = Format;
}

const std::string &udocs_processor::DocsPageNode::GetTitle() const {
  return Title;
}

void udocs_processor::DocsPageNode::SetTitle(std::string Title) {
  this->Title = std::move(Title);
}

bool udocs_processor::DocsPageNode::IsCanonical() const {
  return mIsCanonical;
}

void udocs_processor::DocsPageNode::SetIsCanonical(bool IsCanonical) {
  mIsCanonical = IsCanonical;
}

bool udocs_processor::DocsPageNode::IsEmpty() const {
  return mIsEmpty;
}

void udocs_processor::DocsPageNode::SetIsEmpty(bool IsEmpty) {
  mIsEmpty = IsEmpty;
}

bool udocs_processor::DocsPageNode::DoExpandContents() const {
  return mDoExpandContents;
}

void udocs_processor::DocsPageNode::SetDoExpandContents(bool DoExpandContents) {
  mDoExpandContents = DoExpandContents;
}

bool udocs_processor::DocsPageNode::IsPinned() const {
  return mIsPinned;
}

void udocs_processor::DocsPageNode::SetIsPinned(bool IsPinned) {
  mIsPinned = IsPinned;
}

void udocs_processor::DocsPageNode::SetCanonical(
    std::weak_ptr<DocsPageNode> Canonical) {
  this->Canonical = Canonical;
}

std::weak_ptr<udocs_processor::DocsPageNode>
    udocs_processor::DocsPageNode::GetCanonical() const {
  return Canonical;
}

udocs_processor::DocsPageNode::DocsPageNode(
    std::weak_ptr<DocNode> Parent, uint64_t Id)
    : DocsElementNode(Parent, Id) {}

int udocs_processor::DocsPageNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}
