# Name:        android.cmake
# Description: the Android build (the NDK's toolchain): TMNT and TMNT Arcade, each its libmain.so beside SDL2's, both
#              from their source, and the APKs around them: each game's for TMNT APK, and this computer's own
# Version:     0.1.0
# Created:     06-11-2026
# Modified:    06-11-2026
# Author:      Sebastian Toyos
#
# Included by CMakeLists.txt when the toolchain is the NDK's (build/cmake/android.toolchain.cmake, with ANDROID_ABI and
# ANDROID_PLATFORM), as outrunners-port's. Both games are the computers' builds with their picture fixed (TMNT's WIDE
# as the phone's screen is: TMNT_SCREEN_WIDE), no EXIT in TMNT's menu (TMNT_SYSTEM_EXIT: Android's home leaves it) and
# the touch screen's controls (TMNT_TOUCH, ports/sdl2/touch.c), each in a libmain.so, which SDL's activity starts (its
# Java, from SDL's source). The APKs are put together without Gradle, by the SDK's own tools: javac and d8 (the Java),
# aapt2 (the manifest, the icon, the assets), aapt (the libraries and the Java in), zipalign and apksigner. Android
# makes nothing from the ROM set: an APK carries the assets pack taken out of the player's ROM set on a computer, so it
# is its player's alone and never shared. Of each game, two:
#   build/bin/android/<game>-base.apk    the game without a pack or a signature: TMNT APK (ports/android/apk) makes the
#                                        player's APK of it, on their computer
#   build/bin/android/<Game>-debug.apk   this computer's own, with its pack (build/bin/tmnt.assets), signed with the
#                                        debug key of its Android tools (~/.android/debug.keystore, which keytool
#                                        makes when there is none; the same key each time, so a new APK installs over
#                                        the last one and keeps what the game wrote)

target_compile_definitions(tmnt_console_game PUBLIC TMNT_HANDHELD TMNT_SYSTEM_EXIT TMNT_SCREEN_WIDE TMNT_TOUCH)

set(TMNT_ANDROID_VERSION ${PROJECT_VERSION})
math(EXPR TMNT_ANDROID_VERSION_CODE    # (major * 10000 + minor * 100 + patch)
	"${PROJECT_VERSION_MAJOR} * 10000 + ${PROJECT_VERSION_MINOR} * 100 + ${PROJECT_VERSION_PATCH}")
set(TMNT_ANDROID_TARGET_SDK 35)
set(TMNT_ANDROID_MIN_SDK 24)    # (Android 7: the APK's signature is v2's alone, as TMNT APK signs it)
string(REGEX MATCH "[0-9]+" TMNT_ANDROID_PLATFORM_LEVEL "${ANDROID_PLATFORM}")
if(TMNT_ANDROID_PLATFORM_LEVEL GREATER TMNT_ANDROID_MIN_SDK)
	set(TMNT_ANDROID_MIN_SDK ${TMNT_ANDROID_PLATFORM_LEVEL})
endif()
set(TMNT_ANDROID_ICON "" CACHE FILEPATH
	"The launcher's icon: a square picture of the player's own (JPEG or PNG), instead of the project's")
set(TMNT_ANDROID_KEYSTORE "" CACHE FILEPATH "The key the APKs are signed with (by default the debug one)")
if(NOT TMNT_ANDROID_ICON AND EXISTS ${CMAKE_SOURCE_DIR}/local/icon_android.png)
	set(TMNT_ANDROID_ICON ${CMAKE_SOURCE_DIR}/local/icon_android.png)    # (local/: the player's own pictures, never in
endif()                                                                  #  the repository)

