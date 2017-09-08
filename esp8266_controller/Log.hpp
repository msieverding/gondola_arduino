#ifndef _LOG_HPP_
#define _LOG_HPP_

typedef enum logLevel_e {
  LOG_WARNING,
  LOG_INFO,
  LOG_DEBUG,
} logLevel_t;

void setLogLevel(logLevel_t level);

void logWarning(const char *format, ...);
void logDebug(const char *format, ...);
void logInfo(const char *format, ...);

#endif /* _LOG_HPP_ */
