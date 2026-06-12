#include "Node.hpp"
#include "Symtab.hpp"

Symtab globalTable;

INode* make_value(int v) { return new Value{v}; }
INode* make_op(INode* l, Ops o, INode* r) { return new Op{l, o, r}; }
INode* make_while(INode* o, INode* s) { return new Whiel{o, s}; }
INode* make_if(INode* o, INode* s) { return If{o, s}; }
IScope* create_scope() { return new Scope{nullptr}; }

// NUMBER
RType Value::calc() { return val; }
void Value::dump() const { std::cout << "Node Value: " << val << std::endl; }

RType Decl::calc() { return val; }
void Decl::dump() const { std::cout << "Node Decl: " << val << std::endl; }
void Decl::SetValue(int Val) { val = Val; }

//SCOPE
"Node.cpp"