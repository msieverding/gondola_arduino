#ifndef _WEB_SOCKET_SERVER_HPP_
#define _WEB_SOCKET_SERVER_HPP_

#include <Arduino.h>
#include "IWebSocket.hpp"
#include <Hash.h>   // Need to include it here, since WebSocketsServer library will not find it... That would result in a build error
#include <WebSocketsServer.h>

class WebSocketServer : public IWebSocket
{
public:
  WebSocketServer(uint16_t port);
  virtual ~WebSocketServer();
  virtual void loop();

private:
  void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);

  // membervariables
  uint16_t              m_Port;
  WebSocketsServer      m_WebSocketServer;

};

#endif /* _WEB_SOCKET_SERVER_HPP_ */
