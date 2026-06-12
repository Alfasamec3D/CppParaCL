#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "INode.hpp"

using RType = int;

// Integer literal value node
class Value final : public INode {
  int val;

 public:
  Value(int v) : val(v) {}
  RType calc() override;
  void dump() const override;
};

// Variable declaration node
class Decl final : public INode {
  int val;

 public:
  Decl() = default;
  void SetValue(int Val);
  RType calc() override;
  void dump() const override;
};

// Scope node with variable table and nested statements
class Scope final : public IScope {
  std::vector<INode*> branches;
  IScope* prev_scope;
  std::map<std::string, Decl*> vars_;

 public:
  Scope(Scope* prev) : prev_scope(prev) {}
  ~Scope();

  RType calc() override;
  void dump() const override;

  IScope* push() override { return new Scope(this); }
  IScope* resetScope() const override;
  void addBranch(INode* branch) override;
  INode* access(std::string const& var_name) override;
  INode* visible(std::string const& var_name) override;
};

// Binary/unary operation node
class Op final : public INode {
  INode* left;
  INode* right;
  Ops op;

 public:
  Op(INode* l, Ops o, INode* r) : left(l), right(r), op(o) {}
  ~Op();
  RType calc() override;
  void dump() const override;
};

// While loop node
class While final : public INode {
  INode* op = nullptr;
  INode* scope = nullptr;

 public:
  While(INode* o, INode* s) : op(o), scope(s) {}
  ~While();
  RType calc() override;
  void dump() const override;
};

// If conditional node
class If final : public INode {
  INode* op;
  INode* scope;

 public:
  If(INode* o, INode* s) : op(o), scope(s) {}
  ~If();
  RType calc() override;
  void dump() const override;
};