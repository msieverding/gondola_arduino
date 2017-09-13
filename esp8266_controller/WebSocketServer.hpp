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

// TODO Doc

class WebSocketServer : public IWebSocket
{
public:
  WebSocketServer(uint16_t port);
  virtual ~WebSocketServer();
  virtual void loop();

private:
  void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
  void remoteAnchorMoveFunction(uint8_t num, anchorInformation_t &anchorInfo);

  // membervariables
  uint16_t                        m_Port;
  WebSocketsServer                m_WebSocketServer;
  Gondola                        *m_Gondola;
};

#endif /* _WEB_SOCKET_SERVER_HPP_ */
