add_executable(pox64_demo
  src/pox64_log.cpp
  src/pox64_io.cpp
  src/pox64_assetpack.cpp
  src/pox64_records.cpp
  src/pox64_world.cpp
  src/pox64_save.cpp
  src/pox64_game.cpp
  src/main.cpp
)
target_include_directories(pox64_demo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ../common)
target_link_libraries(pox64_demo PRIVATE pox_common)
