# Name:        CMakeLists.txt
# Description: build of the TMNT runtime and its development tools
# Version:     0.1.0
# Created:     29-10-2026
# Modified:    04-11-2026
# Author:      Sebastian Toyos
cmake_minimum_required(VERSION 3.20)
project(tmnt C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Every executable lands in build/bin (TMNT_BIN_DIR), whatever the generator or configuration
set(TMNT_BIN_DIR ${CMAKE_SOURCE_DIR}/build/bin CACHE PATH "Where the executables go")
foreach(config IN ITEMS "" _DEBUG _RELEASE _RELWITHDEBINFO _MINSIZEREL)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY${config} ${TMNT_BIN_DIR})
endforeach()

if(MSVC)
	add_compile_options(/W3 /utf-8)
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

# The board as the program sees it, its video and sound chips, and the assets' loader
add_library(tmnt_runtime STATIC
	runtime/src/machine.c
	runtime/src/sound.c
	runtime/src/script.c
	runtime/src/game_data.c
	runtime/src/inflate.c
	runtime/src/video.c
	runtime/src/ym2151.c
	runtime/src/sound_chips.c
	runtime/src/assets.c
	runtime/src/music.c
	runtime/src/png.c
)
target_include_directories(tmnt_runtime PUBLIC runtime/include PRIVATE runtime/src)

# The reader of the player's ROM set: for the tools that run the original code, never for the game
add_library(tmnt_rom_set STATIC runtime/src/rom_set.c)
target_link_libraries(tmnt_rom_set PUBLIC tmnt_runtime)
target_include_directories(tmnt_rom_set PRIVATE runtime/src)

# The game's own sound board: the decompiled sound program (src/sound/, tools/decompile_sound.py) on the Z80's
# timing model, with the board's own chips and the program's data from the assets (runtime/src/sound_cpu.c,
# sound_board.c). Nothing of MAME's goes into it; kref compares it with MAME's (--sound-board-check)
file(GLOB TMNT_SOUND_PROGRAM CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/sound/*.c)
if(TMNT_SOUND_PROGRAM)
	add_library(tmnt_sound STATIC runtime/src/sound_cpu.c runtime/src/sound_board.c ${TMNT_SOUND_PROGRAM})
	target_include_directories(tmnt_sound PUBLIC runtime/include PRIVATE runtime/src src/sound)
	target_link_libraries(tmnt_sound PUBLIC tmnt_runtime)
	if(MSVC)
		target_compile_options(tmnt_sound PRIVATE /wd4244)
	endif()
	set(K_GAME_SOUND tmnt_sound)
endif()

# The game: the decompiled 68000 program (src/, tools/decompile.py; TMNT_DECOMPILED_DIR picks another copy) on its
# runtime (runtime/src/game_program.c: the frame, the interrupts, the tables it reads by name from the assets), the
# board's code in place of the boot's ROM test (runtime/src/board_game.c), and the sound board. It opens no ROM:
# its data is the assets'
set(TMNT_DECOMPILED_DIR ${CMAKE_SOURCE_DIR}/src CACHE PATH "The game's C (tools/decompile.py)")
file(GLOB TMNT_DECOMPILED CONFIGURE_DEPENDS ${TMNT_DECOMPILED_DIR}/*.c ${TMNT_DECOMPILED_DIR}/*/*.c)
list(FILTER TMNT_DECOMPILED EXCLUDE REGEX "/sound/[^/]*\.c$")    # (the sound program's: tmnt_sound)
if(TMNT_DECOMPILED AND TMNT_SOUND_PROGRAM)
	add_library(tmnt_game STATIC runtime/src/game_program.c runtime/src/tasks.c runtime/src/board_game.c
		runtime/src/game.c ${TMNT_DECOMPILED})
	target_include_directories(tmnt_game PUBLIC runtime/include ${TMNT_DECOMPILED_DIR})
	target_link_libraries(tmnt_game PUBLIC tmnt_runtime tmnt_sound)
	if(MSVC)
		target_compile_options(tmnt_game PRIVATE /bigobj /wd4244 /wd4146)
	else()
		target_compile_options(tmnt_game PRIVATE -fno-strict-aliasing)
	endif()

	add_executable(tmnt_game_headless ports/headless/main.c)
	target_link_libraries(tmnt_game_headless PRIVATE tmnt_game)
	set(TMNT_WINDOW_PROGRAM tmnt_game)
