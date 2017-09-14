#ifndef _CONNECTION_MGR_HPP_
#define _CONNECTION_MGR_HPP_

#include "IConnection.hpp"
#include "SerialConnection.hpp"
#include "WebServer.hpp"
#include "IWebSocket.hpp"

/**
 * Enumeration for all possible connections
 */
typedef enum connectionType_e : byte {
  CON_NONE,                             //!< No connection
  CON_ACCESS_POINT,                     //!< Open an access point
  CON_WIFI_CONNECTION,                  //!< Connect to a WiFi Network
  CON_DUAL_CONNECTION                   //!< Open an access point and connect to a WiFi Network
} connectionType_t;

/**
 * Enumeration for all possible WebSocket types
 */
typedef enum webSocketType_e : byte {
  WEBSOCKET_NONE,                            //!< Don't provide a WebSocket
  WEBSOCKET_SERVER,                          //!< Open a WebSocketServer
  WEBSOCKET_CLIENT,                          //!< Connect to a given WebSocketServer
} webSocketType_t;

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
   * Change the connection to another type
   * @param  connetionType type of new connection
   */
  void changeConnection(connectionType_t connectionType);

  /**
  * request a change of the connection. Usefull if change is done inside the WebServer of a connection
  * to avoid conflicts when deleting the old connection.
  * Connection will be changed during the next loop() execution.
  * @param connectionType connection type to use
  */
  void requestChangeConnection(connectionType_t connectionType);

  /**
   * Get the current used connection type
   * @return connection type
   */
  connectionType_t getConnectionType();

  /**
   * Change the type of the WebSocket
   * @param  webSocketType type of new WebSocket
   */
  void changeWebSocket(webSocketType_t webSocketType);

  /**
   * Get the current used WebSocketType
   * @return current used WebSocketType
   */
  webSocketType_t getWebSocketType();

  /**
   * Call loop() frequently to handle change requests
   * -> Change of connection type
   * -> Change of server type
   */
  void loop();

  /**
   * Reset all connections
   */
  static void reset();

private:
  /**
   * Constructor
   */
  ConnectionMgr();

  /**
   * Serial command to change the connection
   * @param s serial input with all commandword and all arguments
   */
  bool contypeCommand(std::string &s);

  // instance
  static ConnectionMgr         *s_Instance;                 //!< instance of singleton

  // memvervariables
  // Connection
  connectionType_t              m_ConnectionType;           //!< type of connection to provide
  connectionType_t              m_ChangeConnectionType;     //!< type of connection to change to
  bool                          m_ChangeConnectionRequest;  //!< indicates a request to change the connection
  IConnection                  *m_Connection;               //!< Pointer to current connection
  // WebSocket
  webSocketType_t               m_WebSocketType;            //!< Current used WebSocket tye
  IWebSocket                   *m_WebSocket;                //!< Pointer to WebSocket to provide
  // WebServer
  WebServer                     m_WebServer;                //!< Pointer to current WebServer
};

#endif /* _CONNECTION_MGR_HPP_ */
