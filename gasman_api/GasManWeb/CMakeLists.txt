cmake_minimum_required(VERSION 3.16)

# ---------------------------------------------------------------------------
# gasman_web — cross-platform test harness that loads the gasmanAPI shared lib
# at runtime and calls GasManJsonToCsv.
# ---------------------------------------------------------------------------

add_executable(gasman_web GasManWeb.cpp)

target_compile_features(gasman_web PRIVATE cxx_std_17)

# On POSIX systems we need libdl for dlopen / dlsym / dlclose.
if(NOT WIN32)
    target_link_libraries(gasman_web PRIVATE dl)
endif()

if(MSVC)
    target_compile_options(gasman_web PRIVATE /W3)
else()
    target_compile_options(gasman_web PRIVATE -Wall)
endif()

# ---------------------------------------------------------------------------
# Install rules
# ---------------------------------------------------------------------------
include(GNUInstallDirs)
install(TARGETS gasman_web RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
