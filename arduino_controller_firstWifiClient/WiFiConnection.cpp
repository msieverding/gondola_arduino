#include "WiFiConnection.hpp"


WiFiConnection::WiFiConnection(WebServer *server, std::string ssid, std::string passphrase)
 : m_Server(server)
 , m_SSID(ssid)
 , m_Passphrase(passphrase)
{
  WiFi.begin(m_SSID.c_str(), m_Passphrase.c_str());

  Serial.print("\nConnect to WiFi: ");
  Serial.print(m_SSID.c_str());

  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

WiFiConnection::~WiFiConnection()
{

}
