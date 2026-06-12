#pragma once

#include <string>

// Base interface for all AST nodes
struct INode {
  virtual int calc() = 0;
  virtual void dump() const = 0;
  virtual ~INode() {}
};

// Scope interface for managing variable declarations and nested scopes
struct IScope : public INode {
  virtual IScope* push() = 0;
  virtual IScope* resetScope() const = 0;
  virtual void addBranch(INode* branch) = 0;
  virtual INode* access(std::string const& var_name) = 0;
  virtual INode* visible(std::string const& var_name) = 0;
};

// Binary and unary operations
enum class Ops {
  Plus,
  Minus,
  Assign,
  Greater,
  Less,
  GreaterEq,
  LessEq,
  StdOut,
  StdIn,
  Equal,
  NotEqual,
  Div,
  Mul,
  Mod,
  Not,
  Or,
  And
};

// Factory functions for creating AST nodes
INode* make_value(int);
INode* make_op(INode* l, Ops o, INode* r);
INode* make_while(INode* o, INode* s);
INode* make_if(INode* o, INode* s);
IScope* create_scope();