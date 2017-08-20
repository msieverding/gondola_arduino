#include "WebServer.hpp"
#include "Coordinate.hpp"
#include "ConnectionMgr.hpp"

extern "C" {
  #include <stdio.h>
}

WebServer* WebServer::s_Instance = NULL;

WebServer* WebServer::create(uint16_t port, Gondola *gondola)
{
  if (!WebServer::s_Instance)
  {
    WebServer::s_Instance = new WebServer(port, gondola);
  }
  return s_Instance;
}

WebServer::WebServer(uint16_t port, Gondola *gondola)
 : m_Server(port)
 , m_Gondola(gondola)
{
  initialize();
}

WebServer::~WebServer()
{
  s_Instance = NULL;
}

bool WebServer::initialize()
{
  m_Server.on("/", handleRoot);
  m_Server.on("/initGondola", handleInitGondola);
  m_Server.on("/SetupWiFi", handleSetupWiFi);
  m_Server.on("/setupwifi", handleSetupWiFi);
  m_Server.onNotFound(handleNotFound);
	m_Server.begin();
  Serial.println("HTTP server started");
}

void WebServer::loop()
{
  m_Server.handleClient();
}

void WebServer::handleRoot()
{
  Serial.println("HandleRoot");
  ESP8266WebServer &server = s_Instance->m_Server;
  std::string answer;
  WebServer::prepareHeader(answer);

  uint8_t args = server.args();
  if (args == 0)
  {
    WebServer::prepareMainPage(answer);
  }
  else
  {
    Coordinate newCoordinate;
    float speed = 0.0f;
    Gondola *gondola = s_Instance->m_Gondola;

    readOutMoveArgs(server, newCoordinate, speed);
    prepareGondolaMovePage(answer, newCoordinate, speed);
    if (gondola)
      gondola->setTargetPosition(newCoordinate, speed);
  }

  server.send(200, "text/html", answer.c_str());
}

void WebServer::handleInitGondola()
{
  Serial.println("handleInitGondola");
  ESP8266WebServer &server = s_Instance->m_Server;
  std::string answer;
  Gondola *gondola = s_Instance->m_Gondola;
  uint8_t args = server.args();
  bool changed = false;

  if (!gondola)
  {
    server.send(404, "text/plain", "FATAL ERROR: Gondola not initialized");
  }

  WebServer::prepareHeader(answer);

  Coordinate Coord = gondola->getCurrentPosition();

  if (server.arg("x").length())
  {
    Coord.x = server.arg("x").toFloat();
    changed = true;
  }
  if (server.arg("y").length())
  {
    Coord.y = server.arg("y").toFloat();
    changed = true;
  }
  if (server.arg("z").length())
  {
    Coord.z = server.arg("z").toFloat();
    changed = true;
  }

  if (changed)
    gondola->setInitialPosition(Coord);

  prepareGondolaInitPage(answer, *gondola);

  server.send(200, "text/html", answer.c_str());
}

