# Name:        ios.cmake
# Description: the iOS and iPadOS build (Xcode, CMake's Xcode generator): TMNT and TMNT Arcade as apps on SDL 3's
#              UIKit backend, for an iPhone or an iPad (arm64) or the Simulator, and an unsigned .ipa of each
# Version:     0.1.0
# Created:     20-11-2026
# Modified:    20-11-2026
# Author:      Sebastian Toyos
#
# Included by CMakeLists.txt when CMAKE_SYSTEM_NAME is iOS, as simpsons-port's (Sebas, 19-11-2026: "y anota que hay que
# agregar una version de ios", and "si, al final" of this batch). Both games are Android's builds on Apple's phones and
# tablets: the screen whole (TMNT_HANDHELD: no first run's window, no VSYNC nor window lines), no EXIT in TMNT's menu
# (TMNT_SYSTEM_EXIT: the home gesture leaves it), the touch screen's controls (TMNT_TOUCH, ports/sdl/touch.c), TMNT's
# WIDE by the screen's shape (main.c's follow_screen, between the sides the notch or the Dynamic Island takes), the
# game controllers iOS knows (MFi, Xbox, PlayStation, Switch Pro: SDL's driver for Apple's GameController, with
# SDL_GameControllerDB's iOS lines). Each app takes its assets out of the player's tmnt.zip in its Documents folder at
# its first start (main.c's device_assets), which the Files app shows as the app's own (Info.plist.in:
# UIFileSharingEnabled, LSSupportsOpeningDocumentsInPlace). Two apps, two folders: each its own copy. SDL from its
# source, the computers' version (CMakeLists.txt's TMNT_SDL_VERSION), static; main.c's SDL_main.h starts UIKit. The
# icon is the project's (Assets.xcassets: Raphael's face from a friend's mural, tools/make_icon.py), the launch screen
# black (LaunchScreen.storyboard), both compiled by Xcode. The apps are not signed unless TMNT_IOS_TEAM names a
# development team (a Mac building for its own iPhone): the release's .ipa is unsigned, and the player signs it as it
# is installed (AltStore, SideStore, Sideloadly: docs/package/README-ios.txt). Never the App Store: the game is
# Konami's, and the player brings the ROM set.
#
# Usage (macOS, Xcode; for the Simulator, CMAKE_OSX_SYSROOT iphonesimulator, in a build folder of its own):
#   cmake -S . -B build/ios -G Xcode -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_ARCHITECTURES=arm64
#       -DCMAKE_OSX_DEPLOYMENT_TARGET=13.0 -DTMNT_REFERENCE_TOOLS=OFF [-DCMAKE_OSX_SYSROOT=iphonesimulator]
#       [-DTMNT_BIN_DIR=build/bin-ios] [-DTMNT_IOS_TEAM=<team ID>]
#   cmake --build build/ios --config Release --target tmnt_ios_games -- CODE_SIGNING_ALLOWED=NO
# The apps: TMNT_BIN_DIR/ios/TMNT.app and TMNT Arcade.app; for a device, also TMNT.ipa and TMNT Arcade.ipa there
# (each app inside Payload/, zipped).

if(NOT CMAKE_GENERATOR STREQUAL "Xcode")
	message(FATAL_ERROR "The iOS build is the Xcode generator's (-G Xcode): Xcode compiles the apps' launch screen and "
		"icon, and puts the bundles together")
endif()

set(TMNT_IOS_DEFINITIONS TMNT_HANDHELD TMNT_SYSTEM_EXIT TMNT_SCREEN_WIDE TMNT_TOUCH)
target_compile_definitions(tmnt_console_game PUBLIC ${TMNT_IOS_DEFINITIONS})

set(TMNT_IOS_TEAM "" CACHE STRING "The development team that signs the apps (none: unsigned, as the release's)")
set(TMNT_IOS_OUT ${TMNT_BIN_DIR}/ios)
if(CMAKE_OSX_SYSROOT MATCHES "[Ss]imulator")
	set(TMNT_IOS_SIMULATOR ON)
endif()

