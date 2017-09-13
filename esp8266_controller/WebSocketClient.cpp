#include "WebSocketClient.hpp"
#include <functional>
#include "Log.hpp"

WebSocketClient::WebSocketClient(std::string host, uint16_t port)
 : m_Host(host)
 , m_Port(port)
 , m_WebSocketClient()
 , m_Anchor(Anchor::get())
{
  // Start when WiFi is connected
  // Event for connection changes
  m_StationGotIPHandler = WiFi.onStationModeGotIP(std::bind(&WebSocketClient::onEventGotIP, this, std::placeholders::_1));
}

WebSocketClient::~WebSocketClient()
{
  m_WebSocketClient.disconnect();
}

void WebSocketClient::onEventGotIP(const WiFiEventStationModeGotIP &event)
{
  logDebug("Started WebSocketClient\n");
  // server address, port and URL
	m_WebSocketClient.begin(m_Host.c_str(), m_Port, "/");

	// event handler
	m_WebSocketClient.onEvent(std::bind(&WebSocketClient::webSocketEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	// // use HTTP Basic Authorization this is optional remove if not needed
	// m_WebSocketClient.setAuthorization("user", "Password");

	// try ever 5000 again if connection has failed
  m_WebSocketClient.setReconnectInterval(5000);

  m_Anchor->registerReadyCallback(std::bind(&WebSocketClient::anchorReadyCallback, this));
}

void WebSocketClient::loop()
{
  m_WebSocketClient.loop();
}

void WebSocketClient::webSocketEvent(WStype_t type, uint8_t * payload, size_t length)
{
  switch(type)
  {
		case WStype_DISCONNECTED:
			logDebug("[WSc] Disconnected!\n");
			break;

		case WStype_CONNECTED:
    {
			logDebug("[WSc] Connected to url: %s\n", payload);

			// send message to server when Connected
			m_WebSocketClient.sendTXT("Connected");

      byte msg[16 + 1];
      uint8_t i = 1;
      floatConverter_t converter;

      msg[0] = WSO_C_REGISTER;

      converter.f = m_Anchor->getAnchorPos().x;
      msg[i++] = converter.b[0];
      msg[i++] = converter.b[1];
      msg[i++] = converter.b[2];
      msg[i++] = converter.b[3];

      converter.f = m_Anchor->getAnchorPos().y;
      msg[i++] = converter.b[0];
      msg[i++] = converter.b[1];
      msg[i++] = converter.b[2];
      msg[i++] = converter.b[3];

      converter.f = m_Anchor->getAnchorPos().z;
      msg[i++] = converter.b[0];
      msg[i++] = converter.b[1];
      msg[i++] = converter.b[2];
      msg[i++] = converter.b[3];

      converter.f = m_Anchor->getCurrentSpooledDistance();
      msg[i++] = converter.b[0];
      msg[i++] = converter.b[1];
      msg[i++] = converter.b[2];
      msg[i++] = converter.b[3];

      m_WebSocketClient.sendBIN(msg, i);
      break;
		}

		case WStype_TEXT:
			logDebug("[WSc] get text: %s\n", payload);
			break;

		case WStype_BIN:
    {
			logDebug("[WSc] get binary length: %u\n", length);
			hexdump(payload, length);

      webSocketCommand_t cmd = static_cast<webSocketCommand_t>(payload[0]);

      if (cmd == WSO_S_MOVE)
      {
        floatConverter_t newSpooledDistance;
        floatConverter_t speed;
        uint8_t i = 1;

        newSpooledDistance.b[0] = payload[i++];
        newSpooledDistance.b[1] = payload[i++];
        newSpooledDistance.b[2] = payload[i++];
        newSpooledDistance.b[3] = payload[i++];

        speed.b[0] = payload[i++];
        speed.b[1] = payload[i++];
        speed.b[2] = payload[i++];
        speed.b[3] = payload[i++];

        logDebug("Got new spooled distance = '%s' and speed = '%s'\n", floatToString(newSpooledDistance.f).c_str(), floatToString(speed.f).c_str());
        m_Anchor->setTargetSpooledDistance(newSpooledDistance.f, speed.f);
      }
			break;
    }

    default:
      break;
  }
}

void WebSocketClient::anchorReadyCallback()
{
  uint8_t payload = WSO_C_REPORT;
  m_WebSocketClient.sendBIN(&payload, 1);
}