void WebServer::handleSetupWiFi()
{
  Serial.println("HandleSetupWiFi");
  ESP8266WebServer &server = s_Instance->m_Server;
  ConnectionMgr *conMgr = ConnectionMgr::get();
  Config *config = Config::get();
  std::string answer;

  uint8_t args = server.args();

  WebServer::prepareHeader(answer);

  // WiFi Connection Settings
  if (server.arg("WC_SSID").length())
    config->setWC_SSID(std::string(server.arg("WC_SSID").c_str()));
  if (server.arg("WC_PASSPHRASE").length() && !server.arg("WC_NO_PASS").equals("true"))
    config->setWC_PASSPHRASE(std::string(server.arg("WC_PASSPHRASE").c_str()));
  else if (server.arg("WC_NO_PASS").equals("true"))
    config->setWC_PASSPHRASE(std::string());
  if (server.arg("WC_HOSTNAME").length())
    config->setWC_HOSTNAME(std::string(server.arg("WC_HOSTNAME").c_str()));
  if (server.arg("WC_IPADDRESS").length())
    config->setWC_IPADDRESS(server.arg("WC_IPADDRESS"));
  if (server.arg("WC_GATEWAY").length())
    config->setWC_GATEWAY(server.arg("WC_GATEWAY"));
  if (server.arg("WC_NETMASK").length())
    config->setWC_NETMASK(server.arg("WC_NETMASK"));

  if (server.arg("AP_SSID").length())
    config->setAP_SSID(std::string(server.arg("AP_SSID").c_str()));
  if (server.arg("AP_PASSPHRASE").length() && !server.arg("AP_NO_PASS").equals("true"))
    config->setAP_PASSPHRASE(std::string(server.arg("AP_PASSPHRASE").c_str()));
  else if (server.arg("AP_NO_PASS").equals("true"))
    config->setAP_PASSPHRASE(std::string());
  if (server.arg("AP_URL").length())
    config->setAP_URL(std::string(server.arg("AP_URL").c_str()));
  if (server.arg("AP_IPADDRESS").length())
    config->setAP_IPADDRESS(server.arg("AP_IPADDRESS"));
  if (server.arg("AP_GATEWAY").length())
    config->setAP_GATEWAY(server.arg("AP_GATEWAY"));
  if (server.arg("AP_NETMASK").length())
    config->setAP_NETMASK(server.arg("AP_NETMASK"));


  if (server.arg("WiFiType").equals("APConnection"))
  {
    conMgr->requestChangeConnection(CON_ACCESS_POINT);
    config->setCM_CONTYPE(CON_ACCESS_POINT);
  }
  else if (server.arg("WiFiType").equals("WiFiConnection"))
  {
    conMgr->requestChangeConnection(CON_WIFI_CONNECTION);
    config->setCM_CONTYPE(CON_WIFI_CONNECTION);
  }
  else if (server.arg("WiFiType").equals("restoreDefault"))
  {
    Config::resetConfig();
    config = Config::get();
    conMgr->requestChangeConnection(config->getCM_CONTYPE());
  }

  config->writeToEEPROM();

  WebServer::prepareWiFiSetupPage(answer);
  server.send(200, "text/html", answer.c_str());
}

void WebServer::handleNotFound()
{
  ESP8266WebServer &server = s_Instance->m_Server;
  std::string header;
  prepareHeader(header);
	String message = header.c_str();
  message += "<br><p>";
  message += "File Not Found<br><br>";
	message += "URI: ";
	message += server.uri();
	message += "<br>Method: ";
	message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
	message += "<br>Arguments: ";
	message += server.args();
	message += "<br>";

	for ( uint8_t i = 0; i < server.args(); i++ )
  {
		message += " " + server.argName(i) + ": " + server.arg(i) + "<br>";
	}

	server.send(404, "text/html", message);
}

void WebServer::prepareHeader(std::string &s)
{
  s.append("<html>");
  s.append("<a href=\"/\">Move</a> ");
  s.append("<a href=\"/SetupWiFi\">SetupWiFi</a> ");
  s.append("<a href=\"/initGondola\">InitGondola</a> ");
  s.append("<hr>");
  s.append("</html>");
}

void WebServer::prepareMainPage(std::string &s)
{
  s.append("<html>");

  if (!s_Instance->m_Gondola)
  {
    s.append("Gondola is not initialized.");
    s.append("</html>");
    return;
  }
  Coordinate Coord = s_Instance->m_Gondola->getCurrentPosition();

  s.append("<br><br>");
  s.append("New position:");
  s.append("<form>");
  s.append("<label for=\"x\">X:");
  s.append("<input type=\"text\" id=\"x\" name=\"x\" value=\"" + Coord.compToString('x') + "\">");
  s.append("</label>");
  s.append("<label for=\"y\">Y:");
  s.append("<input type=\"text\" id=\"y\" name=\"y\" value=\"" + Coord.compToString('y') + "\">");
  s.append("</label>");
  s.append("<label for=\"z\">Z:");
  s.append("<input type=\"text\" id=\"z\" name=\"z\" value=\"" + Coord.compToString('z') + "\">");
  s.append("</label>");
  s.append("<br><br>");
  s.append("<label for=\"speed\">Speed:");
  s.append("<input type=\"text\" id=\"speed\" name=\"speed\" value=\"1.0\">");
  s.append("</label>");
  s.append("<br><br>");
  s.append("<button type=\"submit\">Move!</button>");
  s.append("</form>");
  s.append("</html>");
}

