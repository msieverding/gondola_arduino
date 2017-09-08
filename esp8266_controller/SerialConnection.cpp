#include "SerialConnection.hpp"
#include "Log.hpp"
#include "ConnectionMgr.hpp"
#include <string>

SerialConnection *SerialConnection::s_Instance = NULL;

SerialConnection *SerialConnection::create(uint32_t baudrate)
{
  if (!s_Instance)
    s_Instance = new SerialConnection(baudrate);
  return s_Instance;
}

SerialConnection::SerialConnection(uint32_t baudrate)
 : m_Baudrate(baudrate)
 , m_CommandInterpreter()
{
  Serial.begin(m_Baudrate);
  Serial.println("\n\n");
  logDebug("Starting Serial Connection\n");

  m_CommandInterpreter = CommandInterpreter::get();
  m_CommandInterpreter->addCommand("loglevel", std::bind(&SerialConnection::loglevelCommand, this, std::placeholders::_1));

}

SerialConnection::~SerialConnection()
{
  m_CommandInterpreter->deleteCommand("loglevel");
  delete(m_CommandInterpreter);
  s_Instance = NULL;
}

void SerialConnection::loop()
{
  if (Serial.available() > 0)
  {
    char command[255];
    uint8_t bytesRead;

    // read a line from serial
    bytesRead = Serial.readBytesUntil('\n', command, 255);

    // Set a '\0' at the end as a char-string terminator
    if (command[bytesRead - 1] == '\r')   // check if CR was send
    {
      command[bytesRead - 1] = '\0';      // then overwrite it with '\0'
    }
    else
    {
      bytesRead++;
      command[bytesRead - 1] = '\0';      // Set'\0' as last charachter
    }

    std::string cmd(command);             // make a string out of it

    m_CommandInterpreter->interprete(cmd);// interprete the string
  }
}

void SerialConnection::loglevelCommand(std::string &s)
{
  CommandInterpreter *CI = CommandInterpreter::get();
  std::string arg0;
  CI->getArgument(s, arg0, 0);
  if (arg0.compare("info") == 0)
  {
    setLogLevel(LOG_INFO);
  }
  else if (arg0.compare("debug") == 0)
  {
    setLogLevel(LOG_DEBUG);
  }
  else if (arg0.compare("warning") == 0)
  {
    setLogLevel(LOG_WARNING);
  }
  else
  {
    logWarning("Unsupported!\n");
    logWarning("Usage: loglevel level\nLevels:\n\twarning\t only warnings are displayed\n\tinfo\t additional information is displayed\n\tdebug\t addtitional debug output is provided\n");
  }
}
