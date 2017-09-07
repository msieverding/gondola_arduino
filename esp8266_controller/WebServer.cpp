#include "WebServer.hpp"
#include "Coordinate.hpp"
#include "ConnectionMgr.hpp"
#include "Log.hpp"
#include <string>
#include <functional>

WebServer::WebServer(uint16_t port, bool configureServer)
 : m_Server(port)
{
  if (configureServer)
  {
    m_Server.on("/", std::bind(&WebServer::handleRoot, this));
    m_Server.on("/SetupWiFi", std::bind(&WebServer::handleSetupWiFi, this));
    m_Server.on("/SetupSystem", std::bind(&WebServer::handleSetupSystem, this));
    m_Server.onNotFound(std::bind(&WebServer::handleNotFound, this));
    m_Server.begin();
    Log::logInfo("HTTP Server configured and started\n");
  }
}

WebServer::~WebServer()
{

}

void WebServer::loop()
{
  m_Server.handleClient();
}

void WebServer::handleRoot()
{
  std::string answer;
  prepareHeader(answer);

  m_Server.send(200, "text/html", answer.c_str());
}

void WebServer::handleSetupWiFi()
{
  ConnectionMgr *conMgr = ConnectionMgr::get();
  Config *config = Config::get();
  std::string answer;

  // WiFi Connection Settings
  if (m_Server.arg("WC_SSID").length())
    config->setWC_SSID(std::string(m_Server.arg("WC_SSID").c_str()));
  if (m_Server.arg("WC_PASSPHRASE").length() && !m_Server.arg("WC_NO_PASS").equals("true"))
    config->setWC_PASSPHRASE(std::string(m_Server.arg("WC_PASSPHRASE").c_str()));
  else if (m_Server.arg("WC_NO_PASS").equals("true"))
    config->setWC_PASSPHRASE(std::string());
  if (m_Server.arg("WC_HOSTNAME").length())
    config->setWC_HOSTNAME(std::string(m_Server.arg("WC_HOSTNAME").c_str()));
  if (m_Server.arg("WC_IPADDRESS").length())
    config->setWC_IPADDRESS(m_Server.arg("WC_IPADDRESS"));
  if (m_Server.arg("WC_GATEWAY").length())
    config->setWC_GATEWAY(m_Server.arg("WC_GATEWAY"));
  if (m_Server.arg("WC_NETMASK").length())
    config->setWC_NETMASK(m_Server.arg("WC_NETMASK"));

  if (m_Server.arg("AP_SSID").length())
    config->setAP_SSID(std::string(m_Server.arg("AP_SSID").c_str()));
  if (m_Server.arg("AP_PASSPHRASE").length() && !m_Server.arg("AP_NO_PASS").equals("true"))
    config->setAP_PASSPHRASE(std::string(m_Server.arg("AP_PASSPHRASE").c_str()));
  else if (m_Server.arg("AP_NO_PASS").equals("true"))
    config->setAP_PASSPHRASE(std::string());
  if (m_Server.arg("AP_URL").length())
    config->setAP_URL(std::string(m_Server.arg("AP_URL").c_str()));
  if (m_Server.arg("AP_IPADDRESS").length())
    config->setAP_IPADDRESS(m_Server.arg("AP_IPADDRESS"));
  if (m_Server.arg("AP_GATEWAY").length())
    config->setAP_GATEWAY(m_Server.arg("AP_GATEWAY"));
  if (m_Server.arg("AP_NETMASK").length())
    config->setAP_NETMASK(m_Server.arg("AP_NETMASK"));


  if (m_Server.arg("WiFiType").equals("CON_ACCESS_POINT"))
  {
    conMgr->requestChangeConnection(CON_ACCESS_POINT);
    config->setCM_CONNECTIONTYPE(CON_ACCESS_POINT);
  }
  else if (m_Server.arg("WiFiType").equals("CON_WIFI_CONNECTION"))
  {
    conMgr->requestChangeConnection(CON_WIFI_CONNECTION);
    config->setCM_CONNECTIONTYPE(CON_WIFI_CONNECTION);
  }
  else if (m_Server.arg("WiFiType").equals("CON_DUAL_CONNECTION"))
  {
    conMgr->requestChangeConnection(CON_DUAL_CONNECTION);
    config->setCM_CONNECTIONTYPE(CON_DUAL_CONNECTION);
  }
  else if (m_Server.arg("WiFiType").equals("CON_RESTORE_DEFAULT"))
  {
    Config::resetConfig();
    config = Config::get();
    conMgr->requestChangeConnection(config->getCM_CONNECTIONTYPE());
  }

  config->writeToEEPROM();

  prepareHeader(answer);
  prepareSetupWiFiPage(answer);

  m_Server.send(200, "text/html", answer.c_str());
}

