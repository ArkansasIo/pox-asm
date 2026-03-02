add_library(pox_common
  pox_crc16.c
  pox_alloc.c
  pox_rng.c
  pox_seedgen.c
  pox_vm8.c
)
target_include_directories(pox_common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
