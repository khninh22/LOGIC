# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\NEWGUI_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\NEWGUI_autogen.dir\\ParseCache.txt"
  "NEWGUI_autogen"
  )
endif()
