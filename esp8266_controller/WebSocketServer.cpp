#include "WebSocketServer.hpp"
#include <functional>
#include "Log.hpp"

WebSocketServer::WebSocketServer(uint16_t port)
 : m_Port(port)
 , m_WebSocketServer(m_Port)
{
  logDebug("Started WebSocketServer\n");
  m_WebSocketServer.begin();
  m_WebSocketServer.onEvent(std::bind(&WebSocketServer::webSocketEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
}

WebSocketServer::~WebSocketServer()
{
  m_WebSocketServer.disconnect();
}

void WebSocketServer::loop()
{
  static uint32_t nextTime = 0;

  if (millis() >= nextTime)
  {
    // TODO find out how to get the number? maybe store in callback at connect together with IP and/or some other info in a list
    m_WebSocketServer.sendTXT(0, "I'm Happy!");
    nextTime = millis() + 5000;
  }

  m_WebSocketServer.loop();
}

void WebSocketServer::webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
 switch(type)
 {
  case WStype_DISCONNECTED:
    logDebug("[%u] Disconnected!\n", num);
    break;

  case WStype_CONNECTED:
    {
      IPAddress ip = m_WebSocketServer.remoteIP(num);
      logDebug("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

  		// send message to client
  		m_WebSocketServer.sendTXT(num, "Connected");
    }
    break;

  case WStype_TEXT:
    logDebug("[%u] get Text: %s\n", num, payload);

    // send message to client
    // m_WebSocketServer.sendTXT(num, "message here");

    // send data to all connected clients
    // m_WebSocketServer.broadcastTXT("message here");
    break;

  case WStype_BIN:
    logDebug("[%u] get binary length: %u\n", num, length);
    hexdump(payload, length);

    // send message to client
    // m_WebSocketServer.sendBIN(num, payload, length);
    break;

  default:
    break;
  }
}
