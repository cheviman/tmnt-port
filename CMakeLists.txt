# Name:        CMakeLists.txt
# Description: build of the TMNT runtime and its development tools
# Version:     0.2.0
# Created:     29-10-2026
# Modified:    04-11-2026
# Author:      Sebastian Toyos
cmake_minimum_required(VERSION 3.20)
project(tmnt VERSION 0.1.0 LANGUAGES C CXX)    # (the port's release: TMNT's OPTIONS shows it)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Every executable lands in build/bin (TMNT_BIN_DIR, a relative one in the source folder), whatever the generator
# or configuration
set(TMNT_BIN_DIR ${CMAKE_SOURCE_DIR}/build/bin CACHE PATH "Where the executables go")
get_filename_component(TMNT_BIN_DIR ${TMNT_BIN_DIR} ABSOLUTE BASE_DIR ${CMAKE_SOURCE_DIR})
foreach(config IN ITEMS "" _DEBUG _RELEASE _RELWITHDEBINFO _MINSIZEREL)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY${config} ${TMNT_BIN_DIR})
endforeach()

# The packages a player downloads (.github/workflows/build.yml) carry SDL2 inside the window port, built from its
# own source: the package is the executable alone, no library beside it
option(TMNT_STATIC_SDL2 "Build SDL2 from its source into the executables" OFF)

if(MSVC)
	add_compile_options(/W3 /utf-8)
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
	# with TMNT_STATIC_SDL2 the C runtime goes inside the executables too: they need no Visual C++ redistributable
	# on the player's machine
	if(TMNT_STATIC_SDL2)
		set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
		set(CMAKE_POLICY_DEFAULT_CMP0091 NEW)    # (SDL2's targets too)
	endif()
endif()

find_package(Python3 COMPONENTS Interpreter)

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

# The reader of the player's ROM set: for the tools that run the original code, and for the game's first run,
# which takes the assets out of it once (the game never runs on it)
add_library(tmnt_rom_set STATIC runtime/src/rom_set.c)
target_link_libraries(tmnt_rom_set PUBLIC tmnt_runtime)
target_include_directories(tmnt_rom_set PRIVATE runtime/src)

# The game's first run: its assets out of the player's ROM set in C (runtime/src/extract.c), the files
# tools/extract_assets.py writes, byte for byte; what the analysis works out (the pieces, their names and formats,
# the sheets' codes and palettes) as runtime/src/extract_layout.c has it, which tools/extract_layout.py writes with
# positions, names and codes only, no byte of the ROM. The PNGs packed as zlib's level 9 packs them (deflate.c)
add_library(tmnt_extract STATIC runtime/src/extract.c runtime/src/extract_graphics.c runtime/src/extract_samples.c
	runtime/src/extract_music.c runtime/src/extract_layout.c runtime/src/deflate.c)
target_include_directories(tmnt_extract PUBLIC runtime/include PRIVATE runtime/src)
target_link_libraries(tmnt_extract PUBLIC tmnt_rom_set)
if(NOT MSVC)
	target_link_libraries(tmnt_extract PUBLIC m)
