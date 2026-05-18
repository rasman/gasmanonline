cmake_minimum_required(VERSION 3.16)

# ---------------------------------------------------------------------------
# gasman_convert — standalone CSV → GasMan JSON/XML converter
# ---------------------------------------------------------------------------

add_executable(gasman_convert main.cpp)

target_compile_features(gasman_convert PRIVATE cxx_std_17)

# nlohmann/json.hpp lives in gasmanAPI/importLibraries.
target_include_directories(gasman_convert PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../gasmanAPI/importLibraries"
)

if(MSVC)
    target_compile_options(gasman_convert PRIVATE /W3)
else()
    target_compile_options(gasman_convert PRIVATE -Wall)

    # GCC < 9 needs -lstdc++fs for std::filesystem.
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
            target_link_libraries(gasman_convert PRIVATE stdc++fs)
        endif()
    endif()
endif()

# ---------------------------------------------------------------------------
# Install rules
# ---------------------------------------------------------------------------
include(GNUInstallDirs)
install(TARGETS gasman_convert RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
