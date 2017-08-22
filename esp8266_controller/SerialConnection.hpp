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
  static SerialConnection *create(uint32_t baudrate);
  /**
   * virtual destructor
   */
  virtual ~SerialConnection();

  /**
   * Call this loop in periodically to handle serial commands etc.
   */
  virtual void loop();

private:
  SerialConnection(uint32_t baudrate);
  static void moveCommand(std::string &s);
  static void loglevelCommand(std::string &s);
  static void helpCommand(std::string &s);

  // instance
  static SerialConnection    *s_Instance;
  // membervariables
  uint32_t                    m_Baudrate;
  CommandInterpreter         *m_CommandInterpreter;
};

#endif /* _SERIAL_CONNECTION_HPP_ */
