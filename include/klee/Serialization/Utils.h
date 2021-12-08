#ifndef KLEE_SERIALIZATION_UTILS_H
#define KLEE_SERIALIZATION_UTILS_H

#include <klee/Generator/GeneratorDataEntry.h>

#include <boost/throw_exception.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <fstream>
#include <vector>

namespace klee {

void serialize(std::vector<GeneratorDataEntry> generatorData, std::string outfile) {
  std::ofstream ofs(outfile);
  boost::archive::text_oarchive oa(ofs);
  // write class instance to archive
  oa.register_type(static_cast<ConstantExpr *>(nullptr));
  oa.register_type(static_cast<ConcatExpr *>(nullptr));
  oa.register_type(static_cast<ExtractExpr *>(nullptr));
  oa.register_type(static_cast<NotExpr *>(nullptr));
  oa.register_type(static_cast<NotOptimizedExpr *>(nullptr));
  oa.register_type(static_cast<ReadExpr *>(nullptr));
  oa.register_type(static_cast<SelectExpr *>(nullptr));

  oa.register_type(static_cast<SExtExpr *>(nullptr));
  oa.register_type(static_cast<ZExtExpr *>(nullptr));

  oa.register_type(static_cast<AddExpr *>(nullptr));
  oa.register_type(static_cast<SubExpr *>(nullptr));
  oa.register_type(static_cast<MulExpr *>(nullptr));
  oa.register_type(static_cast<UDivExpr *>(nullptr));
  oa.register_type(static_cast<SDivExpr *>(nullptr));
  oa.register_type(static_cast<URemExpr *>(nullptr));
  oa.register_type(static_cast<SRemExpr *>(nullptr));
  oa.register_type(static_cast<AndExpr *>(nullptr));
  oa.register_type(static_cast<OrExpr *>(nullptr));
  oa.register_type(static_cast<XorExpr *>(nullptr));
  oa.register_type(static_cast<ShlExpr *>(nullptr));
  oa.register_type(static_cast<LShrExpr *>(nullptr));
  oa.register_type(static_cast<AShrExpr *>(nullptr));

  oa.register_type(static_cast<EqExpr *>(nullptr));
  oa.register_type(static_cast<NeExpr *>(nullptr));
  oa.register_type(static_cast<UltExpr *>(nullptr));
  oa.register_type(static_cast<UleExpr *>(nullptr));
  oa.register_type(static_cast<UgtExpr *>(nullptr));
  oa.register_type(static_cast<UgeExpr *>(nullptr));
  oa.register_type(static_cast<SltExpr *>(nullptr));
  oa.register_type(static_cast<SleExpr *>(nullptr));
  oa.register_type(static_cast<SgtExpr *>(nullptr));
  oa.register_type(static_cast<SgeExpr *>(nullptr));

  oa << generatorData;
}

bool deserialize(std::vector<GeneratorDataEntry> *generatorData, std::string infile) {
  // create and open an archive for input
  std::ifstream ifs(infile);
  if (!ifs.good()) {
    return false;
  }
  boost::archive::text_iarchive ia(ifs);
  // read class state from archive
  ia.register_type(static_cast<ConstantExpr *>(nullptr));
  ia.register_type(static_cast<ConcatExpr *>(nullptr));
  ia.register_type(static_cast<ExtractExpr *>(nullptr));
  ia.register_type(static_cast<NotExpr *>(nullptr));
  ia.register_type(static_cast<NotOptimizedExpr *>(nullptr));
  ia.register_type(static_cast<ReadExpr *>(nullptr));
  ia.register_type(static_cast<SelectExpr *>(nullptr));

  ia.register_type(static_cast<SExtExpr *>(nullptr));
  ia.register_type(static_cast<ZExtExpr *>(nullptr));

  ia.register_type(static_cast<AddExpr *>(nullptr));
  ia.register_type(static_cast<SubExpr *>(nullptr));
  ia.register_type(static_cast<MulExpr *>(nullptr));
  ia.register_type(static_cast<UDivExpr *>(nullptr));
  ia.register_type(static_cast<SDivExpr *>(nullptr));
  ia.register_type(static_cast<URemExpr *>(nullptr));
  ia.register_type(static_cast<SRemExpr *>(nullptr));
  ia.register_type(static_cast<AndExpr *>(nullptr));
  ia.register_type(static_cast<OrExpr *>(nullptr));
  ia.register_type(static_cast<XorExpr *>(nullptr));
  ia.register_type(static_cast<ShlExpr *>(nullptr));
  ia.register_type(static_cast<LShrExpr *>(nullptr));
  ia.register_type(static_cast<AShrExpr *>(nullptr));

  ia.register_type(static_cast<EqExpr *>(nullptr));
  ia.register_type(static_cast<NeExpr *>(nullptr));
  ia.register_type(static_cast<UltExpr *>(nullptr));
  ia.register_type(static_cast<UleExpr *>(nullptr));
  ia.register_type(static_cast<UgtExpr *>(nullptr));
  ia.register_type(static_cast<UgeExpr *>(nullptr));
  ia.register_type(static_cast<SltExpr *>(nullptr));
  ia.register_type(static_cast<SleExpr *>(nullptr));
  ia.register_type(static_cast<SgtExpr *>(nullptr));
  ia.register_type(static_cast<SgeExpr *>(nullptr));

  ia >> *generatorData;
  return true;
}
}

#endif /* KLEE_SERIALIZATION_UTILS_H */
