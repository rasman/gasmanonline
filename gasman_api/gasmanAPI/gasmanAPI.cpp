#include "pch.h"
#include "GasApplication.h"
#include "gasmanAPI.h"
#include <fstream>
#include <locale>
#include <mutex>
#include <string>

// ---------------------------------------------------------------------------
// Windows DLL entry point (no-op: the library has no per-attach/detach work).
// ---------------------------------------------------------------------------
#ifdef _WIN32
BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD /*ul_reason_for_call*/, LPVOID /*lpReserved*/)
{
    return TRUE;
}
#endif

// ---------------------------------------------------------------------------
// Internal state
// ---------------------------------------------------------------------------

// Serialises all GasManJsonToCsv calls so that GasApplication's global state
// (the gasApp pointer, std::locale::global()) is never touched concurrently.
static std::mutex s_callMutex;

// Per-thread result buffer — each thread holds its own copy so callers on
// different threads can read the returned pointer at the same time.
static thread_local std::string tl_result;

// Per-thread error code — set on every call, readable via GasManLastError().
static thread_local int tl_lastError = GASMAN_OK;

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

// Store an error and return a stable c_str() into the thread-local buffer.
static const char* setError(int code, std::string msg)
{
    tl_lastError = code;
    tl_result    = std::move(msg);
    return tl_result.c_str();
}

// ---------------------------------------------------------------------------
// GasManJsonToCsv
// ---------------------------------------------------------------------------
extern "C" GASMAN_API const char* GasManJsonToCsv(
    const char* jsonStr,
    int         len,
    int         startSecond,
    int         endSecond,
    int         everySeconds
)
{
    // Validate time parameters before acquiring the mutex.
    if (startSecond < 0 || everySeconds <= 0 || endSecond < startSecond)
        return setError(GASMAN_ERR_INVALID_PARAMS, "Error: impossible time parameters");

    // Serialise access to the simulation engine and global locale setting.
    std::lock_guard<std::mutex> lock(s_callMutex);

    GasApplication app;

    // Verify gasman.ini exists before initialising.
    {
        std::ifstream iniFile(app.getIniFile());
        if (!iniFile.good())
            return setError(GASMAN_ERR_INI_NOT_FOUND, "Error: gasman.ini not found");
    }

    // Apply locale from INI (or fall back to a safe default).
    std::string locale = std::locale("").name();
    app.ReadProfile("Application", "Locale", locale);
    try {
        std::locale::global(std::locale(locale));
    } catch (const std::runtime_error&) {
        std::locale::global(std::locale("en_US.UTF-8"));
    }

    if (!app.initialize())
        return setError(GASMAN_ERR_INI_READ_FAILED, "Error: unable to read INI");

    // Run the simulation.
    try {
        tl_result = GasApplication::createCSV(jsonStr, len, startSecond, endSecond, everySeconds);
    } catch (const std::exception& e) {
        return setError(GASMAN_ERR_EXCEPTION, std::string("Error: ") + e.what());
    } catch (...) {
        return setError(GASMAN_ERR_EXCEPTION, "Error: unknown exception");
    }

    // createCSV signals engine-level errors as "Error: …" strings.
    if (tl_result.size() >= 7 && tl_result.compare(0, 7, "Error: ") == 0)
        tl_lastError = GASMAN_ERR_SIMULATION;
    else
        tl_lastError = GASMAN_OK;

    return tl_result.c_str();
}

// ---------------------------------------------------------------------------
// GasManLastError
// ---------------------------------------------------------------------------
extern "C" GASMAN_API int GasManLastError(void)
{
    return tl_lastError;
}

// ---------------------------------------------------------------------------
// GasManErrorString
// ---------------------------------------------------------------------------
extern "C" GASMAN_API const char* GasManErrorString(int code)
{
    switch (code)
    {
        case GASMAN_OK:                  return "Success";
        case GASMAN_ERR_INVALID_PARAMS:  return "Invalid time parameters";
        case GASMAN_ERR_INI_NOT_FOUND:   return "gasman.ini not found";
        case GASMAN_ERR_INI_READ_FAILED: return "Failed to read gasman.ini";
        case GASMAN_ERR_JSON_PARSE:      return "JSON / XML parse error";
        case GASMAN_ERR_SIMULATION:      return "Simulation engine error";
        case GASMAN_ERR_EXCEPTION:       return "Unexpected internal exception";
        default:                         return "Unknown error";
    }
}
