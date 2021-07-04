#include "klee/Expr/ExprTreePPrinter.h"

using namespace klee;
ref<Expr> ExprTreePPrinter::visit(const ref<Expr> &e) {
  if (!isa<ConstantExpr>(e)) {
    e->dump();
  }
  return ExprVisitor::visit(e); 
}
