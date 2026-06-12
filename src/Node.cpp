#include "Node.hpp"

#include <cstdio>

// Factory functions
INode* make_value(int v) { return new Value(v); }
INode* make_op(INode* l, Ops o, INode* r) { return new Op(l, o, r); }
INode* make_while(INode* o, INode* s) { return new While(o, s); }
INode* make_if(INode* o, INode* s) { return new If(o, s); }
IScope* create_scope() { return new Scope(nullptr); }

// Value node implementation
RType Value::calc() { return val; }
void Value::dump() const { std::cout << val; }

// Decl node implementation
RType Decl::calc() { return val; }
void Decl::dump() const { std::cout << val; }
void Decl::SetValue(int Val) { val = Val; }

// Scope node implementation
IScope* Scope::resetScope() const { return prev_scope; }

void Scope::addBranch(INode* branch) { branches.push_back(branch); }

INode* Scope::access(std::string const& var_name) {
  if (INode* found = visible(var_name)) {
    return found;
  }
  auto* decl = new Decl();
  vars_[var_name] = decl;
  return decl;
}

INode* Scope::visible(std::string const& var_name) {
  auto* scope = this;
  while (scope) {
    auto it = scope->vars_.find(var_name);
    if (it != scope->vars_.end()) {
      return it->second;
    }
    scope = dynamic_cast<Scope*>(scope->prev_scope);
  }
  return nullptr;
}

RType Scope::calc() {
  for (auto* branch : branches) {
    branch->calc();
  }
  return 0;
}

void Scope::dump() const {
  std::cout << "{";
  for (auto* branch : branches) {
    branch->dump();
    std::cout << "; ";
  }
  std::cout << "}";
}

Scope::~Scope() {
  for (auto* branch : branches) {
    delete branch;
  }
  for (auto& [name, decl] : vars_) {
    delete decl;
  }
}

// Op node implementation
RType Op::calc() {
  switch (op) {
    case Ops::Plus:
      return left->calc() + right->calc();
    case Ops::Minus:
      return left->calc() - right->calc();
    case Ops::Mul:
      return left->calc() * right->calc();
    case Ops::Div:
      return left->calc() / right->calc();
    case Ops::Mod:
      return left->calc() % right->calc();
    case Ops::Greater:
      return left->calc() > right->calc();
    case Ops::Less:
      return left->calc() < right->calc();
    case Ops::GreaterEq:
      return left->calc() >= right->calc();
    case Ops::LessEq:
      return left->calc() <= right->calc();
    case Ops::Equal:
      return left->calc() == right->calc();
    case Ops::NotEqual:
      return left->calc() != right->calc();
    case Ops::And:
      return left->calc() && right->calc();
    case Ops::Or:
      return left->calc() || right->calc();
    case Ops::Not:
      return !right->calc();
    case Ops::Assign: {
      auto* decl = dynamic_cast<Decl*>(left);
      if (!decl) {
        return 0;
      }
      decl->SetValue(right->calc());
      return decl->calc();
    }
    case Ops::StdIn: {
      int v = 0;
      std::scanf("%d", &v);
      return v;
    }
    case Ops::StdOut:
      std::printf("%d\n", right->calc());
      return 0;
  }
  return 0;
}

void Op::dump() const {
  std::cout << "(";
  if (left) {
    left->dump();
  }
  std::cout << " op " << static_cast<int>(op) << " ";
  if (right) {
    right->dump();
  }
  std::cout << ")";
}

Op::~Op() {
  if (left && dynamic_cast<Decl*>(left) == nullptr) {
    delete left;
  }
  if (right && dynamic_cast<Decl*>(right) == nullptr) {
    delete right;
  }
}

// While node implementation
RType While::calc() {
  while (op->calc()) {
    scope->calc();
  }
  return 0;
}

void While::dump() const {
  std::cout << "while(";
  op->dump();
  std::cout << ") ";
  scope->dump();
}

While::~While() {
  delete op;
  delete scope;
}

// If node implementation
RType If::calc() {
  if (op->calc()) {
    scope->calc();
  }
  return 0;
}

void If::dump() const {
  std::cout << "if(";
  op->dump();
  std::cout << ") ";
  scope->dump();
}

If::~If() {
  delete op;
  delete scope;
}