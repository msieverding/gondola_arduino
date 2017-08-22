#include "WebServer.hpp"
#include "WebServerSlave.hpp"
#include "Log.hpp"

WebServerSlave::WebServerSlave(uint16_t port)
 : WebServer(port, false)
 , m_RegisteredAtMAster(false)
{
  Log::logDebug("Creating WebServerSlave\n");

  m_Server.on("/", std::bind(&WebServerSlave::handleRoot, this));
  m_Server.on("/SetupWiFi", std::bind(&WebServerSlave::handleSetupWiFi, this));
  m_Server.on("/SetupSystem", std::bind(&WebServerSlave::handleSetupSystem, this));
  m_Server.on("/SetAnchorTargetPos", std::bind(&WebServerSlave::handleSetAnchorTargetPos, this));

  m_Server.begin();

  m_Anchor = HardwareAnchor::get();
  if (!m_Anchor)
    Log::logWarning("HardwareAnchor not initialized");

  Log::logInfo("HTTP SlaveServer started\n");
}

WebServerSlave::~WebServerSlave()
{

}

void WebServerSlave::loop()
{
  static uint32_t nextRegisterTry = 0;
  static long lastStepsTodo = 0;

  if (!m_RegisteredAtMAster && millis() > nextRegisterTry)
  {
    Log::logDebug("Start to register at master\n");
    registerAtMaster();
    nextRegisterTry = millis() + 5000;
  }

  if (m_RegisteredAtMAster && lastStepsTodo != 0 && m_Anchor->missingSteps())
  {
    reportSpoolingFinished();
  }
  lastStepsTodo = m_Anchor->missingSteps();

  WebServer::loop();
}

void WebServerSlave::handleSetAnchorTargetPos()
{
  uint8_t changes = 0;
  float spooledDistance = 0.0f;
  float speed = 1.0f;

  m_Server.send(200);

  if (m_Server.arg("spooledDistance").length())
  {
    changes++;
    spooledDistance = m_Server.arg("spooledDistance").toFloat();
  }
  if (m_Server.arg("speed").length())
  {
    changes++;
    speed = m_Server.arg("speed").toFloat();
  }
  if (changes == 2)
  {
    HardwareAnchor::get()->setTargetSpooledDistance(spooledDistance, speed);
  }
}

void WebServerSlave::prepareHeader(std::string &s)
{
  s.append("<html>");
  s.append("<h4>Slave Server: </h4>");
  s.append("<a href=\"/\">Root</a> ");
  s.append("<a href=\"/SetupWiFi\">SetupWiFi</a> ");
  s.append("<a href=\"/SetupSystem\">SetupSystem</a> ");
  s.append("<hr>");
  s.append("</html>");
}

void WebServerSlave::registerAtMaster()
{
  WiFiClient client;
  if (!client.connect(Config::get()->getWS_MASTER_URL().c_str(), Config::get()->getWS_PORT()))
  {
    Log::logDebug("WebServerSlave::registerAtMaster::connection failed\n");
    return;
  }
  Coordinate coord = m_Anchor->getAnchorPosition();
  String url = "/addWebAnchor?x=";
  url += coord.compToString('x').c_str();
  url += "&y=";
  url += coord.compToString('y').c_str();
  url += "&z=";
  url += coord.compToString('z').c_str();
  url += "&spooledDistance=";
  url += String(m_Anchor->getTargetSpooledDistance());
  url += "&ip=";
  url += WiFi.localIP().toString();

  String request = String("GET ") + url + " HTTP/1.1\r\n" +
             "Host: " + String(Config::get()->getWS_MASTER_URL().c_str()) + "\r\n" +
             "Connection: close\r\n\r\n";
  Log::logDebug("WebServerSlave::registerAtMaster::HTTP Request:\n");
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
    Log::logDebug("Answer from registration: (Anchor ID:)");
    Log::logDebug(line.c_str());
    Log::logDebug("\n");
    m_Anchor->setID(line.toInt());
    m_RegisteredAtMAster = true;
  }
}

void WebServerSlave::reportSpoolingFinished()
{
  WiFiClient client;
  if (!client.connect(Config::get()->getWS_MASTER_URL().c_str(), Config::get()->getWS_PORT()))
  {
    Log::logDebug("WebServerSlave::sendSpoolingDistance::connection failed\n");
    return;
  }
  Coordinate coord = m_Anchor->getAnchorPosition();
  String url = "/ReportSpoolingFinished?&id=";
  url += m_Anchor->getID();
  url += "&spooledDistance=";
  url += m_Anchor->getCurrentSpooledDistance();

  String request = String("GET ") + url + " HTTP/1.1\r\n" +
             "Host: " + String(Config::get()->getWS_MASTER_URL().c_str()) + "\r\n" +
             "Connection: close\r\n\r\n";
  Log::logDebug("WebServerSlave::registerAtMaster::HTTP Request:\n");
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
    Log::logDebug("\n");
  }
}
