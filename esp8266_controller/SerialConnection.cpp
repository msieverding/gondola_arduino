#include "SerialConnection.hpp"
#include <string>

SerialConnection::SerialConnection(uint32_t baudrate, Gondola *gondola)
 : m_Baudrate(baudrate)
 , m_Gondola(gondola)
 , m_CommandInterpreter()
{
  Serial.println("Starting Serial Connection.");
  // following line is already done in main. Use it twice will fail
  // Serial.begin(m_Baudrate);

  m_CommandInterpreter = CommandInterpreter::get();
  m_CommandInterpreter->addCommand(std::string("move"), moveCommand);
}

SerialConnection::~SerialConnection()
{

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

    m_CommandInterpreter->interprete(cmd);

    // // parse string on serial (later change it with command interpreter)
    // // we expect 4 float: x, y, z, speed in cm/s
    // char *cmd = strtok(command, TOKENS);
    // // TODO: handle situation where input < 4 floats!!
    // newPosition.x = atof(cmd);
    // cmd = strtok(NULL, TOKENS); // in cm
    // newPosition.y = atof(cmd);
    // cmd = strtok(NULL, TOKENS); // in cm
    // newPosition.z = atof(cmd);
    // cmd = strtok(NULL, TOKENS); // in cm
    // speed = atof(cmd);   // in cm/s
    // cmd = strtok(NULL, TOKENS);
    //
    // m_Gondola->setTargetPosition(newPosition, speed);
  }
}

void SerialConnection::moveCommand(std::string &s)
{
  // TODO impl. move command
  Serial.println("-Move Command-");
}
