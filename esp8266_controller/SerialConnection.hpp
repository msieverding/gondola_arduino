#ifndef _SERIAL_CONNECTION_HPP_
#define _SERIAL_CONNECTION_HPP_

#include "Gondola.hpp"
#include "IConnection.hpp"
#include "CommandInterpreter.hpp"

class SerialConnection : public IConnection
{
public:
  /**
   * [create description]
   * @param  baudrate Baudrate to use
   * @param  gondola  gondola to use for requests
   * @return          pointer to Instance
   */
  static SerialConnection *create(uint32_t baudrate, Gondola *gondola);
  /**
   * virtual destructor
   */
  virtual ~SerialConnection();

  /**
   * Call this loop in periodically to handle serial commands etc.
   */
  virtual void loop();

private:
  SerialConnection(uint32_t baudrate, Gondola *m_Gondola);
  static void moveCommand(std::string &s);

  // instance
  static SerialConnection    *s_Instance;
  // membervariables
  uint32_t                    m_Baudrate;
  Gondola                    *m_Gondola;
  CommandInterpreter         *m_CommandInterpreter;
};

#endif /* _SERIAL_CONNECTION_HPP_ */
