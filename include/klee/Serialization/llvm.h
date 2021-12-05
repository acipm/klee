#ifndef KLEE_SERIALIZE_LLVM_H
#define KLEE_SERIALIZE_LLVM_H

#include "llvm/ADT/APInt.h"

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/vector.hpp>

#include <vector>

namespace boost {
namespace serialization {

template<class Archive>
void save(Archive & ar, const llvm::APInt & g, const unsigned int version)
{
  unsigned bitwidth = g.getBitWidth();
  const uint64_t *rawdata = g.getRawData();
  std::vector<uint64_t> data{rawdata, rawdata + g.getNumWords()};
  ar << make_nvp("bitwidth", bitwidth) << make_nvp("data", data);
}

template<class Archive>
void load(Archive & ar, llvm::APInt & g, const unsigned int version)
{
  unsigned bitwidth;
  std::vector<uint64_t> data;
  ar >> make_nvp("bitwidth", bitwidth) >> make_nvp("data", data);
  g = llvm::APInt{bitwidth, llvm::ArrayRef<uint64_t>{data.data(), data.size()}};
}

template <class Archive>
void serialize(Archive & ar, llvm::APInt & o, const unsigned int version)
{
    boost::serialization::split_free(ar, o, version);
} 

} // namespace serialization
} // namespace boost
#endif /* KLEE_SERIALIZE_LLVM_H */
