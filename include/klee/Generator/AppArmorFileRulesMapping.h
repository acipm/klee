#ifndef KLEE_APPARMORFILERULESMAPPING_H
#define KLEE_APPARMORFILERULESMAPPING_H

#include "klee/Expr/Assignment.h"
#include "klee/Expr/Constraints.h"
#include "klee/Expr/Expr.h"
#include "klee/Expr/ExprPPrinter.h"
#include "klee/Expr/ExprUtil.h"
#include "klee/Support/ErrorHandling.h"
#include "klee/Generator/AppArmorProfile.h"
#include "klee/Generator/GeneratorDataEntry.h"

namespace klee {
  
  class AppArmorFileRulesMapping { 
  protected:
    std::string APPARMOR_FILE_RULE_ACCESS_R = "r";
    std::string APPARMOR_FILE_RULE_ACCESS_W = "w";
    std::string APPARMOR_FILE_RULE_ACCESS_A = "a";

  public:
    virtual std::string Id() = 0;
    virtual AppArmorFileRule mapToFileRule(GeneratorDataEntry entry, std::map<std::string, std::string> concreteValues) = 0;
  };

  class FopenMapping : public AppArmorFileRulesMapping {
    // Mapping FOPEN_MODE to AppArmor file access rule

    char FOPEN_R = 'r';
    char FOPEN_W = 'w';
    char FOPEN_A = 'a';
    char FOPEN_PLUS = '+';
    char FOPEN_B = 'b';

  public:
    std::string Id() override;
    AppArmorFileRule mapToFileRule(GeneratorDataEntry entry, std::map<std::string, std::string> concreteValues) override;
  };
}

#endif /* KLEE_APPARMORFILERULESMAPPING_H */
