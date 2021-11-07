#pragma once

#include "defs.h"
#include "trace.h"

typedef enum err {
    /**
      * There was no error, everything is good
      */
    NO_ERROR,

    /**
     * Some check failed, basically an internal error
     */
    ERROR_CHECK_FAILED,

    /**
     * The requested item was not found
     */
    ERROR_NOT_FOUND,

    /**
     * The function ran out of resources to continue
     */
    ERROR_OUT_OF_RESOURCES,

    /**
     * Got a bad format, most likely when parsing file
     */
    ERROR_BAD_FORMAT,
} err_t;

static inline const char* get_error_name(err_t err) {
    switch (err) {
        case NO_ERROR: return "NO_ERROR";
        case ERROR_CHECK_FAILED: return "ERROR_CHECK_FAILED";
        case ERROR_NOT_FOUND: return "ERROR_NOT_FOUND";
        case ERROR_OUT_OF_RESOURCES: return "ERROR_OUT_OF_RESOURCES";
        case ERROR_BAD_FORMAT: return "ERROR_BAD_FORMAT";
        default: return "<unknown err>";
    }
}

/**
 * Check if there was an error
 */
#define IS_ERROR(err) (err != NO_ERROR)

//----------------------------------------------------------------------------------------------------------------------
// A check that fails if the expression returns false
//----------------------------------------------------------------------------------------------------------------------

#define CHECK_ERROR_LABEL(check, error, label, ...) \
    do { \
        if (!(check)) { \
            err = error; \
            IF(HAS_ARGS(__VA_ARGS__))(ERROR(__VA_ARGS__)); \
            ERROR("Check failed with error %s in function %s (%s:%d)", get_error_name(err), __FUNCTION__, __FILENAME__, __LINE__); \
            goto label; \
        } \
    } while(0)

#define CHECK_ERROR(check, error, ...)              CHECK_ERROR_LABEL(check, error, cleanup, ## __VA_ARGS__)
#define CHECK_LABEL(check, label, ...)              CHECK_ERROR_LABEL(check, ERROR_CHECK_FAILED, label, ## __VA_ARGS__)
#define CHECK(check, ...)                           CHECK_ERROR_LABEL(check, ERROR_CHECK_FAILED, cleanup, ## __VA_ARGS__)

#ifdef __PENTAGON_DEBUG__
    #define DEBUG_CHECK_ERROR(check, error, ...)              CHECK_ERROR_LABEL(check, error, cleanup, ## __VA_ARGS__)
    #define DEBUG_CHECK_LABEL(check, label, ...)              CHECK_ERROR_LABEL(check, ERROR_CHECK_FAILED, label, ## __VA_ARGS__)
    #define DEBUG_CHECK(check, ...)                           CHECK_ERROR_LABEL(check, ERROR_CHECK_FAILED, cleanup, ## __VA_ARGS__)
#else
    #define DEBUG_CHECK_ERROR(check, error, ...)              ({ (void)check; })
    #define DEBUG_CHECK_LABEL(check, label, ...)              ({ (void)check; })
    #define DEBUG_CHECK(check, ...)                           ({ (void)check; })
#endif

//----------------------------------------------------------------------------------------------------------------------
// A check that fails without a condition
//----------------------------------------------------------------------------------------------------------------------

#define CHECK_FAIL(...)                             CHECK_ERROR_LABEL(0, ERROR_CHECK_FAILED, cleanup, ## __VA_ARGS__)
#define CHECK_FAIL_ERROR(error, ...)                CHECK_ERROR_LABEL(0, error, cleanup, ## __VA_ARGS__)
#define CHECK_FAIL_LABEL(label, ...)                CHECK_ERROR_LABEL(0, ERROR_CHECK_FAILED, label, ## __VA_ARGS__)
#define CHECK_FAIL_ERROR_LABEL(error, label, ...)   CHECK_ERROR_LABEL(0, error, label, ## __VA_ARGS__)

//----------------------------------------------------------------------------------------------------------------------
// A check that fails if an error was returned, used around functions returning an error
//----------------------------------------------------------------------------------------------------------------------

#define CHECK_AND_RETHROW_LABEL(error, label) \
    do { \
        err = error; \
        if (IS_ERROR(err)) { \
            ERROR("\trethrown at %s (%s:%d)", __FUNCTION__, __FILENAME__, __LINE__); \
            goto label; \
        } \
    } while(0)

#define CHECK_AND_RETHROW(error) CHECK_AND_RETHROW_LABEL(error, cleanup)

//----------------------------------------------------------------------------------------------------------------------
// Misc utilities
//----------------------------------------------------------------------------------------------------------------------

#define WARN_ON(check, ...)     \
    do {                        \
        if (check) {            \
            WARN(__VA_ARGS__);  \
        }                       \
    } while(0)

#ifdef __PENTAGON_DEBUG__

#define ASSERT(check) \
    do { \
        if (!(check)) { \
           ERROR("Assert failed at %s (%s:%d)", __FUNCTION__, __FILENAME__, __LINE__); \
            __builtin_trap(); \
        } \
    } while(0)

#else

#define ASSERT(check) \
    do { \
        (void)check; \
    } while(0);

#endif