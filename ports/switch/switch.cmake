# Name:        switch.cmake
# Description: the Nintendo Switch build (devkitPro's toolchain): TMNT and TMNT Arcade, each as the pieces of its
#              .nsp (the NSO, main.npdm, the NACP and the icon) and as an .nro (with the assets pack where this
#              build has it)
# Version:     0.1.0
# Created:     06-11-2026
# Modified:    09-11-2026
# Author:      Sebastian Toyos
#
# Included by CMakeLists.txt when the toolchain is the Switch's (aarch64-none-elf-cmake), as outrunners-port's. Both
# games are the computers' builds with SDL2 for the Switch (switch-sdl2, devkitPro's portlibs): the picture fixed and
# wide (TMNT_HANDHELD, TMNT_SCREEN_WIDE: 16:9), their controls fixed (each controller a player, up to four, the
# cabinet's four slots), and no EXIT in TMNT's menu (TMNT_SYSTEM_EXIT: HOME leaves it). A game reads the assets pack
# (tmnt.assets) from its own romfs, which a computer fills with the player's (the .nsp, made there with the player's
# keys by TMNT NSP; or, where this build has the pack beside the executables, build/bin/tmnt.assets, the .nro: the
# player's own, never shared); with none there, from sdmc:/switch/tmnt/, where it takes the assets out of the
# player's tmnt.zip at its first start (main.c's device_assets: the release's .nro, tmnt-switch.zip, carries none).
# Each game its own title, in homebrew's range

target_compile_definitions(tmnt_console_game PUBLIC TMNT_HANDHELD TMNT_SCREEN_WIDE TMNT_SYSTEM_EXIT)

find_package(SDL2 CONFIG QUIET)
if(TARGET SDL2::SDL2)
	set(TMNT_SWITCH_SDL2 SDL2::SDL2)
else()
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(TMNT_SWITCH_SDL2_PKG REQUIRED IMPORTED_TARGET sdl2)
	set(TMNT_SWITCH_SDL2 PkgConfig::TMNT_SWITCH_SDL2_PKG)
endif()

# devkitPro's tools: the NACP (the title's name, author and version), the NSO, main.npdm and the .nro
set(TMNT_SWITCH_TOOLS $ENV{DEVKITPRO}/tools/bin)
find_program(TMNT_NACPTOOL nacptool HINTS ${TMNT_SWITCH_TOOLS} REQUIRED)
find_program(TMNT_ELF2NSO elf2nso HINTS ${TMNT_SWITCH_TOOLS} REQUIRED)
find_program(TMNT_NPDMTOOL npdmtool HINTS ${TMNT_SWITCH_TOOLS} REQUIRED)
find_program(TMNT_ELF2NRO elf2nro HINTS ${TMNT_SWITCH_TOOLS} REQUIRED)

# the HOME menu's icon, 256 x 256: the player's own where this computer has one (local/icon.jpg: local/ holds the
# player's own pictures, never in the repository), else the project's (ports/switch/icon.jpg, from
# ports/sdl2/icon/tmnt.png); TMNT NSP takes a player's own instead
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
	add_executable(${target} ports/sdl2/main.c ${ARGN})
	target_link_libraries(${target} PRIVATE ${program} tmnt_extract ${TMNT_SWITCH_SDL2})
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

tmnt_switch_game(tmnt_switch "TMNT" tmnt 0500544d4e540000 tmnt_console_game ports/sdl2/controls.c)
tmnt_switch_game(tmnt_arcade_switch "TMNT Arcade" tmnt-arcade 0500544d4e541000 ${TMNT_WINDOW_PROGRAM})
if(NOT EXISTS ${TMNT_SWITCH_PACK})
	message(STATUS "No assets pack at ${TMNT_SWITCH_PACK}: the .nro without one (build/bin/switch-nro)")
endif()
add_custom_target(tmnt_switch_games ALL DEPENDS ${TMNT_SWITCH_OUTPUTS})
