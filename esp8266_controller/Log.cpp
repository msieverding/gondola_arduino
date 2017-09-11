#include <Arduino.h>
#include <stdarg.h>
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
    char buf[256]; // resulting string limited to 128 chars
    va_list args;
    va_start (args, format );
    vsnprintf(buf, 256, format, args);
    va_end (args);
    Serial.print("[DBG] ");
    Serial.print(buf);
  }
}

void logWarning(const char *format, ...)
{
  if (s_LogLevel >= LOG_WARNING)
  {
    char buf[128]; // resulting string limited to 128 chars
    va_list args;
    va_start (args, format );
    vsnprintf(buf, 128, format, args);
    va_end (args);
    Serial.print("[WRN] ");
    Serial.print(buf);
  }
}

void logInfo(const char *format, ...)
{
  if (s_LogLevel >= LOG_INFO)
  {
    char buf[128]; // resulting string limited to 128 chars
    va_list args;
    va_start (args, format );
    vsnprintf(buf, 128, format, args);
    va_end (args);
    Serial.print("[INF] ");
    Serial.print(buf);
  }
}

std::string floatToString(float f)
{
  char buf[20];
  return std::string(dtostrf(f, 4, 2, buf));
}

float stringtoFloat(std::string s)
{
  return (float)atof(s.c_str());
}
