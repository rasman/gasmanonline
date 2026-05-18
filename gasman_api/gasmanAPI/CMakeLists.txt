cmake_minimum_required(VERSION 3.16)

# ---------------------------------------------------------------------------
# gasmanAPI — simulation library
#   Shared  → gasmanAPI.dll / gasmanAPI.so / gasmanAPI.dylib
#   Static  → gasmanAPI_static.lib / libgasmanAPI_static.a
# ---------------------------------------------------------------------------

set(GASMANAPI_SOURCES
    Gas.cpp
    GasAnes.cpp
    GasApplication.cpp
    GasDoc.cpp
    GasGlobal.cpp
    GasSample.cpp
    ColorUtils.cpp
    StringUtils.cpp
    gasmanAPI.cpp
)

add_library(gasmanAPI        SHARED ${GASMANAPI_SOURCES})
add_library(gasmanAPI_static STATIC ${GASMANAPI_SOURCES})

# Suppress the automatic "lib" prefix on Linux/macOS so the file is always
# named gasmanAPI.so / gasmanAPI.dylib, matching the Windows gasmanAPI.dll.
set_target_properties(gasmanAPI PROPERTIES
    OUTPUT_NAME "gasmanAPI"
    PREFIX      ""
)
set_target_properties(gasmanAPI_static PROPERTIES
    OUTPUT_NAME "gasmanAPI_static"
)

# Shared include dirs / compile flags applied to both targets.
foreach(_tgt IN ITEMS gasmanAPI gasmanAPI_static)
    target_include_directories(${_tgt}
        PUBLIC                                        # PUBLIC so link targets inherit the path
            "${CMAKE_CURRENT_SOURCE_DIR}"
            "${CMAKE_CURRENT_SOURCE_DIR}/importLibraries"
    )

    if(MSVC)
        target_compile_options(${_tgt} PRIVATE /W3 /bigobj)
    else()
        target_compile_options(${_tgt} PRIVATE -Wall -fPIC)
        if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND
           CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
            target_link_libraries(${_tgt} PRIVATE stdc++fs)
        endif()
    endif()
endforeach()

# Tell gasmanAPI.cpp it is the DLL/library being built (enables dllexport in
# gasmanAPI.h).  Defining it for the static target too avoids a C2491 error:
# without it GASMAN_API resolves to dllimport, and you cannot *define* a
# dllimport function.  __declspec(dllexport) on a static-lib compilation is
# silently ignored at link time, so this is safe.
target_compile_definitions(gasmanAPI        PRIVATE GASMANAPI_EXPORTS)
target_compile_definitions(gasmanAPI_static PRIVATE GASMANAPI_EXPORTS)

# ---------------------------------------------------------------------------
# Install rules
# ---------------------------------------------------------------------------
include(GNUInstallDirs)

# Shared library: DLL/so/dylib → bin on Windows (runtime), lib elsewhere.
install(TARGETS gasmanAPI
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}   # .dll on Windows
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}   # .so/.dylib on Unix
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}   # .lib import lib on Windows
)

# Static library.
install(TARGETS gasmanAPI_static
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Public C API header.
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/gasmanAPI.h"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# ---------------------------------------------------------------------------
# Copy gasman.ini next to the shared library after every build so the runtime
# INI check in gasmanAPI.cpp succeeds without manual steps.
# ---------------------------------------------------------------------------
add_custom_command(TARGET gasmanAPI POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/gasman.ini"
        "$<TARGET_FILE_DIR:gasmanAPI>/gasman.ini"
    COMMENT "Copying gasman.ini to output directory"
)
