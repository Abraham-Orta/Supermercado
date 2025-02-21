# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/supermercado_proyecto_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/supermercado_proyecto_autogen.dir/ParseCache.txt"
  "supermercado_proyecto_autogen"
  )
endif()