endif()

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
		runtime/src/game.c runtime/src/assets_pack.c ${TMNT_DECOMPILED})
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

	# The console version (TMNT, console/PLAN.md): the same game with the console's own versions of the functions
	# console/replaced.txt lists (console/*.c, TMNT_CONSOLE: their originals are the decompiled C's
	# CONSOLE_ORIGINAL definitions), and a headless build of it for scripted checks (its menus' actions and its
	# frames as pictures). It writes its menus with Kenney Mini, font.bmp beside it
	file(GLOB TMNT_CONSOLE_SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/console/*.c)
	add_library(tmnt_console_game STATIC runtime/src/game_program.c runtime/src/tasks.c runtime/src/board_game.c
		runtime/src/game.c runtime/src/assets_pack.c ${TMNT_DECOMPILED} ${TMNT_CONSOLE_SOURCES})
	target_include_directories(tmnt_console_game PUBLIC runtime/include ${TMNT_DECOMPILED_DIR} console)
	target_compile_definitions(tmnt_console_game PUBLIC TMNT_CONSOLE TMNT_VERSION="${PROJECT_VERSION}")
	target_link_libraries(tmnt_console_game PUBLIC tmnt_runtime tmnt_sound)
	if(MSVC)
		target_compile_options(tmnt_console_game PRIVATE /bigobj /wd4244 /wd4146)
	else()
		target_compile_options(tmnt_console_game PRIVATE -fno-strict-aliasing)
	endif()

	add_executable(tmnt_console_headless ports/headless/main.c)
	target_link_libraries(tmnt_console_headless PRIVATE tmnt_console_game)
	add_custom_command(TARGET tmnt_console_headless POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/ports/sdl2/font/font.bmp
			$<TARGET_FILE_DIR:tmnt_console_headless>)
endif()

# The translation scaffold: the 68000 runtime plus the C tools/recompile.py writes from the user's ROM into
# build/generated, an instruction at a time (the program running natively before the decompiler exists).
# Its data is the assets' (runtime/src/game.c): it opens no ROM; nothing of MAME's goes into it
option(TMNT_SCAFFOLD "Build the 68000 translation scaffold (tmnt_headless)" OFF)
file(GLOB TMNT_GENERATED CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/build/generated/*.c)
if(TMNT_SCAFFOLD AND TMNT_GENERATED AND TMNT_SOUND_PROGRAM)
	add_library(tmnt_program STATIC runtime/src/m68000.c runtime/src/tasks.c runtime/src/board_code.c
		runtime/src/game.c runtime/src/assets_pack.c ${TMNT_GENERATED})
	target_compile_definitions(tmnt_program PRIVATE TMNT_TRANSLATION_SCAFFOLD)
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
# system, or on Windows the development package unpacked in build/downloads/sdl2, a library beside the executable;
# or, with TMNT_STATIC_SDL2, SDL2 built from its own source into the executable
if(TMNT_WINDOW_PROGRAM)
	if(TMNT_STATIC_SDL2)
		include(FetchContent)
		FetchContent_Declare(SDL2 GIT_REPOSITORY https://github.com/libsdl-org/SDL.git GIT_TAG release-2.32.10
			GIT_SHALLOW TRUE)
		set(SDL_SHARED OFF CACHE BOOL "" FORCE)
		set(SDL_STATIC ON CACHE BOOL "" FORCE)
		set(SDL_TEST OFF CACHE BOOL "" FORCE)
		set(SDL_FORCE_STATIC_VCRT ON CACHE BOOL "" FORCE)    # (MSVC: the C runtime inside, as the game's)
		set(SDL_CCACHE OFF CACHE BOOL "" FORCE)    # (its block asks for CMake 3.4, which CMake warns is too old)
		FetchContent_MakeAvailable(SDL2)
		set(TMNT_SDL2 SDL2::SDL2-static)
		set(SDL2_FOUND TRUE)
	else()
		set(TMNT_SDL2_UNPACKED "")
		if(WIN32)
			file(GLOB TMNT_SDL2_UNPACKED ${CMAKE_SOURCE_DIR}/build/downloads/sdl2/SDL2-*/cmake)
		endif()
		find_package(SDL2 CONFIG QUIET HINTS ${TMNT_SDL2_UNPACKED})
		set(TMNT_SDL2 SDL2::SDL2)
	endif()
	if(SDL2_FOUND)
		if(TARGET SDL2::SDL2main)
			list(APPEND TMNT_SDL2 SDL2::SDL2main)
		endif()
		# the arcade game, "TMNT Arcade" its executable and its window. Its first run takes the assets out of the
		# player's ROM set (ports/sdl2/first_run.c, tmnt_extract), its window written with Kenney Mini (CC0,
		# ports/sdl2/font), a picture beside the executable
		add_executable(tmnt WIN32 ports/sdl2/main.c ports/sdl2/first_run.c ports/sdl2/letters.c)
		target_link_libraries(tmnt PRIVATE ${TMNT_WINDOW_PROGRAM} tmnt_extract ${TMNT_SDL2})
		set_target_properties(tmnt PROPERTIES OUTPUT_NAME "TMNT Arcade")
		add_custom_command(TARGET tmnt POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/ports/sdl2/font/font.bmp $<TARGET_FILE_DIR:tmnt>)
		if(WIN32 AND NOT TMNT_STATIC_SDL2)
			add_custom_command(TARGET tmnt POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:SDL2::SDL2> $<TARGET_FILE_DIR:tmnt>)
		endif()

		# the executable's icon on Windows (tools/make_icon.py, ports/sdl2/icon): a resource, which SDL gives the
		# window too. A picture of the player's own in its place where this computer has one (TMNT_PC_ICON, else
		# local/icon_pc.png: local/ holds the player's own pictures, never committed), made an .ico here. (The
		# icon's .png and .icns are for the Linux and macOS builds, later)
		set(TMNT_PC_ICON "" CACHE FILEPATH "The executable's icon on Windows: a picture of the player's own")
		set(TMNT_PC_PICTURE ${TMNT_PC_ICON})
		if(NOT TMNT_PC_PICTURE AND EXISTS ${CMAKE_SOURCE_DIR}/local/icon_pc.png)
			set(TMNT_PC_PICTURE ${CMAKE_SOURCE_DIR}/local/icon_pc.png)
		endif()
		set(TMNT_ICON_RC ${CMAKE_SOURCE_DIR}/ports/sdl2/icon/tmnt.rc)
		if(WIN32 AND TMNT_PC_PICTURE AND Python3_FOUND)
			# (made as CMake configures: again when the picture changes. A name of its own, so its compiled resource
			# is not the project's icon's: going from one to the other links the other)
			set(TMNT_OWN_ICON ${CMAKE_BINARY_DIR}/icon)
			execute_process(COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/make_icon.py --ico
				${TMNT_PC_PICTURE} ${TMNT_OWN_ICON}/own_icon.ico RESULT_VARIABLE TMNT_OWN_ICON_MADE)
			set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${TMNT_PC_PICTURE})
			if(TMNT_OWN_ICON_MADE EQUAL 0)
				file(WRITE ${TMNT_OWN_ICON}/own_icon.rc
					"// the player's own picture (CMakeLists.txt)\n1 ICON \"own_icon.ico\"\n")
				set(TMNT_ICON_RC ${TMNT_OWN_ICON}/own_icon.rc)
				message(STATUS "The executable's icon: ${TMNT_PC_PICTURE}")
			else()
				message(WARNING "${TMNT_PC_PICTURE} could not be made an icon (tools/make_icon.py): the project's is used")
			endif()
		endif()
		if(WIN32)
			target_sources(tmnt PRIVATE ${TMNT_ICON_RC})
		endif()

		# the console version beside it, "TMNT" its executable and its window (console/PLAN.md): the same window
		# port with the console's menus, its settings in tmnt.ini and its controls in controls.ini beside it
		# (ports/sdl2/controls.c), OPTIONS > CONTROLS drawing the keys and buttons with Kenney's Input Prompts Pixel
		# (CC0, ports/sdl2/prompts), a picture beside the executable
		if(TARGET tmnt_console_game)
			add_executable(tmnt_console WIN32 ports/sdl2/main.c ports/sdl2/first_run.c ports/sdl2/letters.c
				ports/sdl2/controls.c)
			target_link_libraries(tmnt_console PRIVATE tmnt_console_game tmnt_extract ${TMNT_SDL2})
			set_target_properties(tmnt_console PROPERTIES OUTPUT_NAME "TMNT")
			add_custom_command(TARGET tmnt_console POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/ports/sdl2/font/font.bmp
					${CMAKE_SOURCE_DIR}/ports/sdl2/prompts/input_prompts.bmp $<TARGET_FILE_DIR:tmnt_console>)
			if(WIN32 AND NOT TMNT_STATIC_SDL2)
				add_custom_command(TARGET tmnt_console POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:SDL2::SDL2> $<TARGET_FILE_DIR:tmnt_console>)
			endif()
			if(WIN32)
				target_sources(tmnt_console PRIVATE ${TMNT_ICON_RC})
			endif()
		endif()

		# The Windows package (tmnt_package): what a player downloads, build/package/tmnt-windows-x64.zip, a folder
		# tmnt with TMNT Arcade.exe and TMNT.exe, the console version (SDL2 and the C runtime inside them), and the
		# font.bmp they write with, the keys' and buttons' pictures of TMNT's CONTROLS (input_prompts.bmp), the package's README.txt (docs/package), the project's LICENSE.txt, and
		# licenses/ with the terms of what is in it or what it follows: MAME's and ymfm's (the board's video and
		# sound), SDL2's and Kenney's. Nothing of the ROM set: the game takes its assets out of the player's on its
		# first run
		if(WIN32 AND TMNT_STATIC_SDL2 AND TARGET tmnt_console)
			set(TMNT_PACKAGE_DIR ${CMAKE_SOURCE_DIR}/build/package)
			set(TMNT_PACKAGE_NAME tmnt-windows-x64)
			set(TMNT_PACKED ${TMNT_PACKAGE_DIR}/tmnt)
			file(MAKE_DIRECTORY ${TMNT_PACKAGE_DIR})
			add_custom_target(tmnt_package
				COMMAND ${CMAKE_COMMAND} -E rm -rf ${TMNT_PACKED} ${TMNT_PACKAGE_DIR}/${TMNT_PACKAGE_NAME}.zip
				COMMAND ${CMAKE_COMMAND} -E make_directory ${TMNT_PACKED}/licenses
				COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:tmnt> $<TARGET_FILE:tmnt_console>
					${CMAKE_SOURCE_DIR}/ports/sdl2/font/font.bmp ${CMAKE_SOURCE_DIR}/ports/sdl2/prompts/input_prompts.bmp
					${TMNT_PACKED}
				COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/docs/package/README.txt ${TMNT_PACKED}/README.txt
				COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/LICENSE ${TMNT_PACKED}/LICENSE.txt
				COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/docs/package/licenses/MAME.txt
					${TMNT_PACKED}/licenses/MAME.txt
				COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/tools/core/konami/reference/ymfm/LICENSE
					${TMNT_PACKED}/licenses/ymfm.txt
				COMMAND ${CMAKE_COMMAND} -E copy ${sdl2_SOURCE_DIR}/LICENSE.txt ${TMNT_PACKED}/licenses/SDL2.txt
				COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/ports/sdl2/font/License.txt
					${TMNT_PACKED}/licenses/Kenney-Mini.txt
				COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/ports/sdl2/prompts/License.txt
					${TMNT_PACKED}/licenses/Kenney-Input-Prompts.txt
				COMMAND ${CMAKE_COMMAND} -E tar cf ${TMNT_PACKAGE_NAME}.zip --format=zip tmnt
				WORKING_DIRECTORY ${TMNT_PACKAGE_DIR}
				DEPENDS tmnt tmnt_console
				COMMENT "Packing TMNT Arcade and TMNT: build/package/${TMNT_PACKAGE_NAME}.zip"
				VERBATIM)
		endif()
	else()
		message(STATUS "No SDL2: the window port (TMNT Arcade) is not built")
	endif()
endif()

# The reference tools: the original program on MAME's cores (tools/core/konami/reference)
option(TMNT_REFERENCE_TOOLS "Build the MAME-based reference tools" ON)
if(TMNT_REFERENCE_TOOLS)
	if(NOT Python3_FOUND)
		message(FATAL_ERROR "The reference tools need Python 3 (MAME's 68000 core is written by m68000gen.py)")
	endif()
	set(K_GAME_RUNTIME tmnt_rom_set)
	include(${CMAKE_SOURCE_DIR}/tools/core/konami/reference/reference.cmake)

	# the first run's check (ports/headless/assets_check.c): the assets the C takes out of a ROM set compared file
	# by file with extract_assets.py's. A development tool, as the reference: the game's executables do not carry it
	add_executable(tmnt_assets_check ports/headless/assets_check.c)
	target_link_libraries(tmnt_assets_check PRIVATE tmnt_extract)
endif()
