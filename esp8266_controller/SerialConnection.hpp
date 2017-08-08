#ifndef _SERIAL_CONNECTION_HPP_
#define _SERIAL_CONNECTION_HPP_

#include "Gondola.hpp"
#include "IConnection.hpp"
#include "CommandInterpreter.hpp"

class SerialConnection : public IConnection
{
public:
  explicit SerialConnection(uint32_t baudrate, Gondola *m_Gondola);
  virtual ~SerialConnection();

  virtual void loop();

private:
  static void moveCommand(std::string &s);

  // mermbervariables
  uint32_t             m_Baudrate;
  Gondola             *m_Gondola;
  CommandInterpreter  *m_CommandInterpreter;
};

#endif /* _SERIAL_CONNECTION_HPP_ */
