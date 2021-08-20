#include "klee/Generator/AppArmorFileRulesMapping.h"
#include <algorithm>

namespace klee {

  std::string FopenMapping::Id() {
    return "fopen";
  }

  std::vector<std::string> calculateConcreteParameters(GeneratorDataEntry entry, std::map<std::string, std::string> concreteValues) {
    std::vector<std::string> parameters(entry.parameters.size(), "");
    for (unsigned i = 0; i < entry.parameters.size(); i++) {
      for (unsigned j = 0; j < entry.parameters[i].size(); j++) {
        if (ConstantExpr *ce = dyn_cast<ConstantExpr>(entry.parameters[i][j])) {
          parameters[i] += static_cast<char>(ce->getZExtValue());
        } else {
          std::vector<const Array *> symbolicArrays;
          std::vector<std::vector<unsigned char>> values;
          findSymbolicObjects(entry.parameters[i][j], symbolicArrays);

          for (unsigned arrayIndex = 0; arrayIndex < symbolicArrays.size(); arrayIndex++) {
            auto it = concreteValues.find(symbolicArrays[arrayIndex]->name);
            if (it == concreteValues.end()) {
              // TODO error: missing key array->name
            }
            std::vector<unsigned char> concreteValue(it->second.begin(), it->second.end());
            values.push_back(concreteValue);
          }

          Assignment a(symbolicArrays, values);
          ref<Expr> result = a.evaluate(entry.parameters[i][j]);
          ce = dyn_cast<ConstantExpr>(result);
          ConstantExpr *ce2 = dyn_cast<ConstantExpr>(result);
          parameters[i] += static_cast<char>(ce2->getZExtValue());
        }
      }
    }
    return parameters;
  }

  AppArmorFileRule FopenMapping::mapToFileRule(GeneratorDataEntry entry, std::map<std::string, std::string> concreteValues) {
    std::vector<std::string> parameters = calculateConcreteParameters(entry, concreteValues);

    // TODO first parameter: filepath instead of string
    // TODO first parameter: extend to absolute path

    // remove 'b' as it does nothing
    std::string apparmor_access;
    parameters[1].erase(std::remove(parameters[1].begin(), parameters[1].end(), FOPEN_B), parameters[1].end());

    if (parameters[1][0] == FOPEN_R) {
      if (parameters[1][1] == FOPEN_PLUS) {
        apparmor_access = APPARMOR_FILE_RULE_ACCESS_R + APPARMOR_FILE_RULE_ACCESS_W; 
      } else {
        apparmor_access = APPARMOR_FILE_RULE_ACCESS_R;
      } 
    } else if (parameters[1][0] == FOPEN_W) {
      if (parameters[1][1] == FOPEN_PLUS) {
        apparmor_access = APPARMOR_FILE_RULE_ACCESS_R + APPARMOR_FILE_RULE_ACCESS_W; 
      } else {
        apparmor_access = APPARMOR_FILE_RULE_ACCESS_W; 
      } 
    } else if (parameters[1][0] == FOPEN_A) {
      if (parameters[1][1] == FOPEN_PLUS) {
        apparmor_access = APPARMOR_FILE_RULE_ACCESS_R + APPARMOR_FILE_RULE_ACCESS_W; 
      } else {
        apparmor_access = APPARMOR_FILE_RULE_ACCESS_W; 
      } 
    } else {
      // TODO
    }

    return AppArmorFileRule(parameters[0], apparmor_access);
  }
}
