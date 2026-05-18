cmake_minimum_required(VERSION 3.16)

# ---------------------------------------------------------------------------
# gasman_run — standalone simulation runner (no Python, no DLL at runtime)
#
# Links the simulation engine statically so the single executable is
# self-contained: copy it alongside gasman.ini and run.
# ---------------------------------------------------------------------------

add_executable(gasman_run
    main.cpp
    # miniz 2.2.0 ZIP implementation lives with the other bundled libraries.
    "${CMAKE_CURRENT_SOURCE_DIR}/../gasmanAPI/importLibraries/miniz.cpp"
)

target_compile_features(gasman_run PRIVATE cxx_std_17)

# Link against the static simulation library so no gasmanAPI.dll/so/dylib is
# needed at runtime — just the executable and gasman.ini.
target_link_libraries(gasman_run PRIVATE gasmanAPI_static)

# gasman_run includes headers from both gasmanAPI and convert/.
# gasmanlib_static already exposes gasmanAPI headers via PUBLIC include dirs.
target_include_directories(gasman_run PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../convert"
    # gasmanAPI headers are inherited via gasmanAPI_static PUBLIC include dirs
)

if(MSVC)
    target_compile_options(gasman_run PRIVATE /W3)
else()
    target_compile_options(gasman_run PRIVATE -Wall)

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND
       CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
        target_link_libraries(gasman_run PRIVATE stdc++fs)
    endif()
endif()

# ---------------------------------------------------------------------------
# Install rules
# ---------------------------------------------------------------------------
include(GNUInstallDirs)
install(TARGETS gasman_run RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# Copy gasman.ini next to the executable after every build.
add_custom_command(TARGET gasman_run POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/gasman.ini"
        "$<TARGET_FILE_DIR:gasman_run>/gasman.ini"
    COMMENT "Copying gasman.ini alongside gasman_run"
)
