#include "klee/Generator/GeneratorDataEntry.h"
#include "klee/Generator/Generator.h"
#include "klee/Serialization/llvm.h"
#include "klee/Serialization/Expr.h"
#include "klee/Serialization/Ref.h"
#include "klee/Serialization/Utils.h"
#include "klee/Support/ErrorHandling.h"
#include "klee/Solver/SolverCmdLine.h"

#include <map>
#include <string>
#include <vector>

using namespace llvm;
using namespace klee;

namespace {
  cl::opt<std::string>
  InputFile(cl::desc("<path to expressions>"), cl::Positional);

  cl::list<std::string>
  InputArgv(cl::ConsumeAfter,
            cl::desc("<program arguments>..."));
}

int main(int argc, char *argv[]) {
  // Options handling
  StringMap<cl::Option *> &map = cl::getRegisteredOptions();

  for (auto &elem : map) {
    elem.second->setHiddenFlag(cl::Hidden);
  }
  cl::ParseCommandLineOptions(argc, argv, " klee-generator\n");

  // Deserialize expressions file
  if (InputFile.empty()) {
    klee_error("path to expressions is missing; please see -h for help");
  }
  std::vector<GeneratorDataEntry> newGeneratorData;
  if (!deserialize(&newGeneratorData, InputFile)) {
    klee_error("path to expressions does not exist or could not be opened; please see -h for help");
  }
  
  // Convert arguments into correct format
  std::map<std::string, std::string> concreteValues = {{"model_version", "\1"}};
  for(unsigned i = 0; i < InputArgv.size(); i++) {
    std::string argument = "arg__";
    argument[3] = '0' + i / 10;
    argument[4] = '0' + i % 10;
    concreteValues.insert({argument, InputArgv[i]});
  }

  // Generate parameters
  Generator g = Generator(newGeneratorData, concreteValues);
  g.generate();

  return 0;
}
