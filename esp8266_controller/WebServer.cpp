#include "WebServer.hpp"
#include "Coordinate.hpp"
#include "ConnectionMgr.hpp"
#include "Log.hpp"
#include "IAnchor.hpp"
#include "WebAnchor.hpp"
#include <string>
#include <functional>

WebServer::WebServer(uint16_t port, bool configureServer)
 : m_Server(port)
{
  if (configureServer)
  {
    m_Server.on("/", std::bind(&WebServer::handleRoot, this));
    m_Server.on("/Setup", std::bind(&WebServer::handleSetup, this));
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

void WebServer::handleSetup()
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


  if (m_Server.arg("WiFiType").equals("APConnection"))
  {
    conMgr->requestChangeConnection(CON_ACCESS_POINT);
    config->setCM_CONTYPE(CON_ACCESS_POINT);
  }
  else if (m_Server.arg("WiFiType").equals("WiFiConnection"))
  {
    conMgr->requestChangeConnection(CON_WIFI_CONNECTION);
    config->setCM_CONTYPE(CON_WIFI_CONNECTION);
  }
  else if (m_Server.arg("WiFiType").equals("DualConnection"))
  {
    conMgr->requestChangeConnection(CON_DUAL_CONNECTION);
    config->setCM_CONTYPE(CON_DUAL_CONNECTION);
  }
  else if (m_Server.arg("WiFiType").equals("restoreDefault"))
  {
    Config::resetConfig();
    config = Config::get();
    conMgr->requestChangeConnection(config->getCM_CONTYPE());
  }

  if (m_Server.arg("GO_MASTER").length())
  {
    bool type = (m_Server.arg("GO_MASTER").equals("Master"));
    Config::get()->setGO_MASTER(type);
    // Request new and clean connection, when type was changed
    conMgr->requestChangeConnection(config->getCM_CONTYPE());
  }
  if (m_Server.arg("GO_MASTER_URL").length())
  {
    Config::get()->setGO_MASTER_URL(std::string(m_Server.arg("GO_MASTER_URL").c_str()));
  }

  config->writeToEEPROM();

  prepareHeader(answer);
  prepareSetupPage(answer);

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
  s.append("<a href=\"/Setup\">Setup</a> ");
  s.append("<hr>");
  s.append("</html>");
}

void WebServer::prepareSetupPage(std::string &s)
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
  s.append("<input type=\"radio\" id=\"WC\" name=\"WiFiType\" value=\"WiFiConnection\" " + std::string(config->getCM_CONTYPE() == CON_WIFI_CONNECTION ? "checked" : "") + ">");
  s.append("<label for=\"WC\">WiFi Connection</label><br>");
  s.append("<input type=\"radio\" id=\"AP\" name=\"WiFiType\" value=\"APConnection\" " + std::string(config->getCM_CONTYPE() == CON_ACCESS_POINT ? "checked" : "") + ">");
  s.append("<label for=\"AP\">Access Point</label><br>");
  s.append("<input type=\"radio\" id=\"DU\" name=\"WiFiType\" value=\"DualConnection\" " + std::string(config->getCM_CONTYPE() == CON_DUAL_CONNECTION ? "checked" : "") + ">");
  s.append("<label for=\"DU\">WiFi + AP Connection.</label><br>");
  s.append("<input type=\"radio\" id=\"DE\" name=\"WiFiType\" value=\"restoreDefault\">");
  s.append("<label for=\"DE\">Back to Default</label><br>");

  // Master Slave Radio
  s.append("<h4>Master/Slave setup</h4><br>");
  s.append("<input type=\"radio\" id=\"M\" name=\"GO_MASTER\" value=\"Master\" " + std::string(config->getGO_MASTER() == true ? "checked" : "") + ">");
  s.append("<label for=\"M\">Master</label><br>");
  s.append("<input type=\"radio\" id=\"S\" name=\"GO_MASTER\" value=\"Slave\" " + std::string(config->getGO_MASTER() == false ? "checked" : "") + ">");
  s.append("<label for=\"S\">Slave</label><br>");
  // Netmask
  s.append("<label for=\"GO_MASTER_URL\">URL of Master:</label>");
  s.append("<input type=\"text\" id=\"GO_MASTER_URL\" name=\"GO_MASTER_URL\" value=\"" + config->getGO_MASTER_URL() + "\"><br><br>");

  // Submit
  s.append("<br><button type=\"submit\">Go!</button>");

  s.append("</form>");
  s.append("</html>");
}