void WebServer::prepareGondolaMovePage(std::string &s, Coordinate &coord, float &speed)
{
  char buf[20];
  dtostrf(speed, 4, 2, buf);

  s.append("<html>");
  s.append("<h1>Move Request</h1>");
  s.append("Move Gondola to: "+ coord.toString() + " with speed ");
  s.append(buf);
  s.append("<br><hr />");
  prepareMainPage(s);
}

void WebServer::prepareGondolaInitPage(std::string &s, Gondola &gondola)
{
  Coordinate Coord = gondola.getCurrentPosition();

  s.append("<html><h1>Set Gondolas initial position</h1>");
  s.append("<form>");
  s.append("<label for=\"x\">X:");
  s.append("<input type=\"text\" id=\"x\" name=\"x\" value=\"" + Coord.compToString('x') + "\">");
  s.append("</label><br>");
  s.append("<label for=\"y\">Y:");
  s.append("<input type=\"text\" id=\"y\" name=\"y\" value=\"" + Coord.compToString('y') + "\">");
  s.append("</label><br>");
  s.append("<label for=\"z\">Z:");
  s.append("<input type=\"text\" id=\"z\" name=\"z\" value=\"" + Coord.compToString('z') + "\">");
  s.append("</label><br>");
  s.append("<br><br>");
  s.append("<button type=\"submit\">Set!</button>");
  s.append("</form>");
  s.append("</html>");
}

void WebServer::readOutMoveArgs(ESP8266WebServer &server, Coordinate &coord, float &s)
{
  // get X coordinate. Consider lower and upper case
  if(server.arg("x").length())
  {
    coord.x = server.arg("x").toFloat();
  }
  else if(server.arg("X").length())
  {
    coord.x = server.arg("X").toFloat();
  }
  // get Y coordinate. Consider lower and upper case
  if(server.arg("y").length())
  {
    coord.y = server.arg("y").toFloat();
  }
  else if(server.arg("Y").length())
  {
    coord.y = server.arg("Y").toFloat();
  }
  // get Z coordinate. Consider lower and upper case
  if(server.arg("z").length())
  {
    coord.z = server.arg("z").toFloat();
  }
  else if(server.arg("Z").length())
  {
    coord.z = server.arg("Z").toFloat();
  }
  // get Speed. Consider lower case, upper case and first letter in upper case
  if (server.arg("speed").length())
  {
    s = server.arg("speed").toFloat();
  }
  else if (server.arg("Speed").length())
  {
    s = server.arg("Speed").toFloat();
  }
  else if (server.arg("SPEED").length())
  {
    s = server.arg("SPEED").toFloat();
  }
}

