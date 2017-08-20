#ifndef _LOG_HPP_
#define _LOG_HPP_

#include <string>

class Log
{
public:
  typedef enum logLevel_e {
    LOG_WARNING,
    LOG_INFO,
    LOG_DEBUG,
  } logLevel_t;

  static void setLogLevel(logLevel_t level);

  static void logWarning(std::string &s);
  static void logWarning(const char * c);
  static void logWarning(int i);
  static void logWarning(long int l);
  static void logWarning(float f);
  static void logDebug(std::string &s);
  static void logDebug(const char * c);
  static void logDebug(int i);
  static void logDebug(long int l);
  static void logDebug(float f);
  static void logInfo(std::string &s);
  static void logInfo(const char * c);
  static void logInfo(int i);
  static void logInfo(long int l);
  static void logInfo(float f);

private:
  Log() {}
  virtual ~Log() {}

  static logLevel_t s_LogLevel;
};

#endif /* _LOG_HPP_ */