endif()

# The translation scaffold: the 68000 runtime plus the C tools/recompile.py writes from the user's ROM into
# build/generated, an instruction at a time (the program running natively before the decompiler exists).
# Its data is the assets' (runtime/src/game.c): it opens no ROM; nothing of MAME's goes into it
option(TMNT_SCAFFOLD "Build the 68000 translation scaffold (tmnt_headless)" OFF)
file(GLOB TMNT_GENERATED CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/build/generated/*.c)
if(TMNT_SCAFFOLD AND TMNT_GENERATED AND TMNT_SOUND_PROGRAM)
	add_library(tmnt_program STATIC runtime/src/m68000.c runtime/src/tasks.c runtime/src/board_code.c
		runtime/src/game.c ${TMNT_GENERATED})
	target_include_directories(tmnt_program PUBLIC runtime/include ${CMAKE_SOURCE_DIR}/build/generated)
	target_link_libraries(tmnt_program PUBLIC tmnt_runtime tmnt_sound)
	if(MSVC)
		target_compile_options(tmnt_program PRIVATE /bigobj)
	endif()

	add_executable(tmnt_headless ports/headless/main.c)
	target_link_libraries(tmnt_headless PRIVATE tmnt_program)
	if(NOT TMNT_WINDOW_PROGRAM)
		set(TMNT_WINDOW_PROGRAM tmnt_program)
	endif()
elseif(TMNT_SCAFFOLD)
	message(STATUS "No generated program in build/generated (tools/recompile.py) or no sound program in src/sound: no scaffold")
endif()

# The game in a window (ports/sdl2): the decompiled game, or the scaffold where there is none yet. SDL2 from the
# system, or on Windows the development package unpacked in build/downloads/sdl2, a library beside the executable
if(TMNT_WINDOW_PROGRAM)

	set(TMNT_SDL2_UNPACKED "")
	if(WIN32)
		file(GLOB TMNT_SDL2_UNPACKED ${CMAKE_SOURCE_DIR}/build/downloads/sdl2/SDL2-*/cmake)
	endif()
	find_package(SDL2 CONFIG QUIET HINTS ${TMNT_SDL2_UNPACKED})
	if(SDL2_FOUND)
		set(TMNT_SDL2 SDL2::SDL2)
		if(TARGET SDL2::SDL2main)
			list(APPEND TMNT_SDL2 SDL2::SDL2main)
		endif()
		add_executable(tmnt WIN32 ports/sdl2/main.c)
		target_link_libraries(tmnt PRIVATE ${TMNT_WINDOW_PROGRAM} ${TMNT_SDL2})
		if(WIN32)
			add_custom_command(TARGET tmnt POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:SDL2::SDL2> $<TARGET_FILE_DIR:tmnt>)
		endif()
	else()
		message(STATUS "No SDL2: the window port (tmnt) is not built")
	endif()
endif()


find_package(Python3 COMPONENTS Interpreter)

# The reference tools: the original program on MAME's cores (tools/core/konami/reference)
option(TMNT_REFERENCE_TOOLS "Build the MAME-based reference tools" ON)
if(TMNT_REFERENCE_TOOLS)
	if(NOT Python3_FOUND)
		message(FATAL_ERROR "The reference tools need Python 3 (MAME's 68000 core is written by m68000gen.py)")
	endif()
	set(K_GAME_RUNTIME tmnt_rom_set)
	include(${CMAKE_SOURCE_DIR}/tools/core/konami/reference/reference.cmake)
endif()
