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

  std::vector<std::string> calculateConcreteParameters(std::vector<ParameterExpressions> parameterExpressions, std::map<std::string, std::string> concreteValues) {
    // inserts concrete values into symbolic characters and calculate resulting parameters
    std::vector<std::string> parameters(parameterExpressions.size(), "");
    // for every fopen parameter
    for (unsigned i = 0; i < parameterExpressions.size(); i++) {
      // for every character in that fopen parameter
      for (unsigned j = 0; j < parameterExpressions[i].size(); j++) {
        if (ConstantExpr *ce = dyn_cast<ConstantExpr>(parameterExpressions[i][j])) {
          // if character is constant, add character to final string

          char letter = static_cast<char>(ce->getZExtValue());
          if (!isprint(letter)) {
            break;
          }
          parameters[i] += letter;

        } else {
          // else: character is symbolic

          // symbolicArrays describe all inputs that in any way contribute to the symbolic character
          std::vector<const Array *> symbolicArrays;
          std::vector<std::vector<unsigned char>> values;

          // find all symbolic arrays which describe the character e.g. arg00
          findSymbolicObjects(parameterExpressions[i][j], symbolicArrays);

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
          ref<Expr> result = a.evaluate(parameterExpressions[i][j]);

          // add resulting character to final parameter
          ConstantExpr *ce2 = dyn_cast<ConstantExpr>(result);
          char letter = static_cast<char>(ce2->getZExtValue());
          if (!isprint(letter)) {
            break;
          }
          parameters[i] += letter;
        }
      }
    }
    return parameters;
  }

  void Generator::generate() {
    for (auto entry : data) {
      if (!matchesConstraints(entry.constraints, concreteValues)) {
        continue;
      }
      std::vector<std::string> parameters = calculateConcreteParameters(entry.parameters, concreteValues);
      if (entry.identifier == "fopen") {
      	if (parameters.size() != 2) {
      	  // TODO error: invalid number of parameters
          continue;
      	}
#define chr(x) static_cast<char>(x)
        std::cout << "[THESIS fopen]" << chr(0) << parameters[0] << chr(0) << chr(0) << parameters[1] << chr(0) << "\n";
      }
    }
  }
}
