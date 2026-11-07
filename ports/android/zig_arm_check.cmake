# Name:        zig_arm_check.cmake
# Description: CMake's toolchain for the check of Android's 32-bit build (armeabi-v7a: the Chromecast with Google TV,
#              older phones) on a computer: the headless games for 32-bit ARM Linux (musl, static, Thumb-2 with VFPv3
#              as the NDK builds armeabi-v7a), with Zig's C compiler, run under qemu-arm
# Version:     0.1.0
# Created:     07-11-2026
# Modified:    07-11-2026
# Author:      Sebastian Toyos
#
# Usage: cmake -S . -B build/arm32 -G Ninja -DCMAKE_TOOLCHAIN_FILE=ports/android/zig_arm_check.cmake
#        -DCMAKE_BUILD_TYPE=Release -DTMNT_REFERENCE_TOOLS=OFF -DTMNT_BIN_DIR=build/bin-arm32 [-DZIG=path/to/zig]
#        cmake --build build/arm32 --target tmnt_game_headless tmnt_console_headless
# then, in WSL with qemu-user-binfmt (which runs the ARM executables as they are):
#        python3 tools/check_scenarios.py --native-only --video --sound --bin build/bin-arm32 --out build/scenarios_arm32
# What it checks that the computers' builds do not: 4-byte pointers, the game's tasks on 32-bit ARM
# (runtime/src/tasks.c), char without a sign, and the alignment QEMU holds LDRD, LDM and VLDM to (as Android's
# kernels do: an unaligned one ends the game). Zig (ziglang.org) is found as in ports/anbernic/zig_aarch64.cmake.

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(TMNT_ZIG_TARGET "thumb-linux-musleabihf -mcpu=generic+v7a+vfp3d16")    # (armeabi-v7a's: ARMv7-A, Thumb-2, VFPv3-D16)

if(NOT ZIG)
	if(CMAKE_HOST_WIN32)
		file(GLOB TMNT_ZIG_UNPACKED ${CMAKE_CURRENT_LIST_DIR}/../../build/downloads/zig/*/zig.exe)
	else()
		file(GLOB TMNT_ZIG_UNPACKED ${CMAKE_CURRENT_LIST_DIR}/../../build/downloads/zig/*/zig)
	endif()
	if(TMNT_ZIG_UNPACKED)
		list(GET TMNT_ZIG_UNPACKED 0 ZIG)
	else()
		find_program(ZIG zig REQUIRED)
	endif()
endif()
set(ZIG ${ZIG} CACHE FILEPATH "Zig, the compiler for 32-bit ARM")

set(TMNT_ZIG_TOOLS ${CMAKE_BINARY_DIR}/zig_tools)
file(MAKE_DIRECTORY ${TMNT_ZIG_TOOLS})
foreach(tool cc c++ ar ranlib)
	if(tool STREQUAL "cc" OR tool STREQUAL "c++")
		set(TMNT_ZIG_ARGUMENTS "${tool} -target ${TMNT_ZIG_TARGET}")
	else()
		set(TMNT_ZIG_ARGUMENTS "${tool}")
	endif()
	if(CMAKE_HOST_WIN32)
		set(TMNT_ZIG_SCRIPT ${TMNT_ZIG_TOOLS}/zig-${tool}.cmd)
		file(WRITE ${TMNT_ZIG_SCRIPT} "@\"${ZIG}\" ${TMNT_ZIG_ARGUMENTS} %*\r\n")
	else()
		set(TMNT_ZIG_SCRIPT ${TMNT_ZIG_TOOLS}/zig-${tool})
		file(WRITE ${TMNT_ZIG_SCRIPT} "#!/bin/sh\nexec \"${ZIG}\" ${TMNT_ZIG_ARGUMENTS} \"$@\"\n")
		file(CHMOD ${TMNT_ZIG_SCRIPT} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE)
	endif()
endforeach()

set(TMNT_ZIG_SUFFIX "")
if(CMAKE_HOST_WIN32)
	set(TMNT_ZIG_SUFFIX ".cmd")
endif()
set(CMAKE_C_COMPILER ${TMNT_ZIG_TOOLS}/zig-cc${TMNT_ZIG_SUFFIX})
set(CMAKE_CXX_COMPILER ${TMNT_ZIG_TOOLS}/zig-c++${TMNT_ZIG_SUFFIX})
set(CMAKE_AR ${TMNT_ZIG_TOOLS}/zig-ar${TMNT_ZIG_SUFFIX} CACHE FILEPATH "" FORCE)
set(CMAKE_RANLIB ${TMNT_ZIG_TOOLS}/zig-ranlib${TMNT_ZIG_SUFFIX} CACHE FILEPATH "" FORCE)
