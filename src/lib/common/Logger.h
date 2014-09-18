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

void log(const int level, const std::string& message);

#if defined(TRACE) && defined(DEBUG) && !defined(NDEBUG)
inline void logTrace(const std::string& message) {
	log(LOG_LEVEL_TRACE, message);
#else
inline void logTrace(const std::string&) {
#endif
}

#if defined(DEBUG) && !defined(NDEBUG)
inline void logDebug(const std::string& message) {
	log(LOG_LEVEL_DEBUG, message);
#else
inline void logDebug(const std::string&) {
#endif
}

inline void logInfo(const std::string& message) {
	log(LOG_LEVEL_INFO, message);
}

inline void logWarning(const std::string& message) {
	log(LOG_LEVEL_WARNING, message);
}

inline void logError(const std::string& message) {
	log(LOG_LEVEL_ERROR, message);
}

inline void logFatal(const std::string& message) {
	log(LOG_LEVEL_FATAL, message);
}

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
