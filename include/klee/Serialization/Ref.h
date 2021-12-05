#ifndef KLEE_SERIALIZE_REF_H
#define KLEE_SERIALIZE_REF_H

#include <boost/serialization/shared_ptr_helper.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/tracking.hpp>

#include <klee/ADT/Ref.h>

namespace boost {
namespace serialization {

template<class T>
struct version< ::klee::ref< T > > {
  typedef mpl::integral_c_tag tag;
  typedef mpl::int_<1> type;
  BOOST_STATIC_CONSTANT(int, value = type::value);
};
// don't track shared pointers
template<class T>
struct tracking_level< ::klee::ref< T > > {
  typedef mpl::integral_c_tag tag;
  typedef mpl::int_< ::boost::serialization::track_never> type;
  BOOST_STATIC_CONSTANT(int, value = type::value);
};

template<class Archive, class T>
inline void save(
  Archive & ar,
  const klee::ref< T > &t,
  const unsigned int /* file_version */
){
  BOOST_STATIC_ASSERT((tracking_level< T >::value != track_never));
  const T * t_ptr = t.get();
  ar << boost::serialization::make_nvp("px", t_ptr);
}

template<class Archive, class T>
inline void load(
  Archive & ar,
  klee::ref< T > &t,
  const unsigned int /*file_version*/
){
  BOOST_STATIC_ASSERT((tracking_level< T >::value != track_never));
  T* r;
  ar >> boost::serialization::make_nvp("px", r);
  t = klee::ref<T>(r);
}

template<class Archive, class T>
inline void serialize(
  Archive & ar,
  klee::ref< T > &t,
  const unsigned int file_version
){
  BOOST_STATIC_ASSERT(
    boost::serialization::tracking_level< T >::value
    != boost::serialization::track_never
  );
  boost::serialization::split_free(ar, t, file_version);
}

} // namespace serialization
} // namespace boost

#endif /* KLEE_SERIALIZE_REF_H */
