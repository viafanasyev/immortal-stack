/**
 * @file
 */
#ifndef IMMORTAL_STACK_LOGGER_H
#define IMMORTAL_STACK_LOGGER_H

#include <cassert>
#include <cstdarg>
#include <cstdio>
#include "environment.h"

FILE* _logFile = nullptr;

constexpr const char* defaultLogFileName = "log.txt";

void logClose() {
    if (_logFile != nullptr) {
        fclose(_logFile);
        _logFile = nullptr;
    }
}

void logOpen(const char* logFilePath = defaultLogFileName, const char* modes = "a+") {
    assert(logFilePath != nullptr);
    assert(modes != nullptr);

    if (_logFile != nullptr) logClose();
    _logFile = fopen(logFilePath, modes);
}

void logPrintf(const char* format, ...) {
    assert(_logFile != nullptr);
    assert(format != nullptr);

    va_list args;
    va_start(args, format);
    vfprintf(_logFile, format, args);
    va_end(args);
}

//----------------------------------------------------------------------------------------------------------------------

inline void logValue(int value) {
    logPrintf("%d", value);
}

inline void logValue(unsigned int value) {
    logPrintf("%u", value);
}

inline void logValue(long int value) {
    logPrintf("%ld", value);
}

inline void logValue(long long value) {
    logPrintf("%lld", value);
}

inline void logValue(unsigned long long value) {
    logPrintf("%llu", value);
}

inline void logValue(short value) {
    logPrintf("%hd", value);
}

inline void logValue(unsigned short value) {
    logPrintf("%hu", value);
}

inline void logValue(float value) {
    logPrintf("%lg", value);
}

inline void logValue(double value) {
    logPrintf("%lg", value);
}

inline void logValue(long double value) {
    logPrintf("%Lg", value);
}

inline void logValue(char value) {
    logPrintf("%c", value);
}

inline void logValue(unsigned char value) {
    logPrintf("%hhu", value);
}

inline void logValue(const char* value) {
    logPrintf("%s", value);
}

inline void logValue(bool value) {
    logPrintf("%s", value ? "true" : "false");
}

//----------------------------------------------------------------------------------------------------------------------

#define LOG_VALUE_INDENTED(value, indent) do {                                                                         \
    logPrintf(indent "%s = ", #value);                                                                                 \
    logValue(value);                                                                                                   \
    logPrintf("\n");                                                                                                   \
} while (0)

#define LOG_VALUE(value) LOG_VALUE_INDENTED(value, "")

#define LOG_ARRAY_INDENTED(array, length, indent) do {                                                                 \
    logPrintf(indent "%s [" PTR_FORMAT "]", #array, (uintptr_t)array);                                                 \
    if (array == nullptr) {                                                                                            \
        logPrintf("\n");                                                                                               \
        break;                                                                                                         \
    }                                                                                                                  \
    logPrintf(" = {\n");                                                                                               \
    for (ssize_t i = 0; i < length; ++i) {                                                                             \
        logPrintf(indent "\t[%zu] = ", i);                                                                             \
        logValue(array[i]);                                                                                            \
        logPrintf("\n");                                                                                               \
    }                                                                                                                  \
    logPrintf(indent "}\n");                                                                                           \
} while (0)

#define LOG_ARRAY(array, length) LOG_ARRAY_INDENTED(array, length, "")

#endif // IMMORTAL_STACK_LOGGER_H
