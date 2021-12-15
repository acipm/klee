#include "klee/Generator/Generator.h"
#include "klee/Expr/Assignment.h"
#include "klee/Expr/ExprUtil.h"

#include <iostream>

namespace klee {

  bool matchesConstraints(ConstraintSet constraints, std::map<std::string, std::string> concreteValues) {
    // checks if concreteValues inserted into constraints of the ConstraintSet will result 
    // the constraint being evalued as "true"

    // for every constraint that existed when fopen was reached once during generation
    for (auto ptr = constraints.begin(); ptr < constraints.end(); ptr++) {
      std::vector<const Array *> symbolicArrays;
      std::vector<std::vector<unsigned char>> values;
      findSymbolicObjects(*ptr, symbolicArrays);
      
      for (unsigned arrayIndex = 0; arrayIndex < symbolicArrays.size(); arrayIndex++) {
        auto it = concreteValues.find(symbolicArrays[arrayIndex]->name);
        if (it == concreteValues.end()) {
          // TODO error: missing key array->name
          // this means, the concreteValues need an entry with the name in symbolicArrays[arrayIndex]->name
          return false;
        }
        std::vector<unsigned char> concreteValue(it->second.begin(), it->second.end());
        values.push_back(concreteValue);
      }

      Assignment a(symbolicArrays, values);
      ref<Expr> result = a.evaluate(*ptr);
      if (!result->isTrue()) {
        return false;
      }
    }
    return true;
  }

  std::vector<ref<ConstantExpr>> insertConcreteValues(std::vector<ref<Expr>> parameterExpressions, std::map<std::string, std::string> concreteValues) {
    std::vector<ref<ConstantExpr>> constants;
    // inserts concrete values into symbolic characters and calculate resulting parameters
    for (unsigned i = 0; i < parameterExpressions.size(); i++) {
      if (ref<ConstantExpr> ce = dyn_cast<ConstantExpr>(parameterExpressions[i])) {
        constants.push_back(ce);
      } else {
        // else: character is symbolic

        // symbolicArrays describe all inputs that in any way contribute to the symbolic character
        std::vector<const Array *> symbolicArrays;
        std::vector<std::vector<unsigned char>> values;

        // find all symbolic arrays which describe the character e.g. arg00
        findSymbolicObjects(parameterExpressions[i], symbolicArrays);

        // for every symbolic array which describe the character e.g. arg00
        for (unsigned arrayIndex = 0; arrayIndex < symbolicArrays.size(); arrayIndex++) {
          // find the name of that symbolic array in the concrete values
          // e.g. for symbolic array with name "arg00" there must be entry such as {"arg00", "ab"} in concreteValues
          auto it = concreteValues.find(symbolicArrays[arrayIndex]->name);
          if (it == concreteValues.end()) {
            // TODO error: missing key array->name
            // this means, the concreteValues need an entry with the name in symbolicArrays[arrayIndex]->name
          }
          // add concreteValue entry to list that will be evaluated
          std::vector<unsigned char> concreteValue(it->second.begin(), it->second.end());
          values.push_back(concreteValue);
        }

        // insert concrete values into symbolic arrays and calculate resulting character
        Assignment a(symbolicArrays, values);
        ref<Expr> result = a.evaluate(parameterExpressions[i]);

        // add resulting character to final parameter
        ref<ConstantExpr> ce2 = dyn_cast<ConstantExpr>(result);
        constants.push_back(ce2);
      }
    }
    return constants;
  }

  std::string calculateConcreteString(std::vector<ref<Expr>> parameter, std::map<std::string, std::string> concreteValues) {
    std::string concreteString("");
    std::vector<ref<ConstantExpr>> constants = insertConcreteValues(parameter, concreteValues);
    for (unsigned i = 0; i < constants.size(); i++) {
      char letter = static_cast<char>(constants[i]->getZExtValue());
      if (!isprint(letter)) {
        break;
      }
      concreteString += letter;
    }
    return concreteString;
  }

  uint64_t calculateConcreteInt(std::vector<ref<Expr>> parameter, std::map<std::string, std::string> concreteValues) {
    std::vector<ref<ConstantExpr>> constants = insertConcreteValues(parameter, concreteValues);
    return constants[0]->getZExtValue(); 
  }

#define chr(x) static_cast<char>(x)
  void Generator::generate() {
    for (auto entry : data) {
      if (!matchesConstraints(entry.constraints, concreteValues)) {
        continue;
      }
      if (entry.identifier == "fopen") {
      	if (entry.parameters.size() != 2) {
      	  // TODO error: invalid number of parameters
          continue;
      	}
        std::string filepath = calculateConcreteString(entry.parameters[0], concreteValues);
        std::string mode = calculateConcreteString(entry.parameters[1], concreteValues);
        std::cout << "[THESIS fopen]" << chr(0) << filepath << chr(0) << chr(0) << mode << chr(0) << "\n";
      } else if (entry.identifier == "open") {
      	if (entry.parameters.size() != 2 && entry.parameters.size() != 3) {
      	  // TODO error: invalid number of parameters
          continue;
      	}
        std::string filepath = calculateConcreteString(entry.parameters[0], concreteValues);
        int flags = calculateConcreteInt(entry.parameters[1], concreteValues);
      	if (entry.parameters.size() == 2) {
          std::cout << "[THESIS open]" << chr(0) << filepath << chr(0) << chr(0) << flags << chr(0) << "\n";
      	} else if (entry.parameters.size() == 3) {
          int mode = calculateConcreteInt(entry.parameters[2], concreteValues);
          std::cout << "[THESIS open]" << chr(0) << filepath << chr(0) << chr(0) << flags << chr(0) << chr(0) << mode << chr(0) << "\n";
        }
      }
    }
  }
}
