#include "WebServer.hpp"
#include "WebServerMaster.hpp"
#include "Log.hpp"
#include "Gondola.hpp"
#include "WebAnchor.hpp"

WebServerMaster::WebServerMaster(uint16_t port)
 : WebServer(port, false)
{
  Log::logDebug("Creating WebServerMaster\n");
  m_Server.on("/", std::bind(&WebServerMaster::handleRoot, this));
  m_Server.on("/Setup", std::bind(&WebServerMaster::handleSetup, this));
  m_Server.on("/GondolaMove", std::bind(&WebServerMaster::handleGondolaMove, this));
  m_Server.on("/AddWebAnchor", std::bind(&WebServerMaster::handleAddWebAnchor, this));
  m_Server.onNotFound(std::bind(&WebServerMaster::handleNotFound, this));

  m_Server.begin();

  m_Gondola = Gondola::get();
  if (!m_Gondola)
    Log::logWarning("Gondola ist not initialized\n");
  else
    m_Gondola->addAnchor(HardwareAnchor::get());

  Log::logInfo("HTTP MasterServer started\n");
}

WebServerMaster::~WebServerMaster()
{

}

// void WebServerMaster::loop()
// {
//   m_Server.handleClient();
// }

void WebServerMaster::handleGondolaMove()
{
  std::string answer;
  prepareHeader(answer);

  if (m_Server.args() > 0)
  {
    Coordinate newCoordinate;
    float speed = 1.0f;

    if(m_Server.arg("x").length())
    newCoordinate.x = m_Server.arg("x").toFloat();
    if(m_Server.arg("y").length())
    newCoordinate.y = m_Server.arg("y").toFloat();
    if(m_Server.arg("z").length())
    newCoordinate.z = m_Server.arg("z").toFloat();
    if(m_Server.arg("speed").length())
    speed = m_Server.arg("speed").toFloat();

    if (m_Gondola)
      m_Gondola->setTargetPosition(newCoordinate, speed);
  }

  prepareGondolaMovePage(answer);
  m_Server.send(200, "text/html", answer.c_str());
}

void WebServerMaster::handleAddWebAnchor()
{
  float spooledDistance;
  Coordinate coord;
  IPAddress ip;

  m_Server.send(200);

  if (m_Server.arg("x").length())
    coord.x = m_Server.arg("x").toFloat();
  if (m_Server.arg("y").length())
    coord.y = m_Server.arg("y").toFloat();
  if (m_Server.arg("z").length())
    coord.z = m_Server.arg("z").toFloat();
  if (m_Server.arg("spooledDistance").length())
    spooledDistance = m_Server.arg("spooledDistance").toFloat();
  if (m_Server.arg("ip").length())
    ip.fromString(m_Server.arg("ip"));

  Log::logDebug("Add web anchor on: ");
  Log::logDebug(coord.toString());
  Log::logDebug(" with spooled distance: ");
  Log::logDebug(spooledDistance);
  Log::logDebug("cm and IP: ");
  Log::logDebug(ip.toString().c_str());
  Log::logDebug("\n");

  m_Gondola->addAnchor(new WebAnchor(coord, spooledDistance, ip));
}

void WebServerMaster::prepareHeader(std::string &s)
{
  s.append("<html>");
  s.append("<a href=\"/\">Root</a> ");
  s.append("<a href=\"/Setup\">Setup</a> ");
  s.append("<a href=\"/GondolaMove\">GondolaMove</a> ");
  s.append("<hr>");
  s.append("</html>");
}

void WebServerMaster::prepareGondolaMovePage(std::string &s)
{
  s.append("<html>");

  if (!m_Gondola)
  {
    s.append("Gondola is not initialized.");
    s.append("</html>");
    return;
  }
  Coordinate Coord = m_Gondola->getCurrentPosition();

  if (m_Gondola->getCurrentPosition() != m_Gondola->getTargetPosition())
  {
    s.append("<h1>Gondola is moving</h1>");
    s.append("Move to: "+ m_Gondola->getTargetPosition().toString());
    s.append("<br><hr />");
    s.append("</html>");
    return;
  }

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
