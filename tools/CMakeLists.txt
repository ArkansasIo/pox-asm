add_executable(pox_packer packer.cpp)
target_include_directories(pox_packer PUBLIC ../common)
target_link_libraries(pox_packer PRIVATE pox_common)
