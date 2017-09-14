#ifndef _SERIAL_CONNECTION_HPP_
#define _SERIAL_CONNECTION_HPP_

#include "Gondola.hpp"
#include "IConnection.hpp"
#include "CommandInterpreter.hpp"

class SerialConnection : public IConnection
{
public:
  /**
   * Create the instance of the serial connection
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
  /**
   * Constrcutor
   * @param baudrate baudrate to use for UART
   */
  SerialConnection(uint32_t baudrate);

  /**
   * CI Command to change log level
   * @param s command
   */
  bool loglevelCommand(std::string &s);

  // instance
  static SerialConnection    *s_Instance;           //!< instance of singleton
  // membervariables
  uint32_t                    m_Baudrate;           //!< Baudrate of UART
  CommandInterpreter         *m_CommandInterpreter; //!< Pointer to CommandInterpreter
};

#endif /* _SERIAL_CONNECTION_HPP_ */
