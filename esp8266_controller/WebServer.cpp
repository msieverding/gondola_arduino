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
  else if(validMoveArgs(server))
  {
    Coordinate newCoordinate;
    Gondola *gondola = s_Instance->m_Gondola;

    float speed = 0.0f;
    readMoveOutArgs(server, newCoordinate, speed);
    prepareGondolaMovePage(answer, newCoordinate, speed);
    if (gondola)
      gondola->setTargetPosition(newCoordinate, speed);
  }
  else
  {
    handleNotFound();
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
  s.append("<html>");
  if (!s_Instance->m_Gondola)
  {
    s.append("Gondola is not initialized.");
    s.append("</html>");
    return;
  }

  s.append("<h1>Gondolas actual position: </h1>");
  {
    Coordinate Coord = s_Instance->m_Gondola->getCurrentPosition();
    char buf[20];
    // Use dtostrf since Arduino doesn't support sprintf
    s.append("<br>" + Coord.toString() + "<br>");
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

void WebServer::prepareGondolaMovePage(std::string &s, Coordinate &coord, float &speed)
{
  char buf[20];
  dtostrf(speed, 4, 2, buf);

  s.append("<html>");
  s.append("<h1>Move Request</h1>");
  s.append("Move Gondola to: "+ coord.toString() + " with speed ");
  s.append(buf);
  s.append("<br><hr />");
  prepareGondolaMainPage(s);
}

bool WebServer::validMoveArgs(ESP8266WebServer &server)
{
  if(!server.hasArg("x") && !server.hasArg("X"))
  {
    return false;
  }
  if(!server.hasArg("y") && !server.hasArg("Y"))
  {
    return false;
  }
  if(!server.hasArg("z") && !server.hasArg("Z"))
  {
    return false;
  }
  if(!server.hasArg("Speed") && !server.hasArg("speed") && !server.hasArg("SPEED"))
  {
    return false;
  }
  return true;
}

void WebServer::readMoveOutArgs(ESP8266WebServer &server, Coordinate &coord, float &s)
{
  // get X coordinate. Consider lower and upper case
  if(server.hasArg("x"))
  {
    coord.x = server.arg("x").toFloat();
  }
  else if(server.hasArg("X"))
  {
    coord.x = server.arg("X").toFloat();
  }
  // get Y coordinate. Consider lower and upper case
  if(server.hasArg("y"))
  {
    coord.y = server.arg("y").toFloat();
  }
  else if(server.hasArg("Y"))
  {
    coord.y = server.arg("Y").toFloat();
  }
  // get Z coordinate. Consider lower and upper case
  if(server.hasArg("z"))
  {
    coord.z = server.arg("z").toFloat();
  }
  else if(server.hasArg("Z"))
  {
    coord.z = server.arg("Z").toFloat();
  }
  // get Speed. Consider lower case, upper case and first letter in upper case
  if (server.hasArg("speed"))
  {
    s = server.arg("speed").toFloat();
  }
  else if (server.hasArg("Speed"))
  {
    s = server.arg("Speed").toFloat();
  }
  else if (server.hasArg("SPEED"))
  {
    s = server.arg("SPEED").toFloat();
  }
}
