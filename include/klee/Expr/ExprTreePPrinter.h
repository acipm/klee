#ifndef KLEE_EXPRTREEPPRINTER_H
#define KLEE_EXPRTREEPPRINTER_H

#include "klee/Expr/Expr.h"
#include "klee/Expr/ExprVisitor.h"

namespace klee {
  class ExprTreePPrinter : public ExprVisitor {

  public:
    ExprTreePPrinter() {}
    ref<Expr> visit(const ref<Expr> &e) override;
  };
}

#endif /* KLEE_EXPRTREEPPRINTER_H */