# SDL2 from its source, as the releases' (FETCHCONTENT_SOURCE_DIR_SDL2 picks a copy already here): its library,
# libSDL2.so, and its Java
include(FetchContent)
FetchContent_Declare(SDL2 GIT_REPOSITORY https://github.com/libsdl-org/SDL.git GIT_TAG release-2.32.10 GIT_SHALLOW TRUE)
set(SDL_SHARED ON CACHE BOOL "" FORCE)
set(SDL_STATIC OFF CACHE BOOL "" FORCE)
set(SDL_TEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SDL2)

# The SDK around the NDK (sdk/ndk/<version>), else ANDROID_HOME: the platform's android.jar, the newest build-tools
if(DEFINED ENV{ANDROID_HOME})
	set(TMNT_ANDROID_SDK $ENV{ANDROID_HOME})
else()
	get_filename_component(TMNT_ANDROID_SDK ${ANDROID_NDK}/../.. ABSOLUTE)
endif()
set(TMNT_ANDROID_JAR ${TMNT_ANDROID_SDK}/platforms/android-${TMNT_ANDROID_TARGET_SDK}/android.jar)
if(NOT EXISTS ${TMNT_ANDROID_JAR})
	message(FATAL_ERROR "No android.jar of Android ${TMNT_ANDROID_TARGET_SDK} in the SDK (${TMNT_ANDROID_SDK})")
endif()
file(GLOB TMNT_BUILD_TOOLS LIST_DIRECTORIES true ${TMNT_ANDROID_SDK}/build-tools/*)
list(SORT TMNT_BUILD_TOOLS COMPARE NATURAL)
list(POP_BACK TMNT_BUILD_TOOLS TMNT_BUILD_TOOLS_DIR)
foreach(tool IN ITEMS aapt2 aapt zipalign d8 apksigner)
	string(TOUPPER ${tool} name)
	find_program(TMNT_${name} NAMES ${tool} ${tool}.bat HINTS ${TMNT_BUILD_TOOLS_DIR} NO_DEFAULT_PATH REQUIRED)
endforeach()

# Java 17 (JAVA_HOME): javac and jar for SDL's Java, keytool for the debug key; d8 and apksigner run with it
find_package(Java 17 COMPONENTS Development REQUIRED)
get_filename_component(TMNT_JAVA_BIN ${Java_JAVAC_EXECUTABLE} REALPATH)
get_filename_component(TMNT_JAVA_BIN ${TMNT_JAVA_BIN} DIRECTORY)
get_filename_component(TMNT_JAVA_HOME ${TMNT_JAVA_BIN} DIRECTORY)
find_program(TMNT_KEYTOOL keytool HINTS ${TMNT_JAVA_BIN} REQUIRED)
set(TMNT_WITH_JAVA ${CMAKE_COMMAND} -E env JAVA_HOME=${TMNT_JAVA_HOME})

set(TMNT_APK_WORK ${CMAKE_BINARY_DIR}/apk)

# SDL's Java, the activity that starts libmain.so: classes.dex, the same in both games
file(GLOB TMNT_SDL_JAVA ${sdl2_SOURCE_DIR}/android-project/app/src/main/java/org/libsdl/app/*.java)
add_custom_command(OUTPUT ${TMNT_APK_WORK}/dex/classes.dex
	COMMAND ${CMAKE_COMMAND} -E rm -rf ${TMNT_APK_WORK}/classes ${TMNT_APK_WORK}/dex
	COMMAND ${CMAKE_COMMAND} -E make_directory ${TMNT_APK_WORK}/classes ${TMNT_APK_WORK}/dex
	COMMAND ${Java_JAVAC_EXECUTABLE} -source 8 -target 8 -Xlint:-options -nowarn -encoding UTF-8
		-bootclasspath ${TMNT_ANDROID_JAR} -classpath ${TMNT_BUILD_TOOLS_DIR}/core-lambda-stubs.jar
		-d ${TMNT_APK_WORK}/classes ${TMNT_SDL_JAVA}    # (the stubs: the lambdas' LambdaMetafactory)
	COMMAND ${Java_JAR_EXECUTABLE} cf ${TMNT_APK_WORK}/classes.jar -C ${TMNT_APK_WORK}/classes .
	COMMAND ${TMNT_WITH_JAVA} ${TMNT_D8} --release --min-api ${TMNT_ANDROID_MIN_SDK} --lib ${TMNT_ANDROID_JAR}
		--output ${TMNT_APK_WORK}/dex ${TMNT_APK_WORK}/classes.jar
	DEPENDS ${TMNT_SDL_JAVA}
	COMMENT "SDL's Java: classes.dex"
	VERBATIM)

file(GLOB_RECURSE TMNT_ANDROID_RES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/ports/android/res/*)
set(TMNT_APK_LIB lib/${ANDROID_ABI})

# The key this computer's APKs are signed with: the debug key of its Android tools (made once)
if(TMNT_ANDROID_KEYSTORE)
	set(TMNT_KEYSTORE ${TMNT_ANDROID_KEYSTORE})
elseif(CMAKE_HOST_WIN32)
	file(TO_CMAKE_PATH "$ENV{USERPROFILE}/.android/debug.keystore" TMNT_KEYSTORE)
else()
	set(TMNT_KEYSTORE $ENV{HOME}/.android/debug.keystore)
endif()
get_filename_component(TMNT_KEYSTORE_DIR ${TMNT_KEYSTORE} DIRECTORY)
set(TMNT_KEYSTORE_MADE "")
if(NOT EXISTS ${TMNT_KEYSTORE})    # (one this computer's Android tools made already is kept as it is)
	add_custom_command(OUTPUT ${TMNT_KEYSTORE}
		COMMAND ${CMAKE_COMMAND} -E make_directory ${TMNT_KEYSTORE_DIR}
		COMMAND ${TMNT_KEYTOOL} -genkeypair -keystore ${TMNT_KEYSTORE} -storepass android -alias androiddebugkey
			-keypass android -keyalg RSA -keysize 2048 -validity 10000 -dname "CN=Android Debug,O=Android,C=US"
		COMMENT "The debug key: ${TMNT_KEYSTORE}"
		VERBATIM)
	set(TMNT_KEYSTORE_MADE ${TMNT_KEYSTORE})
endif()

set(TMNT_ANDROID_PACK ${TMNT_BIN_DIR}/tmnt.assets)
if(NOT EXISTS ${TMNT_ANDROID_PACK})
	message(STATUS "No assets pack at ${TMNT_ANDROID_PACK}: this computer's APKs get the games alone")
endif()
set(TMNT_ANDROID_OUTPUTS "")

# An APK's manifest, icon and assets, as aapt2 links them from the folder 'stage' (its res/ and assets/, which
# 'commands' fill), then the game's files (in 'files') in and aligned: 'apk'
function(tmnt_apk apk stage manifest files commands depends)
	add_custom_command(OUTPUT ${apk}
		COMMAND ${CMAKE_COMMAND} -E rm -rf ${stage}
		COMMAND ${CMAKE_COMMAND} -E make_directory ${stage}/assets
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/ports/android/res ${stage}/res
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/ports/sdl2/font/font.bmp ${stage}/assets
		${commands}
		COMMAND ${TMNT_AAPT2} compile --dir ${stage}/res -o ${stage}/res.zip
		COMMAND ${TMNT_AAPT2} link -o ${stage}/linked.apk -I ${TMNT_ANDROID_JAR} --manifest ${manifest}
			--min-sdk-version ${TMNT_ANDROID_MIN_SDK} --target-sdk-version ${TMNT_ANDROID_TARGET_SDK}
			--version-code ${TMNT_ANDROID_VERSION_CODE} --version-name ${TMNT_ANDROID_VERSION}
			-A ${stage}/assets -0 assets ${stage}/res.zip    # (the pack stored: read in place, never unpacked)
		COMMAND ${CMAKE_COMMAND} -E chdir ${files} ${TMNT_AAPT} add ${stage}/linked.apk classes.dex
			${TMNT_APK_LIB}/libmain.so ${TMNT_APK_LIB}/libSDL2.so
		COMMAND ${TMNT_ZIPALIGN} -p -f 4 ${stage}/linked.apk ${apk}
		DEPENDS ${manifest} ${TMNT_ANDROID_RES} ${CMAKE_SOURCE_DIR}/ports/sdl2/font/font.bmp ${files}/classes.dex
			${files}/${TMNT_APK_LIB}/libmain.so ${files}/${TMNT_APK_LIB}/libSDL2.so ${depends}
		COMMENT "The APK: ${apk}"
		VERBATIM)
endfunction()

# A game of Android: its libmain.so ('target', the window port's main.c and 'sources' besides), its manifest
# ('package', 'label'), its base APK (build/bin/android/'base'-base.apk) and this computer's ('debug'-debug.apk)
function(tmnt_android_game target program base debug package label)
	add_library(${target} SHARED ports/sdl2/main.c ports/sdl2/letters.c ports/sdl2/touch.c ${ARGN})
	target_link_libraries(${target} PRIVATE ${program} tmnt_controller_db SDL2::SDL2)
	target_compile_definitions(${target} PRIVATE TMNT_HANDHELD TMNT_SYSTEM_EXIT TMNT_TOUCH)
	target_include_directories(${target} PRIVATE ports/sdl2)
	set_target_properties(${target} PROPERTIES OUTPUT_NAME main LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${target})

	set(TMNT_ANDROID_PACKAGE ${package})
	set(TMNT_ANDROID_LABEL ${label})
	set(manifest ${TMNT_APK_WORK}/${base}/AndroidManifest.xml)
	configure_file(${CMAKE_SOURCE_DIR}/ports/android/AndroidManifest.xml.in ${manifest} @ONLY)

	# the game's files in its APKs: the libraries (without their symbols) and the Java, named as the APK has them
	set(files ${TMNT_APK_WORK}/${base}/files)
	add_custom_command(OUTPUT ${files}/classes.dex ${files}/${TMNT_APK_LIB}/libmain.so ${files}/${TMNT_APK_LIB}/libSDL2.so
		COMMAND ${CMAKE_COMMAND} -E rm -rf ${files}
		COMMAND ${CMAKE_COMMAND} -E make_directory ${files}/${TMNT_APK_LIB}
		COMMAND ${CMAKE_STRIP} --strip-unneeded -o ${files}/${TMNT_APK_LIB}/libmain.so $<TARGET_FILE:${target}>
		COMMAND ${CMAKE_STRIP} --strip-unneeded -o ${files}/${TMNT_APK_LIB}/libSDL2.so $<TARGET_FILE:SDL2::SDL2>
		COMMAND ${CMAKE_COMMAND} -E copy ${TMNT_APK_WORK}/dex/classes.dex ${files}
		DEPENDS ${target} SDL2 ${TMNT_APK_WORK}/dex/classes.dex
		COMMENT "${label}'s libraries and Java"
		VERBATIM)

	# its base APK, for TMNT APK (ports/android/apk), which puts the player's assets pack in it (and their icon,
	# ic_launcher_own.xml and the picture in ic_launcher_art's place), and signs it: no pack, no signature
	set(base_apk ${TMNT_BIN_DIR}/android/${base}-base.apk)
	tmnt_apk(${base_apk} ${TMNT_APK_WORK}/${base}/base ${manifest} ${files} "" "")

	# this computer's own: its assets pack, its icon (TMNT_ANDROID_ICON, as TMNT APK puts it) and the debug key
	set(stage ${TMNT_APK_WORK}/${base}/player)
	set(extra "")
	set(extra_files "")
	if(EXISTS ${TMNT_ANDROID_PACK})
		list(APPEND extra COMMAND ${CMAKE_COMMAND} -E copy ${TMNT_ANDROID_PACK} ${stage}/assets)
		list(APPEND extra_files ${TMNT_ANDROID_PACK})
	endif()
	if(TMNT_ANDROID_ICON)
		get_filename_component(type ${TMNT_ANDROID_ICON} LAST_EXT)
		string(TOLOWER ${type} type)
		set(mipmap ${stage}/res/mipmap-xxxhdpi)
		list(APPEND extra
			COMMAND ${CMAKE_COMMAND} -E rm -f ${mipmap}/ic_launcher.png ${mipmap}/ic_launcher_art.png
			COMMAND ${CMAKE_COMMAND} -E copy ${TMNT_ANDROID_ICON} ${mipmap}/ic_launcher${type}
			COMMAND ${CMAKE_COMMAND} -E copy ${TMNT_ANDROID_ICON} ${mipmap}/ic_launcher_art${type}
			COMMAND ${CMAKE_COMMAND} -E copy ${stage}/res/mipmap-anydpi-v26/ic_launcher_own.xml
				${stage}/res/mipmap-anydpi-v26/ic_launcher.xml)
		list(APPEND extra_files ${TMNT_ANDROID_ICON})
	endif()
	tmnt_apk(${TMNT_APK_WORK}/${base}/player.apk ${stage} ${manifest} ${files} "${extra}" "${extra_files}")
	set(debug_apk ${TMNT_BIN_DIR}/android/${debug}-debug.apk)    # (not TMNT APK's own)
	add_custom_command(OUTPUT ${debug_apk}
		COMMAND ${TMNT_WITH_JAVA} ${TMNT_APKSIGNER} sign --ks ${TMNT_KEYSTORE} --ks-pass pass:android
			--ks-key-alias androiddebugkey --key-pass pass:android --out ${debug_apk} ${TMNT_APK_WORK}/${base}/player.apk
		DEPENDS ${TMNT_APK_WORK}/${base}/player.apk ${TMNT_KEYSTORE_MADE}
		COMMENT "This computer's ${label}, signed: ${debug_apk}"
		VERBATIM)
	set(TMNT_ANDROID_OUTPUTS ${TMNT_ANDROID_OUTPUTS} ${base_apk} ${debug_apk} PARENT_SCOPE)
endfunction()

tmnt_android_game(tmnt_android tmnt_console_game tmnt TMNT io.github.cheviman.tmnt "TMNT" ports/sdl2/controls.c)
tmnt_android_game(tmnt_arcade_android ${TMNT_WINDOW_PROGRAM} tmnt-arcade TMNT-Arcade io.github.cheviman.tmnt.arcade
	"TMNT Arcade")
add_custom_target(tmnt_apks ALL DEPENDS ${TMNT_ANDROID_OUTPUTS})
