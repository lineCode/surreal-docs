/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include <vector>
#include <string>

namespace udocs_processor {
class DocNodeVisitor;

class DocNode : public std::enable_shared_from_this<DocNode> {
 public:
  DocNode(std::weak_ptr<DocNode> Parent, uint64_t Id);

  void AddNode(std::shared_ptr<DocNode> Node);

  const std::vector<const DocNode*>& ViewNodes() const;

  const std::string &GetName() const;

  void SetName(std::string Name);

  const std::string &GetShortDescription() const;

  void SetShortDescription(std::string ShortDescription);

  const std::string &GetFullDescription() const;

  void SetFullDescription(std::string FullDescription);

  const std::string &GetDeprecationMessage() const;

  void SetDeprecationMessage(std::string DeprecationMessage);

  const std::string &GetDeprecationVersion() const;

  void SetDeprecationVersion(std::string DeprecationVersion);

  bool IsDeprecated() const;

  void SetIsDeprecated(bool IsDeprecated);

  const std::string &GetModule() const;

  void SetModule(std::string Module);

  const std::string &GetPath() const;

  void SetPath(std::string Path);

  const std::string &GetSince() const;

  void SetSince(std::string Since);

  uint64_t GetId() const;

  std::weak_ptr<DocNode> GetParent() const;

  // does not copy children, parent, id, only this node's data
  void CopyInfoTo(DocNode& Node) const;

  // todo: implement also copy ctor, move ctor, copy ass.
  // todo: do the same in all the subclasses
  DocNode& operator=(DocNode&& Node) noexcept;

  virtual void IterateChildren(DocNodeVisitor& Visitor, bool DoRecursively,
      size_t LevelsLimit = ~0);

  virtual void MoveTo(std::weak_ptr<DocNode> NewParent);

  virtual int Accept(DocNodeVisitor& Visitor);

  virtual ~DocNode() = default;

 private:
  std::vector<std::shared_ptr<DocNode>> Children;
  std::vector<const DocNode*> ChildrenView;

  std::weak_ptr<DocNode> Parent;

  std::string Name;
  std::string Since;
  std::string ShortDescription;
  std::string FullDescription;
  std::string DeprecationMessage;
  std::string DeprecationVersion;
  std::string Module;  // not using, but not deleting for now
  std::string Path;  // header for native, content path for content

  bool mIsDeprecated = false;

  uint64_t Id;
};
}  // namespace udocs_processor

