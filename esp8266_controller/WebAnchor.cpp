#include <WiFiClient.h>
#include "WebAnchor.hpp"
#include "Log.hpp"

WebAnchor::WebAnchor(Coordinate anchorPos, float spooledDistance, IPAddress ip)
 : m_AnchorPosition(anchorPos)
 , m_SpooledDistance(spooledDistance)
 , m_SpoolingSpeed(1.0f)
 , m_IPAddress(ip)
{

}

WebAnchor::~WebAnchor()
{

}

void WebAnchor::setTargetSpooledDistance(float distance, float speed)
{
  WiFiClient client;
  if (!client.connect(m_IPAddress, 80))
  {
    Log::logDebug("connection failed");
    return;
  }

  String url = "/setTargetSpooledDistance?spooledDistance=";
  url += String(distance);
  url += "&speed=";
  url += String(speed);

  String request = String("GET ") + url + " HTTP/1.1\r\n" +
             "Host: " + m_IPAddress.toString() + "\r\n" +
             "Connection: close\r\n\r\n";
  Log::logDebug("HTTP Request:\n");
  Log::logDebug(request.c_str());

  client.print(request);

  while(client.available() == 0)
  {
    delay(500);
    Log::logDebug(".");
  }
  Log::logDebug("\n");

  // Read all the lines of the reply from server and print them to Serial
  while(client.available())
  {
    String line = client.readStringUntil('\r');
    Log::logDebug(line.c_str());
  }
}

Coordinate WebAnchor::getAnchorPosition()
{
  return m_AnchorPosition;
}
