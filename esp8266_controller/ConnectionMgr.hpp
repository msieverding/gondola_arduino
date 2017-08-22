#ifndef _CONNECTION_MGR_HPP_
#define _CONNECTION_MGR_HPP_

#include "IConnection.hpp"
#include "WebServer.hpp"

typedef enum connectionType_s : byte {
  CON_ACCESS_POINT,
  CON_WIFI_CONNECTION,
  CON_DUAL_CONNECTION,
  CON_NONE
} connectionType_t;

typedef enum serverType_s : byte {
  SERV_NORMAL,
  SERV_MASTER,
  SERV_SLAVE,
  SERV_NONE
} serverType_t;
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
   */
  void changeConnection(connectionType_t connectionType);

  /**
  * request a change of the connection. Usefull if change is done inside the WebServer of a connection
  * to avoid conflicts when deleting the old connection.
  * Connection will be changed during the next loop() execution.
  * @param contype connection type to use
  */
  void requestChangeConnection(connectionType_t connectionType);

  // TODO docu
  void changeServerType(serverType_t serverType, bool force = false);

  // TODO docu
  void requestChangeServerType(serverType_t serverType);

  /**
   * Call loop() frequently to handle change requests
   */
  void loop();

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

  // instance
  static ConnectionMgr         *s_Instance;
  // memvervariables
  connectionType_t              m_ConnectionType;
  connectionType_t              m_ChangeConnectionType;
  bool                          m_ChangeConnectionRequest;
  serverType_t                  m_ServerType;
  serverType_t                  m_ChangeServerType;
  bool                          m_ChangeServerRequest;
  IConnection                  *m_Connection;
  WebServer                    *m_WebServer;
};

#endif /* _CONNECTION_MGR_HPP_ */