void WebServer::prepareWiFiSetupPage(std::string &s)
{
  Config *config = Config::get();

  s.append("<html>");
  // Setup for WiFi
  s.append("<h1>WiFi Setup: </h1>");
  s.append("<form>");

  // WiFi Connection
  s.append("<h4>WiFi Connection settings</h4>");
  // SSID
  s.append("<label for=\"WC_SSID\">SSID:</label>");
  s.append("<input type=\"text\" id=\"WC_SSID\" name=\"WC_SSID\" value=\"" + config->getWC_SSID() + "\"><br>");
  // PASSPHRASE
  s.append("<label for=\"WC_PASSPHRASE\">Passphrase:</label>");
  s.append("<input type=\"text\" id=\"WC_PASSPHRASE\" name=\"WC_PASSPHRASE\" value=\"" + config->getWC_PASSPHRASE() + "\">");
  // NO PASSPHRASE RADIO BUTTON
  s.append("<input type=\"radio\" id=\"WC_NO_PASS\" name=\"WC_NO_PASS\" value=\"true\">");
  s.append("<label for=\"WC_NO_PASS\">No passphrase</label><br>");
  // HOSTNAME
  s.append("<label for=\"WC_HOSTNAME\">Hostname:</label>");
  s.append("<input type=\"text\" id=\"WC_HOSTNAME\" name=\"WC_HOSTNAME\" value=\"" + config->getWC_HOSTNAME() + "\"><br>");

  s.append("<p>Use 0.0.0.0 for IP, GW and NM to use DHCP</p>");
  // IP ADDRESS
  s.append("<label for=\"WC_IPADDRESS\">IP address:</label>");
  s.append("<input type=\"text\" id=\"WC_IPADDRESS\" name=\"WC_IPADDRESS\" value=\"" + std::string(config->getWC_IPADDRESS().toString().c_str()) + "\"><br>");
  // Gateway
  s.append("<label for=\"WC_GATEWAY\">Gateway:</label>");
  s.append("<input type=\"text\" id=\"WC_GATEWAY\" name=\"WC_GATEWAY\" value=\"" + std::string(config->getWC_GATEWAY().toString().c_str()) + "\"><br>");
  // Netmask
  s.append("<label for=\"WC_NETMASK\">Netmask:</label>");
  s.append("<input type=\"text\" id=\"WC_NETMASK\" name=\"WC_NETMASK\" value=\"" + std::string(config->getWC_NETMASK().toString().c_str()) + "\"><br><br>");

  // Access Point
  s.append("<h4>Access Point settings</h4>");
  // SSID
  s.append("<label for=\"AP_SSID\">SSID:</label>");
  s.append("<input type=\"text\" id=\"AP_SSID\" name=\"AP_SSID\" value=\"" + config->getAP_SSID() + "\"><br>");
  // PASSPHRASE
  s.append("<label for=\"AP_PASSPHRASE\">Passphrase:</label>");
  s.append("<input type=\"text\" id=\"AP_PASSPHRASE\" name=\"AP_PASSPHRASE\" value=\"" + config->getAP_PASSPHRASE() + "\">");
  // NO PASSPHRASE RADIO BUTTON
  s.append("<input type=\"radio\" id=\"AP_NO_PASS\" name=\"AP_NO_PASS\" value=\"true\">");
  s.append("<label for=\"AP_NO_PASS\">No passphrase</label><br>");
  // URL
  s.append("<label for=\"AP_URL\">URL:</label>");
  s.append("<input type=\"text\" id=\"AP_URL\" name=\"AP_URL\" value=\"" + config->getAP_URL() + "\"><br>");
  // IP ADDRESS
  s.append("<label for=\"AP_IPADDRESS\">IP address:</label>");
  s.append("<input type=\"text\" id=\"AP_IPADDRESS\" name=\"AP_IPADDRESS\" value=\"" + std::string(config->getAP_IPADDRESS().toString().c_str()) + "\"><br>");
  // Gateway
  s.append("<label for=\"AP_GATEWAY\">Gateway:</label>");
  s.append("<input type=\"text\" id=\"AP_GATEWAY\" name=\"AP_GATEWAY\" value=\"" + std::string(config->getAP_GATEWAY().toString().c_str()) + "\"><br>");
  // Netmask
  s.append("<label for=\"AP_NETMASK\">Netmask:</label>");
  s.append("<input type=\"text\" id=\"AP_NETMASK\" name=\"AP_NETMASK\" value=\"" + std::string(config->getAP_NETMASK().toString().c_str()) + "\"><br><br>");

  // WiFi Type
  s.append("<h4>Select WiFi Type</h4>");
  s.append("<input type=\"radio\" id=\"DE\" name=\"WiFiType\" value=\"restoreDefault\">");
  s.append("<label for=\"DE\">Back to Default</label><br>");
  s.append("<input type=\"radio\" id=\"AP\" name=\"WiFiType\" value=\"APConnection\" " + std::string(config->getCM_CONTYPE() == CON_ACCESS_POINT ? "checked" : "") + ">");
  s.append("<label for=\"AP\">Access Point</label><br>");
  s.append("<input type=\"radio\" id=\"WC\" name=\"WiFiType\" value=\"WiFiConnection\" " + std::string(config->getCM_CONTYPE() == CON_WIFI_CONNECTION ? "checked" : "") + ">");
  s.append("<label for=\"WC\">WiFi Client</label><br>");
  // Submit
  s.append("<br><button type=\"submit\">Go!</button>");

  s.append("</form>");
  s.append("</html>");
}
