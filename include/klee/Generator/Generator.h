#ifndef KLEE_GENERATOR_H
#define KLEE_GENERATOR_H

#include "klee/Expr/Constraints.h"
#include "klee/Expr/Expr.h"
#include "klee/Generator/AppArmorFileRulesMapping.h"

#include <memory>

namespace klee {
  class Generator {
    std::vector<std::unique_ptr<AppArmorFileRulesMapping>> fileRuleMappings;
    std::vector<GeneratorDataEntry> data;
    std::map<std::string, std::string> concreteValues;
    
  public:
    Generator(std::vector<GeneratorDataEntry> data, std::map<std::string, std::string> concreteValues);
    void generate();
  };
}

#endif /* KLEE_GENERATOR_H */
