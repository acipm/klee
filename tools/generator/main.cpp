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
#include <sys/stat.h>
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


  // needed to fix symbolic files; see runtime/POSIX/fd_init.c  
  struct stat64 file_stat;
  stat64(".", &file_stat);
  file_stat.st_ino = 0xFFFFFFFF00000001;
  file_stat.st_mode = 0x21A4;  
 
  // Convert arguments into correct format
  std::map<std::string, std::string> concreteValues = {{"model_version", "\1"},
                                                       {"A-data-stat", std::string((char *) &file_stat, (char *) (&file_stat + 1))},
                                                       {"stdin-stat", std::string((char *) &file_stat, (char *) (&file_stat + 1))},
                                                       {"stdout-stat", std::string((char *) &file_stat, (char *) (&file_stat + 1))}};
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
