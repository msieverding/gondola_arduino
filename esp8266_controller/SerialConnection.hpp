#ifndef _SERIAL_CONNECTION_HPP_
#define _SERIAL_CONNECTION_HPP_

#include "Gondola.hpp"
#include "IConnection.hpp"
#include "CommandInterpreter.hpp"

class SerialConnection : public IConnection
{
public:
  static SerialConnection *create(uint32_t baudrate, Gondola *m_Gondola);
  virtual ~SerialConnection();

  virtual void loop();

private:
  SerialConnection(uint32_t baudrate, Gondola *m_Gondola);
  static void moveCommand(std::string &s);

  static SerialConnection    *s_Instance;
  // mermbervariables
  uint32_t                    m_Baudrate;
  Gondola                    *m_Gondola;
  CommandInterpreter         *m_CommandInterpreter;
};

#endif /* _SERIAL_CONNECTION_HPP_ */
