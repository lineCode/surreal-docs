/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include <vector>
#include "DocNode.h"
#include "DocNodeVisitor.h"

namespace udocs_processor {
class NodeTypeCheckerBase : public DocNodeVisitor {
 public:
  virtual bool Matched() const = 0;

  virtual bool CheckNode(DocNode& NodeToCheck) = 0;
};

// true if one of checkers returns true
class NodeMultiTypeChecker : public NodeTypeCheckerBase {
 public:
  void AddChecker(std::shared_ptr<NodeTypeCheckerBase> Checker) {
    Checkers.emplace_back(Checker);
  }

  bool Matched() const override {
    return HasMatched;
  }

  bool CheckNode(DocNode &NodeToCheck) override {
    if (Checkers.empty()) {
      HasMatched = true;
    } else {
      HasMatched = false;

      for (auto &Checker : Checkers) {
        bool Matched = Checker->CheckNode(NodeToCheck);

        if (Matched) {
          HasMatched = true;
          break;
        }
      }
    }

    return HasMatched;
  }

 private:
  bool HasMatched = false;

  std::vector<std::shared_ptr<NodeTypeCheckerBase>> Checkers;
};

template<typename T>
class NodeTypeChecker : public NodeTypeCheckerBase {
 public:
  explicit NodeTypeChecker(DocNode& Node) {
    Node.Accept(*this);
  }

  NodeTypeChecker() = default;

  bool CheckNode(DocNode& NodeToCheck) {
    IsTType = false;

    NodeToCheck.Accept(*this);

    return IsTType;
  }

  int Visit(T& Node) override {
    IsTType = true;
    this->Node = std::dynamic_pointer_cast<T>(Node.shared_from_this());

    return 0;
  }

  std::shared_ptr<T> GetNode() {
    return Node;
  }

  bool Matched() const override {
    return IsTType;
  }

 private:
  std::shared_ptr<T> Node;
  bool IsTType = false;
};

template<typename T>
class NodeTypeCheckerCast : public NodeTypeCheckerBase {
 public:
  explicit NodeTypeCheckerCast(DocNode& Node) {
    CheckNode(Node);
  }

  NodeTypeCheckerCast() = default;

  bool CheckNode(DocNode& NodeToCheck) {
    IsTType = false;

    if (auto Casted = std::dynamic_pointer_cast<T>(
        NodeToCheck.shared_from_this())) {
      Node = Casted;
      IsTType = true;
    }

    return IsTType;
  }

  std::shared_ptr<T> GetNode() {
    return Node;
  }

  bool Matched() const override {
    return IsTType;
  }

 private:
  std::shared_ptr<T> Node;
  bool IsTType = false;
};
}  // namespace udocs_processor
