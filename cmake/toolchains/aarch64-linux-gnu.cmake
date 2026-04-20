# Cross-compilation toolchain for Raspberry Pi 3B+ (aarch64) running Ubuntu 24.04
# Requires: sudo apt install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu binutils-aarch64-linux-gnu

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(CMAKE_C_COMPILER   aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
set(CMAKE_STRIP        aarch64-linux-gnu-strip)

# Do not search host paths when resolving target libraries/headers/packages.
# (CMake_FIND_ROOT_PATH_MODE_PACKAGE is already set to ONLY in CMakeLists.txt,
#  which vcpkg will honour via CMAKE_FIND_ROOT_PATH pointing at the target sysroot.)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
