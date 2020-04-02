cmake_minimum_required(VERSION 3.0)

set(TARGET message)

#-----------------------------------------------------------------------------#

if (SERIES STREQUAL ATMEGA)
	add_library(${TARGET} STATIC src/uart_message_atmega.c
								src/messagebox.c
								lib/crc32_atmega.c
								lib/uart_atmega.c
	)

elseif (SERIES STREQUAL TIVA)
	add_library(${TARGET} STATIC src/uart_message_tiva.c
								src/messagebox.c
								lib/crc32_tiva.c
								lib/uart_tiva.c
								${TIVAWARE_PATH}/utils/uartstdio.c
	)

else()
	message(">> Failure due to missing SERIES.")

endif()

target_include_directories(${TARGET} PUBLIC include)

#-----------------------------------------------------------------------------#

if (SERIES STREQUAL ATMEGA)
	target_compile_options(${TARGET} PUBLIC -mmcu=${MCU}
											-std=gnu11
											-O2
											-g
											-Wall
											-Werror
	)

	target_compile_definitions(${TARGET} PUBLIC F_CPU=${F_CPU})

#-----------------------------------------------------------------------------#

elseif (SERIES STREQUAL TIVA)
	target_link_libraries(${TARGET} ${TIVAWARE_PATH}/driverlib/gcc/libdriver.a)
	target_include_directories(${TARGET} PRIVATE ${TIVAWARE_PATH})

	target_compile_options(${TARGET} PUBLIC -mthumb
											${CPU}
											${FPU}
											${FLOAT}         
											-std=gnu11
											-O2
											-g
											-Wall
											-Werror
	)

	target_compile_definitions(${TARGET} PUBLIC TARGET_IS_${REV}
												PART_${MCU}
	)

#-----------------------------------------------------------------------------#

else()
	message(">> Failure due to missing SERIES.")

endif()