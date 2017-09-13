#ifndef _WEB_SOCKET_CLIENT_HPP_
#define _WEB_SOCKET_CLIENT_HPP_

#include <Arduino.h>
#include <WebSocketsClient.h>
#include "Anchor.hpp"
#include "IWebSocket.hpp"

// TODO Doc

class WebSocketClient : public IWebSocket
{
public:
  // WebSocketClient(IPAddress ip, uint16_t port);
  WebSocketClient(std::string host, uint16_t port);
  virtual ~WebSocketClient();
  virtual void loop();

private:
  void webSocketEvent(WStype_t type, uint8_t * payload, size_t length);
  void anchorReadyCallback();
  void onEventGotIP(const WiFiEventStationModeGotIP &event);

  // membervariables
  std::string             m_Host;
  uint16_t                m_Port;
  WebSocketsClient        m_WebSocketClient;
  Anchor                 *m_Anchor;
  WiFiEventHandler        m_StationGotIPHandler;     //!< Handler is called, when this station got an IP
};

#endif /* _WEB_SOCKET_CLIENT_HPP_ */
