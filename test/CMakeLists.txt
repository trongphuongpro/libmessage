cmake_minimum_required(VERSION 3.0)

set(MCU **MCU name**)
set(F_CPU **frequency**)

#-----------------------------------------------------------------------------#
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CXX_COMPILER avr-g++)
set(CMAKE_C_COMPILER avr-gcc)

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

set(CMAKE_EXE_LINKER_FLAGS -mmcu=${MCU})
#-----------------------------------------------------------------------------#

add_subdirectory()

set(TARGET **target filename**)

add_executable(${TARGET} **source files**)
add_library(${TARGET} STATIC **source files**)
target_link_libraries(${TARGET} **lib**)
target_include_directories(${TARGET} PUBLIC **dirs**)

#-----------------------------------------------------------------------------#
target_compile_options(${TARGET} PUBLIC -mmcu=${MCU}
										-std=gnu11
										-O2
										-Wall
										-Werror
)

target_compile_definitions(${TARGET} PUBLIC F_CPU=${F_CPU})
#-----------------------------------------------------------------------------#
#set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME ${TARGET}.elf)
#add_custom_target(strip ALL avr-strip ${TARGET}.elf DEPENDS ${TARGET})
#add_custom_target(hex ALL avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex ${TARGET}.elf ${TARGET}.hex DEPENDS strip)
#add_custom_target(eeprom avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O ihex ${TARGET}.elf ${TARGET}.eep DEPENDS strip)
#add_custom_target(size ALL avr-size --format=avr --mcu=${MCU} ${TARGET}.elf DEPENDS strip)

#set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${TARGET}.hex;${TARGET}.eeprom;${TARGET}.lst")
#-----------------------------------------------------------------------------#
#install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.hex DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)