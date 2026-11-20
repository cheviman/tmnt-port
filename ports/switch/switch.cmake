# Name:        switch.cmake
# Description: the Nintendo Switch build (devkitPro's toolchain): TMNT and TMNT Arcade, each as the pieces of its
#              .nsp (the NSO, main.npdm, the NACP and the icon) and as an .nro (with the assets pack where this
#              build has it)
# Version:     0.1.0
# Created:     06-11-2026
# Modified:    19-11-2026
# Author:      Sebastian Toyos
#
# Included by CMakeLists.txt when the toolchain is the Switch's (aarch64-none-elf-cmake), as outrunners-port's. Both
# games are the computers' builds with SDL 3 for the Switch (below: devkitPro's own branch of it): the picture fixed and
# wide (TMNT_HANDHELD, TMNT_SCREEN_WIDE: 16:9), their controls fixed (each controller a player, up to four, the
# cabinet's four slots), and no EXIT in TMNT's menu (TMNT_SYSTEM_EXIT: HOME leaves it). A game reads the assets pack
# (tmnt.assets) from its own romfs, which a computer fills with the player's (the .nsp, made there with the player's
# keys by TMNT NSP; or, where this build has the pack beside the executables, build/bin/tmnt.assets, the .nro: the
# player's own, never shared); with none there, from sdmc:/switch/tmnt/, where it takes the assets out of the
# player's tmnt.zip at its first start (main.c's device_assets: the release's .nro, tmnt-switch.zip, carries none).
# Each game its own title, in homebrew's range

target_compile_definitions(tmnt_console_game PUBLIC TMNT_HANDHELD TMNT_SCREEN_WIDE TMNT_SYSTEM_EXIT)

# SDL 3 for the Switch (Sebas, 19-11-2026: "Probar SDL3 en Switch"). SDL's own Switch port is for those under
# Nintendo's NDA (its docs/README-switch.md), and devkitPro has no package of SDL 3 yet (its portlibs have switch-sdl2
# alone): SDL is built here from devkitPro's own branch of it (github.com/devkitPro/SDL, switch-sdl-3.4: SDL 3.4.0 with
# libnx's video, sound and controllers; not released, its maintainer: "a couple of tests failing"), the commit below
# by its archive's hash (the one in build/downloads/sdl3-switch-archive where it is there), into the games. A package
# of devkitPro's (switch-sdl3) is taken instead once there is one
find_package(SDL3 CONFIG QUIET)
if(TARGET SDL3::SDL3)
	set(TMNT_SWITCH_SDL SDL3::SDL3)
