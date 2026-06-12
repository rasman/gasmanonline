#ifndef GASMANAPI_H
#define GASMANAPI_H

/*
 * gasmanAPI.h — public C API for the GasMan simulation shared library.
 *
 * Include this header when calling the library from C, C++, Python (ctypes),
 * Node.js (ffi-napi), or any other FFI.  No other GasMan headers are needed.
 */

#ifdef __cplusplus
extern "C" {
#endif

/* ---------------------------------------------------------------------------
 * Symbol visibility
 * -------------------------------------------------------------------------*/
#ifdef _WIN32
#  ifdef GASMANAPI_EXPORTS          /* defined when building gasmanAPI.dll  */
#    define GASMAN_API __declspec(dllexport)
#  else                             /* defined when using  gasmanAPI.dll    */
#    define GASMAN_API __declspec(dllimport)
#  endif
#else
#  define GASMAN_API __attribute__((visibility("default")))
#endif

/* ---------------------------------------------------------------------------
 * Error codes  (returned by GasManLastError)
 * -------------------------------------------------------------------------*/
#define GASMAN_OK                   0   /* success                          */
#define GASMAN_ERR_INVALID_PARAMS   1   /* bad time parameters              */
#define GASMAN_ERR_INI_NOT_FOUND    2   /* gasman.ini not found             */
#define GASMAN_ERR_INI_READ_FAILED  3   /* gasman.ini could not be read     */
#define GASMAN_ERR_JSON_PARSE       4   /* JSON / XML parse failure         */
#define GASMAN_ERR_SIMULATION       5   /* simulation engine returned error */
#define GASMAN_ERR_EXCEPTION        6   /* unexpected C++ exception         */

/* ---------------------------------------------------------------------------
 * GasManJsonToCsv
 *
 * Run a GasMan simulation and return the result as a CSV string.
 *
 * Parameters
 *   jsonStr      GasMan JSON or XML scenario encoded as UTF-8.
 *   len          Byte length of jsonStr.
 *   startSecond  Simulation start time in seconds.
 *   endSecond    Simulation end time in seconds.
 *   everySeconds Output row interval in seconds (must be > 0).
 *
 * Return value
 *   On success : pointer to a CSV string with columns:
 *                  Time, Agent, FGF, VA, CO, CKT, ALV, ART,
 *                  VRG, MUS, FAT, VEN, Uptake, Delivered
 *   On failure : pointer to an "Error: …" string.
 *
 * The returned pointer is valid until the SAME thread calls
 * GasManJsonToCsv again.  Copy the string if you need it to outlive
 * the next call.
 *
 * Thread safety
 *   Concurrent calls from different threads are serialized internally
 *   (a process-wide mutex guards shared engine state and the global
 *   locale setting).  Each thread has its own result buffer so callers
 *   on different threads can hold the returned pointer simultaneously.
 *
 *   GasManLastError() is per-thread and does not require external locking.
 * -------------------------------------------------------------------------*/
GASMAN_API const char* GasManJsonToCsv(
    const char* jsonStr,
    int         len,
    int         startSecond,
    int         endSecond,
    int         everySeconds
);

/* ---------------------------------------------------------------------------
 * GasManLastError
 *
 * Return the error code from the most recent GasManJsonToCsv call on the
 * calling thread.  Returns GASMAN_OK (0) when the last call succeeded.
 * -------------------------------------------------------------------------*/
GASMAN_API int GasManLastError(void);

/* ---------------------------------------------------------------------------
 * GasManLastDtMs
 *
 * Return the integration time-step in milliseconds the engine actually used on
 * the most recent GasManJsonToCsv call on the calling thread.  This equals the
 * scenario's dt_ms when one was supplied and applied; otherwise it is the
 * weight-derived allometric default.  Use it to confirm dt_ms took effect
 * (e.g. expect 6000 when you set dt_ms=6000).  Returns 0 if no call has run.
 * -------------------------------------------------------------------------*/
GASMAN_API int GasManLastDtMs(void);

/* ---------------------------------------------------------------------------
 * GasManErrorString
 *
 * Return a human-readable description for an error code.
 * The returned pointer is to a static string and is always valid.
 * -------------------------------------------------------------------------*/
GASMAN_API const char* GasManErrorString(int code);

#ifdef __cplusplus
}
#endif

#endif /* GASMANAPI_H */
