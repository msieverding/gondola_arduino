#include "SerialConnection.hpp"
#include <string>

SerialConnection::SerialConnection(uint32_t baudrate, Gondola *gondola)
 : m_Baudrate(baudrate)
 , m_Gondola(gondola)
{
  Serial.println("Starting Serial Connection.");
  // Serial.begin(m_Baudrate);
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


    // read a line from serial
    Serial.readBytesUntil('\n', command, 255);

    // parse string on serial (later change it with command interpreter)
    // we expect 4 float: x, y, z, speed in cm/s
    char *cmd = strtok(command, TOKENS);
    // TODO: handle situation where input < 4 floats!!
    newPosition.x = atof(cmd);
    cmd = strtok(NULL, TOKENS); // in cm
    newPosition.y = atof(cmd);
    cmd = strtok(NULL, TOKENS); // in cm
    newPosition.z = atof(cmd);
    cmd = strtok(NULL, TOKENS); // in cm
    speed = atof(cmd);   // in cm/s
    cmd = strtok(NULL, TOKENS);
    
    m_Gondola->setTargetPosition(newPosition, speed);
  }
}
