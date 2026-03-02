add_executable(pox8_sim
  src/pox8_platform_sim.c
  src/pox8_bios.c
  src/pox8_eeprom_sim.c
  src/pox8_records.c
  src/pox8_game.c
  src/pox8_vm_bridge.c
  src/pox8_main.c
)
target_include_directories(pox8_sim PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ../common)
target_link_libraries(pox8_sim PRIVATE pox_common)
