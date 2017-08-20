#include "SerialConnection.hpp"
#include <string>

SerialConnection *SerialConnection::s_Instance = NULL;

SerialConnection *SerialConnection::create(uint32_t baudrate, Gondola *gondola)
{
  if (!s_Instance)
    s_Instance = new SerialConnection(baudrate, gondola);
  return s_Instance;
}

SerialConnection::SerialConnection(uint32_t baudrate, Gondola *gondola)
 : m_Baudrate(baudrate)
 , m_Gondola(gondola)
 , m_CommandInterpreter()
{
  Serial.println("Starting Serial Connection.");
  // following line is already done in main. Use it twice will fail
  // Serial.begin(m_Baudrate);

  m_CommandInterpreter = CommandInterpreter::get();
  m_CommandInterpreter->addCommand("move", moveCommand);
}

SerialConnection::~SerialConnection()
{
  m_CommandInterpreter->deleteCommand("move", moveCommand);
  delete(m_CommandInterpreter);
  s_Instance = NULL;
}

void SerialConnection::loop()
{
  if (Serial.available() > 0)
  {
    char command[255];
    Coordinate newPosition;
    float speed;
    long start_time;
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

void SerialConnection::moveCommand(std::string &s)
{
  CommandInterpreter *CI = CommandInterpreter::get();
  uint8_t args = CI->getNumArgument(s);
  Coordinate newPosition;
  float speed;

  if(args != 4)
  {
    Serial.println("Unsupported!");
    Serial.println("Usage: move x y z s");
    Serial.println("x - float for x coordinate (e.g. 1.0)");
    Serial.println("y - float for y coordinate (e.g. 1.0)");
    Serial.println("z - float for z coordinate (e.g. 1.0)");
    Serial.println("s - float for speed (e.g. 1.0)");
    return;
  }
  std::string arg;
  CI->getArgument(s, arg, 0);
  newPosition.x = atof(arg.c_str());
  CI->getArgument(s, arg, 1);
  newPosition.y = atof(arg.c_str());
  CI->getArgument(s, arg, 2);
  newPosition.z = atof(arg.c_str());
  CI->getArgument(s, arg, 3);
  speed = atof(arg.c_str());

  s_Instance->m_Gondola->setTargetPosition(newPosition, speed);
}
