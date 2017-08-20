#include <Arduino.h>
#include "Log.hpp"

Log::logLevel_t Log::s_LogLevel = LOG_DEBUG;

void Log::setLogLevel(logLevel_t level)
{
  s_LogLevel = level;
}

void Log::logWarning(std::string &s)
{
  if (s_LogLevel >= LOG_WARNING)
    Serial.print(s.c_str());
}

void Log::logWarning(const char * c)
{
  if (s_LogLevel >= LOG_WARNING)
    Serial.print(c);
}

void Log::logWarning(int i)
{
  if (s_LogLevel >= LOG_WARNING)
    Serial.print(i);
}

void Log::logWarning(long int l)
{
  if (s_LogLevel >= LOG_WARNING)
    Serial.print(l);
}

void Log::logWarning(float f)
{
  if (s_LogLevel >= LOG_WARNING)
    Serial.print(f);
}

void Log::logDebug(std::string &s)
{
  if (s_LogLevel >= LOG_DEBUG)
    Serial.print(s.c_str());
}

void Log::logDebug(const char * c)
{
  if (s_LogLevel >= LOG_DEBUG)
    Serial.print(c);
}

void Log::logDebug(int i)
{
  if (s_LogLevel >= LOG_DEBUG)
    Serial.print(i);
}

void Log::logDebug(long int l)
{
  if (s_LogLevel >= LOG_DEBUG)
    Serial.print(l);
}

void Log::logDebug(float f)
{
  if (s_LogLevel >= LOG_DEBUG)
    Serial.print(f);
}

void Log::logInfo(std::string &s)
{
  if (s_LogLevel >= LOG_INFO)
    Serial.print(s.c_str());
}

void Log::logInfo(const char * c)
{
  if (s_LogLevel >= LOG_INFO)
    Serial.print(c);
}

void Log::logInfo(int i)
{
  if (s_LogLevel >= LOG_INFO)
    Serial.print(i);
}

void Log::logInfo(long int l)
{
  if (s_LogLevel >= LOG_INFO)
    Serial.print(l);
}

void Log::logInfo(float f)
{
  if (s_LogLevel >= LOG_INFO)
    Serial.print(f);
}
