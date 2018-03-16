cmake_minimum_required(VERSION 2.8)

set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_C_COMPILER "arm-vita-eabi-gcc")
set(CMAKE_CXX_COMPILER "arm-vita-eabi-g++")

project(HENkaku)

set(CMAKE_C_FLAGS "-Wl,-q -Wall -O3 -std=gnu99")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11 -fno-rtti -fno-exceptions")

include_directories(
)

link_directories(
	${CMAKE_CURRENT_BINARY_DIR}
)

if (NOT ${RELEASE})
	add_definitions(-DENABLE_LOGGING)
endif()

add_executable(GN.elf
	main.c
)

target_link_libraries(GN.elf
	libSceDisplayForDriver_stub.a
	taihenForKernel_stub
	SceLedForDriver_stub
	ScePowerForDriver_stub
	SceGpioForDriver_stub
	)

set_target_properties(GN.elf
	PROPERTIES LINK_FLAGS "-nostdlib"
	COMPILE_FLAGS "-D__VITA_KERNEL__"
)

add_custom_target(GN.skprx ALL
	COMMAND vita-elf-create -e ${CMAKE_SOURCE_DIR}/GN.yml GN.elf GN.velf ${CMAKE_SOURCE_DIR}/taihen.json
	COMMAND vita-make-fself -c GN.velf GN.skprx
)
add_dependencies(GN.skprx GN.elf)

add_custom_target(send
	COMMAND curl -T GN.skprx ftp://$(PSVITAIP):1337/ux0:/data/tai/
	DEPENDS GN.skprx
)
