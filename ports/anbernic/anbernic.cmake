# Name:        anbernic.cmake
# Description: the build for Anbernic's H700 handhelds (the RG40XX H...) on their stock firmware: TMNT and TMNT Arcade,
#              as Roms/PORTS' TMNT.sh and TMNT Arcade.sh and their folder tmnt/
# Version:     0.1.0
# Created:     06-11-2026
# Modified:    06-11-2026
# Author:      Sebastian Toyos
#
# Included by CMakeLists.txt with TMNT_ANBERNIC (and the toolchain zig_aarch64.cmake: aarch64, glibc 2.35), as
# outrunners-port's. Both games are the computers' builds with their picture fixed (TMNT_HANDHELD: the whole screen,
# the game's 4:3 picture; no VIDEO nor CONTROLS: the handheld's buttons as a game controller's), the handheld's MENU
# and volume keys (TMNT_HANDHELD_KEYS), and with the firmware's SDL2: the executables are linked against a stand-in of
# its name (sdl2_stub.c) and SDL's headers (TMNT_SDL2_INCLUDE, by default the development package in
# build/downloads/sdl2). The build is PORTS's files without the assets, in build/bin/anbernic: the handheld makes
# nothing from the ROM set, and TMNT Anbernic (ports/anbernic/maker), on the player's computer, writes PORTS from them
# and the player's ROM set (one assets pack in tmnt/, both games').

target_compile_definitions(tmnt_console_game PUBLIC TMNT_HANDHELD TMNT_HANDHELD_KEYS)

set(TMNT_SDL2_INCLUDE "" CACHE PATH "SDL2's headers (the folder with SDL.h)")
if(NOT TMNT_SDL2_INCLUDE)
	file(GLOB TMNT_SDL2_INCLUDES ${CMAKE_SOURCE_DIR}/build/downloads/sdl2/SDL2-*/include)
	list(GET TMNT_SDL2_INCLUDES 0 TMNT_SDL2_INCLUDE)
endif()
if(NOT EXISTS ${TMNT_SDL2_INCLUDE}/SDL.h)
	message(FATAL_ERROR "No SDL.h in TMNT_SDL2_INCLUDE (${TMNT_SDL2_INCLUDE})")
endif()

# the stand-in for libSDL2-2.0.so.0, beside the build's own files (never in the package)
add_library(SDL2_device SHARED ports/anbernic/sdl2_stub.c)
set_target_properties(SDL2_device PROPERTIES OUTPUT_NAME SDL2-2.0 SUFFIX ".so.0" NO_SONAME ON
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/sdl2_device)
target_link_options(SDL2_device PRIVATE -Wl,-soname,libSDL2-2.0.so.0)

set(TMNT_ANBERNIC_OUT ${TMNT_BIN_DIR}/anbernic)    # (PORTS's files, beside TMNT Anbernic)

# A game of the handheld: its executable in the folder tmnt ('name', the window port's main.c and 'sources' besides)
function(tmnt_anbernic_game target name program)
	add_executable(${target} ports/sdl2/main.c ${ARGN})
	target_include_directories(${target} PRIVATE ${TMNT_SDL2_INCLUDE})
	target_link_libraries(${target} PRIVATE ${program} SDL2_device m)
	target_compile_definitions(${target} PRIVATE TMNT_HANDHELD TMNT_HANDHELD_KEYS)
	set_target_properties(${target} PROPERTIES OUTPUT_NAME ${name} RUNTIME_OUTPUT_DIRECTORY ${TMNT_ANBERNIC_OUT}/tmnt
		SKIP_BUILD_RPATH ON)    # (no path of this computer's in it: the handheld's loader finds its SDL2)
	foreach(config IN ITEMS _DEBUG _RELEASE _RELWITHDEBINFO _MINSIZEREL)
		set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY${config} ${TMNT_ANBERNIC_OUT}/tmnt)
	endforeach()
endfunction()

tmnt_anbernic_game(tmnt_anbernic "TMNT" tmnt_console_game ports/sdl2/controls.c)
tmnt_anbernic_game(tmnt_arcade_anbernic "TMNT Arcade" ${TMNT_WINDOW_PROGRAM})

# the launchers beside the folder; in it CONTROLS' button pictures (input_prompts.bmp). The assets pack is TMNT
# Anbernic's, made from the player's ROM set with the rest of PORTS
add_custom_target(tmnt_anbernic_games ALL
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/ports/anbernic/TMNT.sh
		"${CMAKE_SOURCE_DIR}/ports/anbernic/TMNT Arcade.sh" ${TMNT_ANBERNIC_OUT}
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/ports/sdl2/prompts/input_prompts.bmp
		${TMNT_ANBERNIC_OUT}/tmnt
	DEPENDS tmnt_anbernic tmnt_arcade_anbernic
	COMMENT "TMNT's and TMNT Arcade's PORTS files: build/bin/anbernic"
	VERBATIM)
