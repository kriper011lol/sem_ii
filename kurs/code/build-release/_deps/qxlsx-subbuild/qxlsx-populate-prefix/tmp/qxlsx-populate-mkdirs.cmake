# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/Users/kriperlol/Desktop/лабораторные/astu_c++/sem_ii/kurs/code/build-release/_deps/qxlsx-src")
  file(MAKE_DIRECTORY "C:/Users/kriperlol/Desktop/лабораторные/astu_c++/sem_ii/kurs/code/build-release/_deps/qxlsx-src")
endif()
file(MAKE_DIRECTORY
  "C:/Users/kriperlol/Desktop/лабораторные/astu_c++/sem_ii/kurs/code/build-release/_deps/qxlsx-build"
  "C:/Users/kriperlol/Desktop/лабораторные/astu_c++/sem_ii/kurs/code/build-release/_deps/qxlsx-subbuild/qxlsx-populate-prefix"
  "C:/Users/kriperlol/Desktop/лабораторные/astu_c++/sem_ii/kurs/code/build-release/_deps/qxlsx-subbuild/qxlsx-populate-prefix/tmp"
  "C:/Users/kriperlol/Desktop/лабораторные/astu_c++/sem_ii/kurs/code/build-release/_deps/qxlsx-subbuild/qxlsx-populate-prefix/src/qxlsx-populate-stamp"
  "C:/Users/kriperlol/Desktop/лабораторные/astu_c++/sem_ii/kurs/code/build-release/_deps/qxlsx-subbuild/qxlsx-populate-prefix/src"
  "C:/Users/kriperlol/Desktop/лабораторные/astu_c++/sem_ii/kurs/code/build-release/_deps/qxlsx-subbuild/qxlsx-populate-prefix/src/qxlsx-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/kriperlol/Desktop/лабораторные/astu_c++/sem_ii/kurs/code/build-release/_deps/qxlsx-subbuild/qxlsx-populate-prefix/src/qxlsx-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/kriperlol/Desktop/лабораторные/astu_c++/sem_ii/kurs/code/build-release/_deps/qxlsx-subbuild/qxlsx-populate-prefix/src/qxlsx-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
