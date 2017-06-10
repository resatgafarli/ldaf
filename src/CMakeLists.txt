######################### General Settings ######################
cmake_minimum_required(VERSION 3.8.2)
set(LDAF_VERSION_MAJOR 1 CACHE STRING "Ldaf major version")
set(LDAF_VERSION_MINOR 0 CACHE STRING "Ldaf minor version") 
set(LDAF_VERSION_PATCH 0 CACHE STRING "Ldaf patch version")
set(LDAF_VERSION ${LDAF_VERSION_MAJOR}.${LDAF_VERSION_MINOR}.${LDAF_VERSION_PATCH} CACHE STRING "Ldaf version") 
set(LDAF_SOLUTION_NAME ldaf${LDAF_VERSION} CACHE STRING "Ldaf solution name")
add_subdirectory(LDAF)
add_subdirectory(LDAFUnitTests)
