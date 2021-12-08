#===------------------------------------------------------------------------===#
#
#                     The KLEE Symbolic Virtual Machine
#
# This file is distributed under the University of Illinois Open Source
# License. See LICENSE.TXT for details.
#
#===------------------------------------------------------------------------===#
add_executable(klee-generator
  main.cpp
)

set(KLEE_LIBS
  kleeCore
  kleaverExpr
  generator
)

target_link_libraries(klee-generator ${KLEE_LIBS})

install(TARGETS klee-generator RUNTIME DESTINATION bin)

# The KLEE binary depends on the runtimes
#add_dependencies(klee BuildKLEERuntimes)
