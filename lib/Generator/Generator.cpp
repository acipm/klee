#include "klee/Generator/Generator.h"
#include "klee/Generator/AppArmorProfile.h"
#include <fstream>

namespace klee {
  Generator::Generator(std::vector<GeneratorDataEntry> data, std::map<std::string, std::string> concreteValues, std::string fullPathToApp, std::string apparmorConfigPath) : data(data), concreteValues(concreteValues), fullPathToApp(fullPathToApp), apparmorConfigPath(apparmorConfigPath) {
    fileRuleMappings.push_back(std::make_unique<FopenMapping>());
  }

  std::string Generator::apparmorPathToFilename(std::string p) {
    std::replace(p.begin(), p.end(), '/', '.');
    if (p[0] == '.') {
      p = p.substr(1, p.size() - 1);
    }
    return p;
  }


  void Generator::generate() {
    std::vector<std::shared_ptr<AppArmorCommaRule>> commaRules;
    for (unsigned i = 0; i < data.size(); i++) {
      for (unsigned j = 0; j < fileRuleMappings.size(); j++) {
        if (fileRuleMappings[j]->Id() == data[i].identifier) {
          std::shared_ptr<AppArmorFileRule> rule = std::make_shared<AppArmorFileRule>(fileRuleMappings[j]->mapToFileRule(data[i], concreteValues));
          bool isDuplicate = false;
          for (const auto &commaRule : commaRules) {
            if (*commaRule == *rule) {
              isDuplicate = true;
              break;
            }
          }
          if (!isDuplicate) {
            commaRules.push_back(rule);
          }
        }
      }
    }

    std::ofstream profile;
    profile.open(apparmorConfigPath + apparmorPathToFilename(fullPathToApp));
    profile << AppArmorProfile(fullPathToApp, true, commaRules); 
    profile.close();
  }
}
