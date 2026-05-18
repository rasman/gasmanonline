// GasManWeb.cpp — cross-platform test harness for the GasMan shared library.
//
// Usage:
//   gasman_web [input.json] [startSec] [endSec] [everySec]
//
// Defaults: data.json  0  60  5
//
// The program:
//   1. Reads the JSON input file.
//   2. Locates gasmanAPI.dll / gasmanAPI.so / gasmanAPI.dylib next to the executable,
//      falling back to the current working directory.
//   3. Loads the library and resolves GasManJsonToCsv.
//   4. Calls the function and prints the CSV result to stdout.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <cstdlib>
#include <stdexcept>

// ---------------------------------------------------------------------------
// Platform abstraction for dynamic library loading
// ---------------------------------------------------------------------------
#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  define NOMINMAX
#  include <windows.h>
#elif defined(__APPLE__)
#  include <mach-o/dyld.h>
#  include <dlfcn.h>
#else
#  include <dlfcn.h>
#endif

#ifdef _WIN32

   using LibHandle = HMODULE;

   static LibHandle lib_open(const std::string& path)
   {
       return LoadLibraryA(path.c_str());
   }

   static void* lib_sym(LibHandle h, const char* name)
   {
       return reinterpret_cast<void*>(GetProcAddress(h, name));
   }

   static void lib_close(LibHandle h)
   {
       FreeLibrary(h);
   }

   static std::string lib_error()
   {
       DWORD err = GetLastError();
       char buf[256] = {};
       FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                      nullptr, err, 0, buf, sizeof(buf), nullptr);
       return std::string(buf);
   }

   static constexpr const char* LIB_NAME = "gasmanAPI.dll";

#else   // POSIX (Linux / macOS)

   using LibHandle = void*;

   static LibHandle lib_open(const std::string& path)
   {
       return dlopen(path.c_str(), RTLD_LAZY | RTLD_LOCAL);
   }

   static void* lib_sym(LibHandle h, const char* name)
   {
       return dlsym(h, name);
   }

   static void lib_close(LibHandle h)
   {
       dlclose(h);
   }

   static std::string lib_error()
   {
       const char* e = dlerror();
       return e ? std::string(e) : std::string("unknown error");
   }

#  ifdef __APPLE__
   static constexpr const char* LIB_NAME = "gasmanAPI.dylib";
#  else
   static constexpr const char* LIB_NAME = "gasmanAPI.so";
#  endif

#endif  // _WIN32

// ---------------------------------------------------------------------------
// GasManJsonToCsv function-pointer type
// ---------------------------------------------------------------------------
using GasManJsonToCsvFunc =
    const char* (*)(const char* jsonStr, int len,
                    int startSecond, int endSecond, int everySeconds);

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

/// Return the directory that contains this executable (best effort).
static std::filesystem::path executable_dir()
{
    // std::filesystem::current_path() is the working directory, not the
    // executable directory.  We use platform APIs where available.
#ifdef _WIN32
    char buf[MAX_PATH] = {};
    GetModuleFileNameA(nullptr, buf, MAX_PATH);
    return std::filesystem::path(buf).parent_path();
#elif defined(__APPLE__)
    uint32_t sz = 0;
    _NSGetExecutablePath(nullptr, &sz);
    std::string buf(sz, '\0');
    _NSGetExecutablePath(buf.data(), &sz);
    return std::filesystem::path(buf).parent_path();
#else
    // Linux: read /proc/self/exe
    std::error_code ec;
    auto p = std::filesystem::read_symlink("/proc/self/exe", ec);
    if (!ec) return p.parent_path();
    return std::filesystem::current_path();
#endif
}

/// Try to load the shared library.  Search order:
///   1. Directory containing the executable.
///   2. Current working directory.
static LibHandle find_and_load_library()
{
    std::vector<std::filesystem::path> candidates = {
        executable_dir() / LIB_NAME,
        std::filesystem::current_path() / LIB_NAME,
    };

    for (const auto& p : candidates) {
        if (std::filesystem::exists(p)) {
            LibHandle h = lib_open(p.string());
            if (h) return h;
        }
    }

    // Last resort: let the OS loader search its default paths.
    return lib_open(LIB_NAME);
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    // Parse command-line arguments.
    std::string jsonPath = "data.json";
    int startSec   = 0;
    int endSec     = 60;
    int everySec   = 5;

    if (argc > 1) jsonPath  = argv[1];
    if (argc > 2) startSec  = std::atoi(argv[2]);
    if (argc > 3) endSec    = std::atoi(argv[3]);
    if (argc > 4) everySec  = std::atoi(argv[4]);

    if (argc > 5) {
        std::cerr << "Usage: gasman_web [input.json] [startSec] [endSec] [everySec]\n";
        return 1;
    }

    // (1) Read JSON input.
    std::ifstream jsonFile(jsonPath);
    if (!jsonFile) {
        std::cerr << "Error: Unable to open '" << jsonPath << "'\n";
        return 1;
    }
    std::ostringstream oss;
    oss << jsonFile.rdbuf();
    std::string jsonStr = oss.str();
    int jsonLen = static_cast<int>(jsonStr.size());

    // (2) Load the shared library.
    LibHandle hLib = find_and_load_library();
    if (!hLib) {
        std::cerr << "Error: Failed to load " << LIB_NAME << ": "
                  << lib_error() << "\n";
        return 1;
    }

    // (3) Resolve the exported function.
    auto fnPtr = reinterpret_cast<GasManJsonToCsvFunc>(
        lib_sym(hLib, "GasManJsonToCsv"));
    if (!fnPtr) {
        std::cerr << "Error: Failed to locate GasManJsonToCsv: "
                  << lib_error() << "\n";
        lib_close(hLib);
        return 1;
    }

    // (4) Call the simulation.
    const char* resultCsv =
        fnPtr(jsonStr.c_str(), jsonLen, startSec, endSec, everySec);

    // (5) Print result.
    std::cout << resultCsv << '\n';

    lib_close(hLib);
    return 0;
}