else()
	set(TMNT_SWITCH_SDL_COMMIT c329016c0e338ab4397ad930f83330c9fe058348)
	set(TMNT_SWITCH_SDL_SHA256 6047e4787f105ab4a0c6293ee3090ce5d1ebad112fcbe2a3e5a8874b0cc6f709)
	set(TMNT_SWITCH_SDL_URL https://github.com/devkitPro/SDL/archive/${TMNT_SWITCH_SDL_COMMIT}.tar.gz)
	if(EXISTS ${CMAKE_SOURCE_DIR}/build/downloads/sdl3-switch-archive/${TMNT_SWITCH_SDL_COMMIT}.tar.gz)
		set(TMNT_SWITCH_SDL_URL
			${CMAKE_SOURCE_DIR}/build/downloads/sdl3-switch-archive/${TMNT_SWITCH_SDL_COMMIT}.tar.gz)
	endif()
	include(FetchContent)
	FetchContent_Declare(SDL3 URL ${TMNT_SWITCH_SDL_URL} URL_HASH SHA256=${TMNT_SWITCH_SDL_SHA256})
	set(SDL_SHARED OFF CACHE BOOL "" FORCE)
	set(SDL_STATIC ON CACHE BOOL "" FORCE)
	set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
	# On Windows devkitPro's CMake is MSYS2's, which writes SDL's precompiled header with a /d/... path in it, and
	# devkitPro's compiler, a Windows program, does not find it. There SDL's sources get SDL_internal.h included
	# first by the compiler's own option instead (the Switch's files of SDL count on it: without it they are empty)
	if(CMAKE_HOST_SYSTEM_NAME MATCHES "MSYS|CYGWIN|MINGW")
		set(CMAKE_DISABLE_PRECOMPILE_HEADERS ON)
	endif()
	FetchContent_MakeAvailable(SDL3)
	if(CMAKE_DISABLE_PRECOMPILE_HEADERS)
		target_compile_options(SDL3-static PRIVATE -include SDL_internal.h)
	endif()
	set(TMNT_SWITCH_SDL SDL3::SDL3-static)
endif()

# devkitPro's tools: the NACP (the title's name, author and version), the NSO, main.npdm and the .nro
set(TMNT_SWITCH_TOOLS $ENV{DEVKITPRO}/tools/bin)
find_program(TMNT_NACPTOOL nacptool HINTS ${TMNT_SWITCH_TOOLS} REQUIRED)
find_program(TMNT_ELF2NSO elf2nso HINTS ${TMNT_SWITCH_TOOLS} REQUIRED)
find_program(TMNT_NPDMTOOL npdmtool HINTS ${TMNT_SWITCH_TOOLS} REQUIRED)
find_program(TMNT_ELF2NRO elf2nro HINTS ${TMNT_SWITCH_TOOLS} REQUIRED)

# the HOME menu's icon, 256 x 256: the player's own where this computer has one (local/icon.jpg: local/ holds the
# player's own pictures, never in the repository), else the project's (ports/switch/icon.jpg, from
# ports/sdl/icon/tmnt.png); TMNT NSP takes a player's own instead
if(EXISTS ${CMAKE_SOURCE_DIR}/local/icon.jpg)
	set(TMNT_SWITCH_ICON ${CMAKE_SOURCE_DIR}/local/icon.jpg)
else()
	set(TMNT_SWITCH_ICON ${CMAKE_SOURCE_DIR}/ports/switch/icon.jpg)
endif()
set(TMNT_SWITCH_PACK ${TMNT_BIN_DIR}/tmnt.assets)
set(TMNT_SWITCH_OUTPUTS "")

# A game of the Switch: its executable (the window port's main.c, 'sources' besides), and in build/bin/switch/'folder'
# its .nsp's pieces, which the computer's TMNT NSP puts together with the player's assets and keys; its .nro where
# this build has the pack
function(tmnt_switch_game target name folder title_id program)
	add_executable(${target} ports/sdl/main.c ${ARGN})
	target_link_libraries(${target} PRIVATE ${program} tmnt_extract ${TMNT_SWITCH_SDL})
	target_compile_definitions(${target} PRIVATE TMNT_HANDHELD TMNT_SCREEN_WIDE TMNT_SYSTEM_EXIT)
	set_target_properties(${target} PROPERTIES SUFFIX ".elf")

	set(out ${TMNT_BIN_DIR}/switch/${folder})
	set(TMNT_SWITCH_NAME ${name})
	set(TMNT_SWITCH_TITLE_ID ${title_id})
	configure_file(${CMAKE_SOURCE_DIR}/ports/switch/npdm.json.in ${CMAKE_BINARY_DIR}/switch/${folder}.json @ONLY)
	add_custom_command(OUTPUT ${out}/main ${out}/main.npdm ${out}/control.nacp ${out}/icon.jpg
		COMMAND ${CMAKE_COMMAND} -E make_directory ${out}
		COMMAND ${TMNT_NACPTOOL} --create ${name} "Sebastian Toyos" ${PROJECT_VERSION} ${out}/control.nacp
			--titleid=${title_id}
		COMMAND ${TMNT_ELF2NSO} $<TARGET_FILE:${target}> ${out}/main
		COMMAND ${TMNT_NPDMTOOL} ${CMAKE_BINARY_DIR}/switch/${folder}.json ${out}/main.npdm
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${TMNT_SWITCH_ICON} ${out}/icon.jpg
		DEPENDS ${target} ${TMNT_SWITCH_ICON} ${CMAKE_SOURCE_DIR}/ports/switch/npdm.json.in
		COMMENT "${name}: the .nsp's pieces (main, main.npdm, control.nacp, icon.jpg)"
		VERBATIM)
	set(outputs ${out}/main ${out}/main.npdm)
	if(NOT EXISTS ${TMNT_SWITCH_PACK})
		# the .nro without a pack (the release's: the game takes the assets out of tmnt.zip on the SD card), in
		# build/bin/switch-nro, apart from the .nsp's pieces the computers' packages carry
		set(nro ${TMNT_BIN_DIR}/switch-nro/${name}.nro)
		add_custom_command(OUTPUT ${nro}
			COMMAND ${CMAKE_COMMAND} -E make_directory ${TMNT_BIN_DIR}/switch-nro
			COMMAND ${TMNT_ELF2NRO} $<TARGET_FILE:${target}> ${nro} --icon=${TMNT_SWITCH_ICON} --nacp=${out}/control.nacp
			DEPENDS ${target} ${out}/control.nacp ${TMNT_SWITCH_ICON}
			COMMENT "${name}.nro without an assets pack"
			VERBATIM)
		list(APPEND outputs ${nro})
	else()
		set(romfs ${CMAKE_BINARY_DIR}/switch/${folder}_romfs)
		add_custom_command(OUTPUT ${out}/${folder}.nro
			COMMAND ${CMAKE_COMMAND} -E make_directory ${romfs}
			COMMAND ${CMAKE_COMMAND} -E copy_if_different ${TMNT_SWITCH_PACK} ${romfs}
			COMMAND ${TMNT_ELF2NRO} $<TARGET_FILE:${target}> ${out}/${folder}.nro --icon=${TMNT_SWITCH_ICON}
				--nacp=${out}/control.nacp --romfsdir=${romfs}
			DEPENDS ${target} ${TMNT_SWITCH_PACK} ${out}/control.nacp ${TMNT_SWITCH_ICON}
			COMMENT "${name}.nro with the assets pack in its romfs (the player's own: never shared)"
			VERBATIM)
		list(APPEND outputs ${out}/${folder}.nro)
	endif()
	set(TMNT_SWITCH_OUTPUTS ${TMNT_SWITCH_OUTPUTS} ${outputs} PARENT_SCOPE)
endfunction()

tmnt_switch_game(tmnt_switch "TMNT" tmnt 0500544d4e540000 tmnt_console_game ports/sdl/controls.c)
tmnt_switch_game(tmnt_arcade_switch "TMNT Arcade" tmnt-arcade 0500544d4e541000 ${TMNT_WINDOW_PROGRAM})
if(NOT EXISTS ${TMNT_SWITCH_PACK})
	message(STATUS "No assets pack at ${TMNT_SWITCH_PACK}: the .nro without one (build/bin/switch-nro)")
endif()
add_custom_target(tmnt_switch_games ALL DEPENDS ${TMNT_SWITCH_OUTPUTS})