void WebServer::handleSetupSystem()
{
  ConnectionMgr *conMgr = ConnectionMgr::get();
  Config *config = Config::get();
  std::string answer;

  if (m_Server.arg("CM_MQTTTYPE").equals("MQTT_SERVER"))
  {
    conMgr->requestChangeMqTTType(MQTT_SERVER);
    Config::get()->setCM_MQTTTYPE(MQTT_SERVER);
  }
  else if (m_Server.arg("CM_MQTTTYPE").equals("MQTT_CLIENT"))
  {
    conMgr->requestChangeMqTTType(MQTT_CLIENT);
    Config::get()->setCM_MQTTTYPE(MQTT_CLIENT);
  }
  else if (m_Server.arg("CM_MQTTTYPE").equals("MQTT_NONE"))
  {
    conMgr->requestChangeMqTTType(MQTT_NONE);
    Config::get()->setCM_MQTTTYPE(MQTT_NONE);
  }

  config->writeToEEPROM();

  prepareHeader(answer);
  prepareSetupSystemPage(answer);

  m_Server.send(200, "text/html", answer.c_str());
}

void WebServer::handleNotFound()
{
  std::string header;
  prepareHeader(header);
	String message = header.c_str();
  message += "<br><p>File Not Found<br><br>URI:";
	message += m_Server.uri();
	message += "<br>Method: ";
	message += ( m_Server.method() == HTTP_GET ) ? "GET" : "POST";
	message += "<br>Arguments: ";
	message += m_Server.args();
	message += "<br>";

	for ( uint8_t i = 0; i < m_Server.args(); i++ )
  {
		message += " " + m_Server.argName(i) + ": " + m_Server.arg(i) + "<br>";
	}

	m_Server.send(404, "text/html", message);
}

void WebServer::prepareHeader(std::string &s)
{
  s.append("<html>");
  s.append("<a href=\"/\">Root</a> ");
  s.append("<a href=\"/SetupWiFi\">SetupWiFi</a> ");
  s.append("<a href=\"/SetupSystem\">SetupSystem</a> ");
  s.append("<hr>");
  s.append("</html>");
}

void WebServer::prepareSetupWiFiPage(std::string &s)
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
  s.append("<input type=\"radio\" id=\"WC\" name=\"WiFiType\" value=\"CON_WIFI_CONNECTION\" " + std::string(config->getCM_CONNECTIONTYPE() == CON_WIFI_CONNECTION ? "checked" : "") + ">");
  s.append("<label for=\"WC\">WiFi Connection</label><br>");
  s.append("<input type=\"radio\" id=\"AP\" name=\"WiFiType\" value=\"CON_ACCESS_POINT\" " + std::string(config->getCM_CONNECTIONTYPE() == CON_ACCESS_POINT ? "checked" : "") + ">");
  s.append("<label for=\"AP\">Access Point</label><br>");
  s.append("<input type=\"radio\" id=\"DU\" name=\"WiFiType\" value=\"CON_DUAL_CONNECTION\" " + std::string(config->getCM_CONNECTIONTYPE() == CON_DUAL_CONNECTION ? "checked" : "") + ">");
  s.append("<label for=\"DU\">WiFi + AP Connection.</label><br>");
  s.append("<input type=\"radio\" id=\"DE\" name=\"WiFiType\" value=\"CON_RESTORE_DEFAULT\">");
  s.append("<label for=\"DE\">Back to Default</label><br>");

  // Submit
  s.append("<br><button type=\"submit\">Go!</button>");
  s.append("<br>(A new connection will be established!)");

  s.append("</form>");
  s.append("</html>");
}


void WebServer::prepareSetupSystemPage(std::string &s)
{
  Config *config = Config::get();

  s.append("<html>");
  s.append("<h1>System setup</h1>");
  s.append("<form>");
  // Setup WebServer
  s.append("<h4>MQTT setup</h4>");
  // Master Slave Normal Radio
  s.append("<input type=\"radio\" id=\"S\" name=\"CM_MQTTTYPE\" value=\"MQTT_SERVER\" " + std::string(config->getCM_MQTTTYPE() == MQTT_SERVER ? "checked" : "") + ">");
  s.append("<label for=\"S\">Server</label><br>");
  s.append("<input type=\"radio\" id=\"C\" name=\"CM_MQTTTYPE\" value=\"MQTT_CLIENT\" " + std::string(config->getCM_MQTTTYPE() == MQTT_CLIENT ? "checked" : "") + ">");
  s.append("<label for=\"C\">Client</label><br>");
  s.append("<input type=\"radio\" id=\"N\" name=\"CM_MQTTTYPE\" value=\"MQTT_NONE\" " + std::string(config->getCM_MQTTTYPE() == MQTT_NONE ? "checked" : "") + ">");
  s.append("<label for=\"N\">None</label><br>");

  // Submit
  s.append("<br><button type=\"submit\">Go!</button>");
  s.append("<br>(A new servive will be started!)");

  s.append("</form>");
  s.append("</html>");
}
