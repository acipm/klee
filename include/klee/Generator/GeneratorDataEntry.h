#ifndef KLEE_GENERATORDATAENTRY_H
#define KLEE_GENERATORDATAENTRY_H

#include "klee/Expr/Constraints.h"
#include "klee/Expr/Expr.h"

#include <memory>

namespace klee {
  typedef std::vector<ref<Expr>> ParameterExpressions;

  struct GeneratorDataEntry {
    /*** serialization ***/
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & identifier;
        ar & constraints;
        ar & parameters;
    }
    /*** end serialization ***/

    std::string identifier;
    ConstraintSet constraints;
    std::vector<ParameterExpressions> parameters;
  };
}

#endif /* KLEE_GENERATORDATAENTRY_H */
