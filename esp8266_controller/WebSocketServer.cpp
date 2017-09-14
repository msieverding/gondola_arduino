#include "WebSocketServer.hpp"
#include <functional>
#include "Log.hpp"

WebSocketServer::WebSocketServer(uint16_t port)
 : m_Port(port)
 , m_WebSocketServer(m_Port)
 , m_Gondola(Gondola::get())
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
  static uint32_t nextPing = 0;

  // Ping all clients to detect timeouts
  if (millis() > nextPing)
  {
    m_WebSocketServer.broadcastPing();
    nextPing += 1000;
  }

  m_WebSocketServer.loop();
}

void WebSocketServer::webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
 switch(type)
 {
  case WStype_DISCONNECTED:
    logDebug("[%u] Disconnected!\n", num);
    m_Gondola->deleteAnchor(num);
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
    break;

  case WStype_BIN:
  {
    logDebug("[%u] get binary length: %u\n", num, length);
    hexdump(payload, length);

    webSocketCommand_t cmd = static_cast<webSocketCommand_t>(payload[0]);

    if (cmd == WSO_C_REGISTER)
    {
      anchorInformation_t anchorInfo(num);
      b4Converter_t converter;
      uint8_t i = 1;
      converter.b[0] = payload[i++];
      converter.b[1] = payload[i++];
      converter.b[2] = payload[i++];
      converter.b[3] = payload[i++];
      anchorInfo.anchorPos.x = converter.f;

      converter.b[0] = payload[i++];
      converter.b[1] = payload[i++];
      converter.b[2] = payload[i++];
      converter.b[3] = payload[i++];
      anchorInfo.anchorPos.y = converter.f;

      converter.b[0] = payload[i++];
      converter.b[1] = payload[i++];
      converter.b[2] = payload[i++];
      converter.b[3] = payload[i++];
      anchorInfo.anchorPos.z = converter.f;

      anchorInfo.moveFunc = std::bind(&WebSocketServer::remoteAnchorMoveFunction, this, std::placeholders::_1);
      anchorInfo.initFunc = std::bind(&WebSocketServer::remoteAnchorInitFunction, this, std::placeholders::_1);
      m_Gondola->addAnchor(anchorInfo);     // Call will use initFunc

      logDebug("Client registered at position(%s)\n", anchorInfo.anchorPos.toString().c_str());
    }
    else if (cmd == WSO_C_REPORT)
    {
      m_Gondola->reportAnchorFinished(num);
    }
    break;
  }

  default:
    break;
  }
}

void WebSocketServer::remoteAnchorMoveFunction(anchorInformation_t &anchorInfo)
{
  uint8_t i = 1;
  uint8_t payload[8 + 1];

  payload[0] = WSO_S_MOVE;
  b4Converter_t converter;

  converter.f = anchorInfo.targetSpooledDistance;
  payload[i++] = converter.b[0];
  payload[i++] = converter.b[1];
  payload[i++] = converter.b[2];
  payload[i++] = converter.b[3];

  converter.u = anchorInfo.travelTime;
  payload[i++] = converter.b[0];
  payload[i++] = converter.b[1];
  payload[i++] = converter.b[2];
  payload[i++] = converter.b[3];

  m_WebSocketServer.sendBIN(anchorInfo.id, payload, i);
}

void WebSocketServer::remoteAnchorInitFunction(anchorInformation_t &anchorInfo)
{
  uint8_t i = 1;
  uint8_t payload[4 + 1];

  payload[0] = WSO_S_SPOOLED_DIST;
  b4Converter_t converter;

  converter.f = anchorInfo.spooledDistance;
  payload[i++] = converter.b[0];
  payload[i++] = converter.b[1];
  payload[i++] = converter.b[2];
  payload[i++] = converter.b[3];

  m_WebSocketServer.sendBIN(anchorInfo.id, payload, i);
}
