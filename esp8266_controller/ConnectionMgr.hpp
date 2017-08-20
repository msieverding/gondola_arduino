#ifndef _CONNECTION_MGR_HPP_
#define _CONNECTION_MGR_HPP_

#include "IConnection.hpp"
#include "WebServer.hpp"

typedef enum conType_s : byte {
  CON_ACCESS_POINT,
  CON_WIFI_CONNECTION,
  CON_NONE
} conType_t;

/**
 * Class to manage the connection of the chip
 */
class ConnectionMgr
{
public:
  /**
   * Get instance of ConnectionMgr
   * @return pointer to instance
   */
  static ConnectionMgr *get();
  /**
   * virtual destructor
   */
  virtual ~ConnectionMgr();

  /**
  * initialize the connection with a given WebServer
  * @param  webServer WebServer to use
  * @return           success
  */
  bool initConnection(WebServer *webServer);
  /**
   * Change the connection to another type
   * @param  contype type of new connection
   * @return         success
   */
  bool changeConnection(conType_t contype);
  /**
   * Call loop() frequently to handle change requests
   */
  void loop();
  /**
   * request a change of the connection. Usefull if change is done inside the WebServer of a connection
   * to avoid conflicts when deleting the old connection.
   * Connection will be changed during the next loop() execution.
   * @param contype connection type to use
   */
  void requestChangeConnection(conType_t contype);

private:
  /**
   * Constructor
   */
  ConnectionMgr();

  /**
   * Serial command to change the connection
   * @param s serial input with all commandword and all arguments
   */
  static void contypeCommand(std::string &s);

  // memvervariables
  static ConnectionMgr  *s_Instance;
  conType_t              m_ConType;
  bool                   m_ChangeRequest;
  conType_t              m_ChangeContype;
  IConnection           *m_Connection;
  WebServer             *m_WebServer;
};

#endif /* _CONNECTION_MGR_HPP_ */
