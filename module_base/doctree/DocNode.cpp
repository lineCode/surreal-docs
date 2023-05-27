/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/doctree/DocNode.h"
#include "udocs-processor/doctree/DocNodeVisitor.h"

udocs_processor::DocNode::DocNode(std::weak_ptr<DocNode> Parent, uint64_t Id)
  : Parent(Parent), Id(Id) {}

void udocs_processor::DocNode::AddNode(std::shared_ptr<DocNode> Node) {
  ChildrenView.emplace_back(Node.get());
  Children.emplace_back(Node);
}

const std::vector<const udocs_processor::DocNode *>&
    udocs_processor::DocNode::ViewNodes() const {
  return ChildrenView;
}

const std::string &udocs_processor::DocNode::GetName() const {
  return Name;
}

void udocs_processor::DocNode::SetName(std::string Name) {
  this->Name = std::move(Name);
}

const std::string &udocs_processor::DocNode::GetShortDescription() const {
  return ShortDescription;
}

void udocs_processor::DocNode::SetShortDescription(
    std::string ShortDescription) {
  this->ShortDescription = std::move(ShortDescription);
}

const std::string &udocs_processor::DocNode::GetFullDescription() const {
  return FullDescription;
}

void udocs_processor::DocNode::SetFullDescription(
    std::string FullDescription) {
  this->FullDescription = std::move(FullDescription);
}

uint64_t udocs_processor::DocNode::GetId() const {
  return Id;
}

const std::string &udocs_processor::DocNode::GetDeprecationMessage() const {
  return DeprecationMessage;
}

void udocs_processor::DocNode::SetDeprecationMessage(
    std::string DeprecationMessage) {
  this->DeprecationMessage = std::move(DeprecationMessage);
}

const std::string &udocs_processor::DocNode::GetDeprecationVersion() const {
  return DeprecationVersion;
}

void udocs_processor::DocNode::SetDeprecationVersion(
    std::string DeprecationVersion) {
  this->DeprecationVersion = std::move(DeprecationVersion);
}

bool udocs_processor::DocNode::IsDeprecated() const {
  return mIsDeprecated;
}

void udocs_processor::DocNode::SetIsDeprecated(bool IsDeprecated) {
  mIsDeprecated = IsDeprecated;
}

const std::string &udocs_processor::DocNode::GetModule() const {
  return Module;
}

void udocs_processor::DocNode::SetModule(std::string Module) {
  this->Module = std::move(Module);
}

const std::string &udocs_processor::DocNode::GetPath() const {
  return Path;
}

void udocs_processor::DocNode::SetPath(std::string Path) {
  this->Path = std::move(Path);
}

int udocs_processor::DocNode::Accept(DocNodeVisitor &Visitor) {
  return Visitor.Visit(*this);
}

std::weak_ptr<udocs_processor::DocNode>
    udocs_processor::DocNode::GetParent() const {
  return Parent;
}

void udocs_processor::DocNode::IterateChildren(DocNodeVisitor& Visitor,
    bool DoRecursively, size_t LevelsLimit) {
  // todo replace on a non-recursive approach
  for (auto& Child : Children) {
    int Result = Child->Accept(Visitor);

    if (Result == 0 && DoRecursively && LevelsLimit > 1) {
      size_t NewLevelsLimit = LevelsLimit != ~0 ? LevelsLimit - 1 : ~0;
      Child->IterateChildren(Visitor, DoRecursively, NewLevelsLimit);
    }
  }
}

void udocs_processor::DocNode::MoveTo(std::weak_ptr<DocNode> NewParent) {
  Parent = NewParent;
}

const std::string &udocs_processor::DocNode::GetSince() const {
  return Since;
}

void udocs_processor::DocNode::SetSince(std::string Since) {
  this->Since = std::move(Since);
}

void udocs_processor::DocNode::CopyInfoTo(DocNode& Node) const {
  Node.Name = Name;
  Node.Since = Since;
  Node.ShortDescription = ShortDescription;
  Node.FullDescription = FullDescription;
  Node.DeprecationMessage = DeprecationMessage;
  Node.DeprecationVersion = DeprecationVersion;
  Node.Module = Module;
  Node.Path = Path;
  Node.mIsDeprecated = mIsDeprecated;
}

udocs_processor::DocNode&
    udocs_processor::DocNode::operator=(DocNode&& Node) noexcept {
  Children = std::move(Node.Children);
  ChildrenView = std::move(Node.ChildrenView);
  Parent = std::move(Node.Parent);
  Name = std::move(Node.Name);
  Since = std::move(Node.Since);
  ShortDescription = std::move(Node.ShortDescription);
  FullDescription = std::move(Node.FullDescription);
  DeprecationMessage = std::move(Node.DeprecationMessage);
  DeprecationVersion = std::move(Node.DeprecationVersion);
  Module = std::move(Node.Module);
  Path = std::move(Node.Path);
  mIsDeprecated = Node.mIsDeprecated;
  Id = Node.Id;

  for (auto& Child : Children) {
    Child->MoveTo(shared_from_this());
  }

  return *this;
}
