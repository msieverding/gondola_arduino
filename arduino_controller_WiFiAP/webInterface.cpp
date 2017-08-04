#include "config.hpp"
#include "webInterface.hpp"

static ESP8266WebServer *s_Server = NULL;

webInterface::webInterface()
 : m_Server(80)
 , m_IP()
 , m_Gateway()
 , m_Netmask()
 , m_DnsServer()
{
  // Initialize member variables from config
  m_IP.fromString(AP_IPAddress.c_str());
  m_Gateway.fromString(AP_Gateway.c_str());
  m_Netmask.fromString(AP_Netmask.c_str());
  s_Server = &m_Server;
}

void webInterface::start()
{
  boolean result = false;
  // Setup AP in WiFi variable
  if (DEBUG)
  {
    Serial.print("Setup AP with IP: ");
    Serial.print(m_IP);
  }
  if(WiFi.softAPConfig(m_IP, m_Gateway, m_Netmask))
  {
    Serial.println(" Success");
  }
  else
  {
    Serial.println(" Failed!");
  }

  if(DEBUG)
  {
    Serial.print("Start AP with Name: ");
    Serial.print(AP_Name.c_str());
    Serial.print(" and Passphrase: ");
    Serial.print(AP_Passphrase.c_str());
  }
  if(WiFi.softAP(AP_Name.c_str(), AP_Passphrase.c_str()))
  {
    Serial.println(" Success");
  }
  else
  {
    Serial.println(" Failed!");
  }

  // Setup DNS server for easy access without IP address
  // modify TTL associated  with the domain name (in seconds)
  // default is 60 seconds
  m_DnsServer.setTTL(300);
  // set which return code will be used for all other domains (e.g. sending
  // ServerFailure instead of NonExistentDomain will reduce number of queries
  // sent by clients)
  // default is DNSReplyCode::NonExistentDomain
  m_DnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);

  // start DNS server for a specific domain name
  m_DnsServer.start(53, AP_URL.c_str(), m_IP);

  // simple HTTP server to see that DNS server is working
  m_Server.onNotFound([]() {
    String message = "Can't handle request with URL: ";
    message += s_Server->uri();

    s_Server->send(200, "text/plain", message);
  });

  // Print IP Adress
  Serial.print("AP IP address: ");
  Serial.println(getIP());
  m_Server.on("/", handleRoot);
  m_Server.begin();
  Serial.println("HTTP server started");
}

IPAddress webInterface::getIP()
{
  return WiFi.softAPIP();
}

void webInterface::handleRoot()
{
  Serial.println("handleRoot");
  if(s_Server != NULL)
  {
    Serial.println(s_Server->arg("x"));
    s_Server->send(200, "text/html", "<h1>You are connected</h1>");
  }
}

void webInterface::task()
{
  m_DnsServer.processNextRequest();
  m_Server.handleClient();
}
