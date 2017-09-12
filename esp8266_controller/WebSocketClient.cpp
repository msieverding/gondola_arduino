#include "WebSocketClient.hpp"
#include <functional>
#include "Log.hpp"

WebSocketClient::WebSocketClient(std::string host, uint16_t port)
 : m_Host(host)
 , m_Port(port)
 , m_WebSocketClient()
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
}

WebSocketClient::~WebSocketClient()
{
  m_WebSocketClient.disconnect();
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
		}
			break;

		case WStype_TEXT:
			logDebug("[WSc] get text: %s\n", payload);

			// send message to server
			// webSocket.sendTXT("message here");
			break;

		case WStype_BIN:
			logDebug("[WSc] get binary length: %u\n", length);
			hexdump(payload, length);

			// send data to server
			// webSocket.sendBIN(payload, length);
			break;

    default:
      break;
  }
}
