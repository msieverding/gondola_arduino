#ifndef _LOG_HPP_
#define _LOG_HPP_

#include <string>

typedef enum logLevel_e {
  LOG_WARNING,
  LOG_INFO,
  LOG_DEBUG,
} logLevel_t;

void setLogLevel(logLevel_t level);

void logWarning(const char *format, ...);
void logDebug(const char *format, ...);
void logInfo(const char *format, ...);

std::string FloatToString(float f);

#endif /* _LOG_HPP_ */
