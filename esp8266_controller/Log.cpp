#include <Arduino.h>
#include "Log.hpp"

static logLevel_t s_LogLevel = LOG_DEBUG;

void setLogLevel(logLevel_t level)
{
  s_LogLevel = level;
}

void logDebug(const char *format, ...)
{
  if (s_LogLevel >= LOG_DEBUG)
  {
    va_list arg;

    va_start(arg, format);
    Serial.printf(format, arg);
    va_end(arg);
  }
}

void logWarning(const char *format, ...)
{
  if (s_LogLevel >= LOG_WARNING)
  {
    va_list arg;

    va_start(arg, format);
    Serial.printf(format, arg);
    va_end(arg);
  }
}

void logInfo(const char *format, ...)
{
  if (s_LogLevel >= LOG_INFO)
  {
    va_list arg;

    va_start(arg, format);
    Serial.printf(format, arg);
    va_end(arg);
  }
}
