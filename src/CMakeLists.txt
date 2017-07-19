add_library(DIFrame unsafe_module.cpp)

install(TARGETS DIFrame
  ARCHIVE DESTINATION lib/static)