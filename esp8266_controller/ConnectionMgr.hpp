#ifndef _CONNECTION_MGR_HPP_
#define _CONNECTION_MGR_HPP_

#include "IConnection.hpp"
#include "SerialConnection.hpp"
#include "WebServer.hpp"
#include "IMQTTService.hpp"

/**
 * Enumeration for all possible connections
 */
typedef enum connectionType_s : byte {
  CON_ACCESS_POINT,                     //!< Open an access point
  CON_WIFI_CONNECTION,                  //!< Connect to a WiFi Network
  CON_DUAL_CONNECTION,                  //!< Open an access point and connect to a WiFi Network
  CON_NONE                              //!< No connection
} connectionType_t;

/**
 * Enumeration for all possible server types
 */
typedef enum mqttType_s : byte {
  MQTT_SERVER,                          //!< Act as a MQTT Server
  MQTT_CLIENT,                          //!< Act as a MQTT Client
  MQTT_CLIENT_ASYNC,                    //!< Act as an asynchronous MQTT Client
  MQTT_NONE                             //!< Don't provide a MqTT service
} mqttType_t;

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

  // TODO docu
  connectionType_t getConnectionType();

  /**
   * Change the type of the provided mqtt service
   * @param mqttType mqtt service to provide
   */
  void changeMqTTType(mqttType_t mqttType);

  /**
   * Request a change of the provided mqtt ervice
   * Change will be handled dring next loop() execution
   * @param mqttType Type of WebServer to provide
   */
  void requestChangeMqTTType(mqttType_t mqttType);

  // TODO DOKU
  mqttType_t getMqTTType();

  /**
   * Call loop() frequently to handle change requests
   * -> Change of connection type
   * -> Change of server type
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
  void contypeCommand(std::string &s);

  // instance
  static ConnectionMgr         *s_Instance;                 //!< instance of singleton

  // memvervariables
  connectionType_t              m_ConnectionType;           //!< type of connection to provide
  connectionType_t              m_ChangeConnectionType;     //!< type of connection to change to
  bool                          m_ChangeConnectionRequest;  //!< indicates a request to change the connection
  mqttType_t                    m_MqTTType;                 //!< type of mqtt service
  mqttType_t                    m_changeMqTTType;           //!< type of mqtt service to change to
  bool                          m_ChangeMqTTRequest;        //!< indicates a request to change the mqtt service
  IConnection                  *m_Connection;               //!< Pointer to current connection
  WebServer                     m_WebServer;                //!< Pointer to current WebServer
  IMQTTService                 *m_MqTTService;              //!< Pointer to current MqTT Service
};

#endif /* _CONNECTION_MGR_HPP_ */
