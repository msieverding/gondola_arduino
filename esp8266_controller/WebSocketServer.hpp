#ifndef _WEB_SOCKET_SERVER_HPP_
#define _WEB_SOCKET_SERVER_HPP_

#include <Arduino.h>
#include <Hash.h>   // Need to include it here, since WebSocketsServer library will not find it... That would result in a build error
#include <WebSocketsServer.h>
#include <list>
#include "Coordinate.hpp"
#include "Log.hpp"
#include "Gondola.hpp"
#include "IWebSocket.hpp"

/**
 * WebSocket Server as Server for gondola
 */
class WebSocketServer : public IWebSocket
{
public:
  /**
   * Constructor
   * @param port port to use
   */
  WebSocketServer(uint16_t port);

  /**
   * Virtual destructor
   */
  virtual ~WebSocketServer();

  /**
   * Loop to call frequently
   */
  virtual void loop();

private:
  /**
   * Event that is trigered on incomming message
   * @param num     Client number
   * @param type    type of data
   * @param payload message content
   * @param length  length of payload
   */
  void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);

  /**
   * function to call, when an anchor should move.
   * Is registerd at gonola and called from there
   * @param anchorInfo information about the anchor
   */
  void remoteAnchorMoveFunction(anchorInformation_t &anchorInfo);

  /**
   * function to call, when an anchor should be initialised.
   * Is registerd at gonola and called from there
   * @param anchorInfo information about the anchor
   */
  void remoteAnchorInitFunction(anchorInformation_t &anchorInfo);

  // membervariables
  uint16_t                        m_Port;             //!< Port to use
  WebSocketsServer                m_WebSocketServer;  //!< Server from WebSocketsLibrary
  Gondola                        *m_Gondola;          //!< pointer to Gondola

};

#endif /* _WEB_SOCKET_SERVER_HPP_ */
