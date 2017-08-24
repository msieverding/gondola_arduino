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
  /**
   * Constrcutor
   * @param baudrate baudrate to use for UART
   */
  SerialConnection(uint32_t baudrate);

  /**
   * CI Command to move gondola
   * @param s command
   */
  static void moveCommand(std::string &s);

  /**
   * CI Command to change log level
   * @param s command
   */
  static void loglevelCommand(std::string &s);

  /**
   * CI Command to get help
   * @param s command
   */
  static void helpCommand(std::string &s);

  // instance
  static SerialConnection    *s_Instance;           //!< instance of singleton
  // membervariables
  uint32_t                    m_Baudrate;           //!< Baudrate of UART
  CommandInterpreter         *m_CommandInterpreter; //!< Pointer to CommandInterpreter
};

#endif /* _SERIAL_CONNECTION_HPP_ */
