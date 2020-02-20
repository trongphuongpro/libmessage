cmake_minimum_required(VERSION 3.0)

#-----------------------------------------------------------------------------#
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CXX_COMPILER avr-g++)
set(CMAKE_C_COMPILER avr-gcc)

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

set(CMAKE_EXE_LINKER_FLAGS -mmcu=${MCU})
#-----------------------------------------------------------------------------#

set(TARGET message)

add_library(${TARGET} STATIC src/uart_message_atmega.c
							src/ringbuffer.c
							lib/uart_atmega.c
							lib/crc32.c)

target_include_directories(${TARGET} PUBLIC include)

#-----------------------------------------------------------------------------#
target_compile_options(${TARGET} PUBLIC -mmcu=${MCU}
										-std=gnu11
										-O2
										-Wall
										-Werror
)

target_compile_definitions(${TARGET} PUBLIC F_CPU=${F_CPU})
#-----------------------------------------------------------------------------#