# SDL from its source, static (FETCHCONTENT_SOURCE_DIR_SDL3 picks a copy already here). No HIDAPI: iOS's game
# controllers come through Apple's GameController framework, and SDL's HIDAPI there talks Bluetooth itself, which
# needs a permission the apps do not ask for
include(FetchContent)
FetchContent_Declare(SDL3 URL ${TMNT_SDL_URL} URL_HASH SHA256=${TMNT_SDL_SHA256})
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
set(SDL_HIDAPI OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SDL3)

# the apps' resources, which Xcode copies into each bundle (compiling the launch screen and the asset catalog): the
# letters of the touch controls and the menus, the launch screen, the icon
set(TMNT_IOS_RESOURCES ports/sdl/font/font.bmp ports/ios/LaunchScreen.storyboard ports/ios/Assets.xcassets)
set_source_files_properties(${TMNT_IOS_RESOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

set(TMNT_IOS_OUTPUTS "")

# A game of iOS: its app ('target', named 'name', its bundle 'identifier'), the window port's main.c, letters.c and
# touch.c and the sources after 'program' besides, on 'program' (tmnt_extract: the assets out of the player's
# tmnt.zip on the device, main.c's device_assets); for a device, its .ipa beside it
function(tmnt_ios_game target name identifier program)
	add_executable(${target} MACOSX_BUNDLE ports/sdl/main.c ports/sdl/letters.c ports/sdl/touch.c ${ARGN}
		${TMNT_IOS_RESOURCES})
	target_include_directories(${target} PRIVATE ports/sdl)
	target_link_libraries(${target} PRIVATE ${program} tmnt_extract tmnt_controller_db SDL3::SDL3-static)
	target_compile_definitions(${target} PRIVATE ${TMNT_IOS_DEFINITIONS})
	set_target_properties(${target} PROPERTIES OUTPUT_NAME ${name}
		MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/ports/ios/Info.plist.in
		MACOSX_BUNDLE_BUNDLE_NAME ${name} MACOSX_BUNDLE_GUI_IDENTIFIER ${identifier}
		MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION} MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
		XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER ${identifier}
		XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2"    # (iPhone and iPad)
		XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME AppIcon)
	foreach(config IN ITEMS "" _DEBUG _RELEASE _RELWITHDEBINFO _MINSIZEREL)
		set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY${config} ${TMNT_IOS_OUT})
	endforeach()
	if(TMNT_IOS_TEAM)
		set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_DEVELOPMENT_TEAM ${TMNT_IOS_TEAM}
			XCODE_ATTRIBUTE_CODE_SIGN_STYLE Automatic XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Apple Development")
	else()
		set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED NO
			XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "")
	endif()

	set(outputs "")
	if(NOT TMNT_IOS_SIMULATOR)
		# its .ipa: the app in Payload/, zipped (ditto keeps the bundle's permissions), as a sideloading tool takes it
		set(stage ${CMAKE_BINARY_DIR}/ipa/${target})
		set(ipa ${TMNT_IOS_OUT}/${name}.ipa)
		add_custom_command(OUTPUT ${ipa}
			COMMAND ${CMAKE_COMMAND} -E rm -rf ${stage} ${ipa}
			COMMAND ${CMAKE_COMMAND} -E make_directory ${stage}/Payload
			COMMAND ditto $<TARGET_BUNDLE_DIR:${target}> ${stage}/Payload/${name}.app
			COMMAND ${CMAKE_COMMAND} -E chdir ${stage} ditto -c -k --keepParent Payload ${ipa}
			DEPENDS ${target}
			COMMENT "${name}.ipa (unsigned: the player's sideloading tool signs it)"
			VERBATIM)
		list(APPEND outputs ${ipa})
	endif()
	set(TMNT_IOS_OUTPUTS ${TMNT_IOS_OUTPUTS} ${outputs} PARENT_SCOPE)
endfunction()

tmnt_ios_game(tmnt_ios "TMNT" io.github.cheviman.tmnt tmnt_console_game ports/sdl/controls.c)
tmnt_ios_game(tmnt_arcade_ios "TMNT Arcade" io.github.cheviman.tmnt.arcade ${TMNT_WINDOW_PROGRAM})
add_custom_target(tmnt_ios_games ALL DEPENDS ${TMNT_IOS_OUTPUTS})
add_dependencies(tmnt_ios_games tmnt_ios tmnt_arcade_ios)
