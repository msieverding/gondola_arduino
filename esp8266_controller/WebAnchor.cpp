#include <WiFiClient.h>
#include "Config.hpp"
#include "WebAnchor.hpp"
#include "Log.hpp"

WebAnchor::WebAnchor(Coordinate anchorPos, float spooledDistance, IPAddress ip)
 : IAnchor(spooledDistance, anchorPos)
 , m_IPAddress(ip)
{

}

WebAnchor::~WebAnchor()
{

}

void WebAnchor::setTargetSpooledDistance(float targetDistance, float speed)
{
  WiFiClient client;
  m_TargetSpooledDistance = targetDistance;
  m_Speed = speed;
  if (!client.connect(m_IPAddress, Config::get()->getWS_PORT()));
  {
    Log::logDebug("connection failed");
    // TODO error handling
    return;
  }

  String url = "/SetAnchorTargetPos?id=";
  url += String(getID());
  url += "spooledDistance=";
  url += String(m_TargetSpooledDistance);
  url += "&speed=";
  url += String(m_Speed);

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
