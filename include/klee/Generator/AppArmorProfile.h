#ifndef KLEE_APPARMORPROFILE_H
#define KLEE_APPARMORPROFILE_H

#include <iostream>
#include <vector>

namespace klee {
  /** RULES **/

  class AppArmorCommaRule {
  public:
    friend std::ostream& operator<<(std::ostream &os, const AppArmorCommaRule &dt) {
      return dt.print(os);
    }
    virtual std::ostream& print(std::ostream &os) const = 0;
    virtual bool operator ==(const AppArmorCommaRule &b) const = 0;
  };
  
  class AppArmorFileRule : public AppArmorCommaRule {
    std::string fileglob;
    std::string access;
    bool owner;

  public:
    AppArmorFileRule(std::string fileglob, std::string access, bool owner = false) : fileglob(fileglob), access(access), owner(owner) {}
    virtual ~AppArmorFileRule() {}

    std::ostream& print(std::ostream& os) const override {
      if (owner) {
        os << "owner ";
      }
      os << fileglob  << " " << access;
      return os;
    }

    bool operator ==(const AppArmorCommaRule &b) const {
       if (const AppArmorFileRule *ce = dynamic_cast<const AppArmorFileRule*>(&b)) {
         if (fileglob == ce->fileglob && access == ce->access && owner == ce->owner) {
           return true;
         }
       }
       return false;
    }
  };

  /** PROFILE **/  

  class AppArmorProfile {
    std::string name;
    bool fileglob;
    std::vector<std::shared_ptr<AppArmorCommaRule>> commaRules;
   
  public:
    AppArmorProfile(std::string name, bool fileglob = false, std::vector<std::shared_ptr<AppArmorCommaRule>> commaRules = {}) : name(name), fileglob(fileglob), commaRules(commaRules) {}

    friend std::ostream& operator<<(std::ostream &os, const AppArmorProfile &dt) {
      if (!dt.fileglob) {
        os << "profile";
      }
      os << dt.name << " {\n";
      if (!dt.commaRules.empty()) {
        for (unsigned i = 0; i < dt.commaRules.size(); i++) {
          os << "  " << *dt.commaRules[i] << ",\n";
        }
        os << "}\n";
      }
      return os;
    }
  };
}

#endif /* KLEE_APPARMORPROFILE_H */
