#include "klee/Generator/Generator.h"
#include "klee/Generator/AppArmorProfile.h"
#include <fstream>

namespace klee {
  Generator::Generator(std::vector<GeneratorDataEntry> data, std::map<std::string, std::string> concreteValues) : data(data), concreteValues(concreteValues) {
    fileRuleMappings.push_back(std::make_unique<FopenMapping>());
  }

  void Generator::generate() {
    std::vector<std::shared_ptr<AppArmorCommaRule>> commaRules;
    for (unsigned i = 0; i < data.size(); i++) {
      for (unsigned j = 0; j < fileRuleMappings.size(); j++) {
        if (fileRuleMappings[j]->Id() == data[i].identifier) {
          std::shared_ptr<AppArmorFileRule> rule = std::make_shared<AppArmorFileRule>(fileRuleMappings[j]->mapToFileRule(data[i], concreteValues));
          commaRules.push_back(rule);
        }
      }
    }
    // TODO remove duplicates
    // TODO write to correct files
    std::ofstream profile;
    profile.open("profile.txt");
    profile << AppArmorProfile("/some/path/whatever", true, commaRules); 
    profile.close();
  }
}
