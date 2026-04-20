# vcpkg triplet: arm64-linux-static
# Statically links all vcpkg-managed libraries (spdlog, fmt, ncurses, gtest),
# while the C/C++ runtime (glibc, libstdc++) remains dynamically linked —
# both are present on every Ubuntu 24.04 installation.

set(VCPKG_TARGET_ARCHITECTURE arm64)
set(VCPKG_CRT_LINKAGE          dynamic)   # glibc / libstdc++ stay dynamic
set(VCPKG_LIBRARY_LINKAGE      static)    # vcpkg packages built as .a archives
set(VCPKG_CMAKE_SYSTEM_NAME    Linux)

# Tell vcpkg to use the cross-compiler when building packages for this triplet.
# Without this, vcpkg uses the host (x86-64) compiler and produces x86-64 libs
# in the arm64 install tree, causing find_package(Curses) and linker failures.
set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/../toolchains/aarch64-linux-gnu.cmake")
