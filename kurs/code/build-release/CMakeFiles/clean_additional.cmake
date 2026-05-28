# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\ComplexCalculator_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\ComplexCalculator_autogen.dir\\ParseCache.txt"
  "ComplexCalculator_autogen"
  "minidocx\\CMakeFiles\\minidocx_autogen.dir\\AutogenUsed.txt"
  "minidocx\\CMakeFiles\\minidocx_autogen.dir\\ParseCache.txt"
  "minidocx\\minidocx_autogen"
  )
endif()
