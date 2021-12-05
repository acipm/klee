#include <exception>
#include <iostream>

namespace boost {

void throw_exception(std::exception const & e) {
  std::cerr << e.what() << std::endl;
  std::terminate();
}

}  // namespace boost
