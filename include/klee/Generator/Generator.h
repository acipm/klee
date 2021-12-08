#ifndef KLEE_GENERATOR_H
#define KLEE_GENERATOR_H

#include "klee/Expr/Constraints.h"
#include "klee/Generator/GeneratorDataEntry.h"
#include "klee/Expr/Expr.h"

#include <algorithm>
#include <memory>

namespace klee {
  class Generator {
    std::vector<GeneratorDataEntry> data;
    std::map<std::string, std::string> concreteValues;
    
  public:
    Generator(std::vector<GeneratorDataEntry> data, std::map<std::string, std::string> concreteValues) : data(data), concreteValues(concreteValues) {}

    void generate();
  };
}

#endif /* KLEE_GENERATOR_H */
