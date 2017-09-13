#ifndef _LOG_HPP_
#define _LOG_HPP_

#include <string>

typedef enum logLevel_e {
  LOG_WARNING,
  LOG_INFO,
  LOG_DEBUG,
} logLevel_t;

typedef union floatConverter_u {
  float f;
  uint8_t b[4];
  uint32_t u;
} floatConverter_t;

void setLogLevel(logLevel_t level);

void logWarning(const char *format, ...);
void logDebug(const char *format, ...);
void logInfo(const char *format, ...);

std::string floatToString(float f);
float stringtoFloat(std::string s);

#endif /* _LOG_HPP_ */
