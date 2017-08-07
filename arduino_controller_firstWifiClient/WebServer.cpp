#include "WebServer.hpp"
#include "Coordinate.hpp"
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
}

bool WebServer::initialize()
{
  m_Server.on("/", handleRoot);
  m_Server.onNotFound(handleNotFound);
  // TODO
	// server.onNotFound ( handleNotFound );
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

  uint8_t args = server.args();
  if (args == 0)
  {
    WebServer::prepareGondolaMainPage(answer);
  }
  else
  {
    Serial.println("More than 0 args currently not supported");
    // TODO check for given args with hasArg and arg(name)
  }

  server.send(200, "text/html", answer.c_str());
}

void WebServer::handleNotFound()
{
  ESP8266WebServer &server = s_Instance->m_Server;
	String message = "File Not Found\n\n";
	message += "URI: ";
	message += server.uri();
	message += "\nMethod: ";
	message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server.args();
	message += "\n";

	for ( uint8_t i = 0; i < server.args(); i++ )
  {
		message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
	}

	server.send ( 404, "text/plain", message );
}

void WebServer::prepareGondolaMainPage(std::string &s)
{
  if (!s_Instance->m_Gondola)
  {
    s.append("Gondola is not initialized.");
    s.append("</html>");
    return;
  }

  s.append("Gondolas actual position: ");
  return;
  {
    Coordinate Coord = s_Instance->m_Gondola->get_position();
    char buf[20];
    // Use dtostrf since Arduino doesn't support sprintf
    s.append("<br>x= "),
    dtostrf(Coord.x, 4, 2, buf);
    s.append(buf);
    s.append("<br>y= "),
    dtostrf(Coord.y, 4, 2, buf);
    s.append(buf);
    s.append("<br>z= "),
    dtostrf(Coord.z, 4, 2, buf);
    s.append(buf);
  }

  s.append("<br><br>");
  s.append("New position:");
  s.append("<form>");
  s.append("<label for=\"x\">X:");
  s.append("<input type=\"text\" id=\"x\" name=\"x\">");
  s.append("</label>");
  s.append("<label for=\"y\">Y:");
  s.append("<input type=\"text\" id=\"y\" name=\"y\">");
  s.append("</label>");
  s.append("<label for=\"z\">Z:");
  s.append("<input type=\"text\" id=\"z\" name=\"z\">");
  s.append("</label>");
  s.append("<br><br>");
  s.append("<label for=\"speed\">Speed:");
  s.append("<input type=\"text\" id=\"speed\" name=\"speed\">");
  s.append("</label>");
  s.append("<br><br>");
  s.append("<button type=\"submit\">Move!</button>");
  s.append("</form>");
  s.append("</html>");
}
