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
{
  m_Gondola = gondola;
  initialize();
}

WebServer::~WebServer()
{
  s_Instance = NULL;
}

bool WebServer::initialize()
{
  m_Server.on("/", handleRoot);
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

void WebServer::handleSetupWiFi()
{
  Serial.println("HandleSetupWiFi");
  ESP8266WebServer &server = s_Instance->m_Server;
  ConnectionMgr *conMgr = ConnectionMgr::get();
  std::string answer;

  uint8_t args = server.args();

  WebServer::prepareHeader(answer);

  // WiFi Connection Settings
  if (server.arg("WC_SSID").length())
    Config::setWC_SSID(std::string(server.arg("WC_SSID").c_str()));
  if (server.arg("WC_PASSPHRASE").length() && !server.arg("WC_NO_PASS").equals("true"))
    Config::setWC_PASSPHRASE(std::string(server.arg("WC_PASSPHRASE").c_str()));
  else if (server.arg("WC_NO_PASS").equals("true"))
    Config::setWC_PASSPHRASE(std::string());
  if (server.arg("WC_HOSTNAME").length())
    Config::setWC_HOSTNAME(std::string(server.arg("WC_HOSTNAME").c_str()));

  if (server.arg("AP_SSID").length())
    Config::setAP_SSID(std::string(server.arg("AP_SSID").c_str()));
  if (server.arg("AP_PASSPHRASE").length() && !server.arg("AP_NO_PASS").equals("true"))
    Config::setAP_PASSPHRASE(std::string(server.arg("AP_PASSPHRASE").c_str()));
  else if (server.arg("AP_NO_PASS").equals("true"))
    Config::setAP_PASSPHRASE(std::string());
  if (server.arg("AP_URL").length())
    Config::setAP_URL(std::string(server.arg("AP_URL").c_str()));
  if (server.arg("AP_IPADDRESS").length())
    Config::setAP_IPADDRESS(server.arg("AP_IPADDRESS"));
  if (server.arg("AP_GATEWAY").length())
    Config::setAP_GATEWAY(server.arg("AP_GATEWAY"));
  if (server.arg("AP_NETMASK").length())
    Config::setAP_NETMASK(server.arg("AP_NETMASK"));

  if (server.arg("WiFiType").equals("APConnection"))
  {
    conMgr->requestChangeConnection(ConnectionMgr::CON_ACCESS_POINT);
  }
  else if (server.arg("WiFiType").equals("WiFiConnection"))
  {
    conMgr->requestChangeConnection(ConnectionMgr::CON_WIFI_CONNECTION);
  }

  WebServer::prepareGondolaWiFiSettingPage(answer);
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
  s.append("<a href=\"/SetupWiFi\">SetupWiFi</a>");
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

void WebServer::prepareGondolaWiFiSettingPage(std::string &s)
{
  s.append("<html>");
  // Setup for WiFi
  s.append("<h1>WiFi Setup: </h1>");
  s.append("<p>Select WiFi Type</p>");
  s.append("<br>");
  s.append("<form>");

  // WiFi Connection
  s.append("<input type=\"radio\" id=\"WF\" name=\"WiFiType\" value=\"WiFiConnection\">");
  s.append("<label for=\"WF\">WiFi Client</label><br>");
  s.append("<p>WiFi Connection settings</p>");
  // SSID
  s.append("<label for=\"WC_SSID\">SSID:</label>");
  s.append("<input type=\"text\" id=\"WC_SSID\" name=\"WC_SSID\" value=\"" + Config::getWC_SSID() + "\"><br>");
  // PASSPHRASE
  s.append("<label for=\"WC_PASSPHRASE\">Passphrase:</label>");
  s.append("<input type=\"text\" id=\"WC_PASSPHRASE\" name=\"WC_PASSPHRASE\" value=\"" + Config::getWC_PASSPHRASE() + "\">");
  // NO PASSPHRASE RADIO BUTTON
  s.append("<input type=\"radio\" id=\"WF_NO_PASS\" name=\"WF_NO_PASS\" value=\"true\">");
  s.append("<label for=\"WF_NO_PASS\">No passphrase</label><br>");
  // HOSTNAME
  s.append("<label for=\"WC_HOSTNAME\">Hostname:</label>");
  s.append("<input type=\"text\" id=\"WC_HOSTNAME\" name=\"WC_HOSTNAME\" value=\"" + Config::getWC_HOSTNAME() + "\"><br><br>");

  // Access Point
  s.append("<input type=\"radio\" id=\"AP\" name=\"WiFiType\" value=\"APConnection\">");
  s.append("<label for=\"AP\">Access Point</label>");
  s.append("<p>Access Point settings</p>");
  // SSID
  s.append("<label for=\"AP_SSID\">SSID:</label>");
  s.append("<input type=\"text\" id=\"AP_SSID\" name=\"AP_SSID\" value=\"" + Config::getAP_SSID() + "\"><br>");
  // PASSPHRASE
  s.append("<label for=\"AP_PASSPHRASE\">Passphrase:</label>");
  s.append("<input type=\"text\" id=\"AP_PASSPHRASE\" name=\"AP_PASSPHRASE\" value=\"" + Config::getAP_PASSPHRASE() + "\">");
  // NO PASSPHRASE RADIO BUTTON
  s.append("<input type=\"radio\" id=\"AP_NO_PASS\" name=\"AP_NO_PASS\" value=\"true\">");
  s.append("<label for=\"AP_NO_PASS\">No passphrase</label><br>");
  // URL
  s.append("<label for=\"AP_URL\">URL:</label>");
  s.append("<input type=\"text\" id=\"AP_URL\" name=\"AP_URL\" value=\"" + Config::getAP_URL() + "\"><br>");
  // IP ADDRESS
  s.append("<label for=\"AP_IPADDRESS\">IP address:</label>");
  s.append("<input type=\"text\" id=\"AP_IPADDRESS\" name=\"AP_IPADDRESS\" value=\"" + std::string(Config::getAP_IPADDRESS().toString().c_str()) + "\"><br>");
  // Gateway
  s.append("<label for=\"AP_GATEWAY\">Gateway:</label>");
  s.append("<input type=\"text\" id=\"AP_GATEWAY\" name=\"AP_GATEWAY\" value=\"" + std::string(Config::getAP_GATEWAY().toString().c_str()) + "\"><br>");
  // Netmask
  s.append("<label for=\"AP_NETMASK\">Netmask:</label>");
  s.append("<input type=\"text\" id=\"AP_NETMASK\" name=\"AP_NETMASK\" value=\"" + std::string(Config::getAP_NETMASK().toString().c_str()) + "\"><br><br>");

  // Submit
  s.append("<button type=\"submit\">Go!</button>");
  s.append("</form>");
  s.append("</html>");
}
