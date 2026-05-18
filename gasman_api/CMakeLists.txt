cmake_minimum_required(VERSION 3.16)
project(gasman_api VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------------------------------------------------------------------
# Output directory: all binaries (DLL/SO/dylib + executables) go to
# gasman_api/compiled/ regardless of the generator or build type.
# ---------------------------------------------------------------------------
set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/compiled")

# For single-config generators (Make, Ninja)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR}")

# For multi-config generators (Visual Studio, Xcode) every configuration
# goes to the same flat directory so callers don't need to know the config.
foreach(_cfg Debug Release RelWithDebInfo MinSizeRel)
    string(TOUPPER ${_cfg} _CFG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${_CFG} "${OUTPUT_DIR}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${_CFG} "${OUTPUT_DIR}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${_CFG} "${OUTPUT_DIR}")
endforeach()

# Make sure the output directory exists at configure time so IDEs do not warn.
file(MAKE_DIRECTORY "${OUTPUT_DIR}")

# ---------------------------------------------------------------------------
# Sub-projects
# ---------------------------------------------------------------------------
add_subdirectory(gasmanAPI)
add_subdirectory(convert)
add_subdirectory(run)
add_subdirectory(GasManWeb)

# ---------------------------------------------------------------------------
# Install: gasman.ini goes alongside every installed binary.
# ---------------------------------------------------------------------------
include(GNUInstallDirs)
install(FILES "${CMAKE_SOURCE_DIR}/gasman.ini"
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# ---------------------------------------------------------------------------
# CPack — produces a ZIP (Windows / all) or TGZ (Unix) release archive.
#
# Usage:
#   cmake --install build --prefix /tmp/gasman_install
#   cpack --config build/CPackConfig.cmake
# or just:
#   cmake --build build --target package
# ---------------------------------------------------------------------------
set(CPACK_PACKAGE_NAME        "gasman_api")
set(CPACK_PACKAGE_VERSION     "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "GasMan pharmacokinetic simulation API and tools")
set(CPACK_PACKAGE_VENDOR      "GasManWeb")

# Flat archive: all files land directly in a versioned top-level folder.
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY ON)
set(CPACK_PACKAGE_DIRECTORY  "${CMAKE_SOURCE_DIR}/dist")

if(WIN32)
    set(CPACK_GENERATOR "ZIP")
else()
    set(CPACK_GENERATOR "TGZ")
endif()

# Only package the installed components (binaries + headers + ini), not the
# build tree.
set(CPACK_INSTALLED_DIRECTORIES
    "${CMAKE_BINARY_DIR}/install" "."
)

include(CPack)
