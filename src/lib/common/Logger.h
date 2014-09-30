/*
 * Logger.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_LOGGER_H_
#define threescanner_LOGGER_H_

#include <string>
#include <format.h>

namespace threescanner {

static const int LOG_LEVEL_TRACE = 8;
static const int LOG_LEVEL_DEBUG = 7;
static const int LOG_LEVEL_INFO = 6;
static const int LOG_LEVEL_WARNING = 4;
static const int LOG_LEVEL_ERROR = 3;
static const int LOG_LEVEL_FATAL = 0;

#define __FILE_LINE__ fmt::sprintf("%s:%i", __FILE__, __LINE__)

/**
 * log any level
 *
 * \param level can be `LOG_LEVEL_{TRACE,DEBUG,INFO,WARNING,ERROR,FATAL}`
 * \param message the text to log
 */
void log(const int level, const std::string& message);

/**
 * log trace level. Trace level is enabled only if compiled with -DDEBUG
 * and -DTRACE.
 */
#if defined(TRACE) && defined(DEBUG) && !defined(NDEBUG)
inline void logTrace(const std::string& message) {
	log(LOG_LEVEL_TRACE, message);
#else
inline void logTrace(const std::string&) {
#endif
}

/**
 * log debug level. Debug level is enabled only if compiled with -DDEBUG
 */
#if defined(DEBUG) && !defined(NDEBUG)
inline void logDebug(const std::string& message) {
	log(LOG_LEVEL_DEBUG, message);
#else
inline void logDebug(const std::string&) {
#endif
}

/**
 * log info level
 */
inline void logInfo(const std::string& message) {
	log(LOG_LEVEL_INFO, message);
}

/**
 * log warning level
 */
inline void logWarning(const std::string& message) {
	log(LOG_LEVEL_WARNING, message);
}

/**
 * log warning level
 */
inline void logError(const std::string& message) {
	log(LOG_LEVEL_ERROR, message);
}

/**
 * log warning level
 */
inline void logFatal(const std::string& message) {
	log(LOG_LEVEL_FATAL, message);
}

/**
 * log any level with cppformat.
 * format and args must be used as printf
 *
 * \see https://github.com/cppformat/cppformat
 *
 * \param level can be `LOG_LEVEL_{TRACE,DEBUG,INFO,WARNING,ERROR,FATAL}`
 * \param format the format, as used with printf
 * \param args a variadic list, as used with printf
 */
void log(const int level, const std::string& format, const fmt::ArgList& args);

#if defined(TRACE) && defined(DEBUG) && !defined(NDEBUG)
inline void logTrace(const std::string& format, const fmt::ArgList& args) {
	log(LOG_LEVEL_TRACE, format, args);
#else
inline void logTrace(const std::string&, const fmt::ArgList&) {
#endif
}
FMT_VARIADIC(void, logTrace, const std::string&)

#if defined(DEBUG) && !defined(NDEBUG)
inline void logDebug(const std::string& format, const fmt::ArgList& args) {
	log(LOG_LEVEL_DEBUG, format, args);
#else
inline void logDebug(const std::string&, const fmt::ArgList&) {
#endif
}
FMT_VARIADIC(void, logDebug, const std::string&)

inline void logInfo(const std::string& format, const fmt::ArgList& args) {
	log(LOG_LEVEL_INFO, format, args);
}
FMT_VARIADIC(void, logInfo, const std::string&)

inline void logWarning(const std::string& format, const fmt::ArgList& args) {
	log(LOG_LEVEL_WARNING, format, args);
}
FMT_VARIADIC(void, logWarning, const std::string&)

inline void logError(const std::string& format, const fmt::ArgList& args) {
	log(LOG_LEVEL_ERROR, format, args);
}
FMT_VARIADIC(void, logError, const std::string&)

inline void logFatal(const std::string& format, const fmt::ArgList& args) {
	log(LOG_LEVEL_FATAL, format, args);
}
FMT_VARIADIC(void, logFatal, const std::string&)

} /* namespace threescanner */

#endif /* threescanner_LOGGER_H_ */
