#include <Arduino.h>
#include <stdarg.h>
#include "Log.hpp"
#include "Config.hpp"

#define MAX_LENGTH    256

static logLevel_t s_LogLevel = LOG_DEBUG;

void setLogLevel(logLevel_t level)
{
  s_LogLevel = level;
  Config::get()->setDEBUG_LOG(level);
  Config::get()->writeToEEPROM();
}

void logDebug(const char *format, ...)
{
  if (s_LogLevel >= LOG_DEBUG)
  {
    char buf[MAX_LENGTH]; // resulting string limited to 128 chars
    va_list args;
    va_start (args, format );
    vsnprintf(buf, MAX_LENGTH, format, args);
    va_end (args);
    Serial.print("[DBG] ");
    Serial.print(buf);
  }
}

void logWarning(const char *format, ...)
{
  if (s_LogLevel >= LOG_WARNING)
  {
    char buf[MAX_LENGTH]; // resulting string limited to 128 chars
    va_list args;
    va_start (args, format );
    vsnprintf(buf, MAX_LENGTH, format, args);
    va_end (args);
    Serial.print("[WRN] ");
    Serial.print(buf);
  }
}

void logInfo(const char *format, ...)
{
  if (s_LogLevel >= LOG_INFO)
  {
    char buf[MAX_LENGTH]; // resulting string limited to 128 chars
    va_list args;
    va_start (args, format );
    vsnprintf(buf, MAX_LENGTH, format, args);
    va_end (args);
    Serial.print("[INF] ");
    Serial.print(buf);
  }
}

void logVerbose(const char *format, ...)
{
  if (s_LogLevel >= LOG_VERBOSE)
  {
    char buf[MAX_LENGTH]; // resulting string limited to 128 chars
    va_list args;
    va_start (args, format );
    vsnprintf(buf, MAX_LENGTH, format, args);
    va_end (args);
    Serial.print("[VBO] ");
    Serial.print(buf);
  }
}


std::string floatToString(float f)
{
  char buf[20];
  return std::string(dtostrf(f, 4, 2, buf));
}

float stringToFloat(std::string s)
{
  return (float)atof(s.c_str());
}
