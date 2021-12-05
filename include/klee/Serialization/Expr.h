#ifndef KLEE_SERIALIZE_EXPR_H
#define KLEE_SERIALIZE_EXPR_H

#include "klee/Expr/Expr.h"
#include "klee/Serialization/Ref.h"

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>

#include <vector>

namespace boost {
namespace serialization {

// Array
template<class Archive>
inline void save_construct_data(Archive & ar, const klee::Array * t, const unsigned int version) {
  ar << make_nvp("name", t->name);
  ar << make_nvp("size", t->size);
  ar << make_nvp("domain", t->domain);
  ar << make_nvp("range", t->range);
  ar << make_nvp("constantValues", t->constantValues);
}

template<class Archive>
inline void load_construct_data(Archive & ar, klee::Array * t, const unsigned int version) {
  std::string name;
  unsigned size;
  klee::Expr::Width domain, range;
  std::vector<klee::ref<klee::ConstantExpr>> constantValues;
  ar >> make_nvp("name", name);
  ar >> make_nvp("size", size);
  ar >> make_nvp("domain", domain);
  ar >> make_nvp("range", range);
  ar >> make_nvp("constantValues", constantValues);
  // XXX leaks memory. how to use ArrayCache::CreateArray()?
  ::new(t)klee::Array{name, size, &*constantValues.begin(), &*constantValues.end(), domain, range};
}

// UpdateNode
template<class Archive>
inline void save_construct_data(Archive & ar, const klee::UpdateNode * t, const unsigned int version) {
  ar << make_nvp("next", t->next);
  ar << make_nvp("index", t->index);
  ar << make_nvp("index", t->value);
}

template<class Archive>
inline void load_construct_data(Archive & ar, klee::UpdateNode * t, const unsigned int version) {
  klee::ref<klee::UpdateNode> next;
  klee::ref<klee::Expr> index;
  klee::ref<klee::Expr> value;
  ar >> make_nvp("next", next);
  ar >> make_nvp("index", index);
  ar >> make_nvp("value", value);
  ::new(t)klee::UpdateNode{next, index, value};
}

} // namespace serialization
} // namespace boost
#endif /* KLEE_SERIALIZE_EXPR_H */
