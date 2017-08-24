#include "WebServer.hpp"
#include "WebServerMaster.hpp"
#include "Log.hpp"
#include "Gondola.hpp"
#include "IAnchor.hpp"
#include "WebAnchor.hpp"

WebServerMaster::WebServerMaster(uint16_t port)
 : WebServer(port, false)
{
  Log::logDebug("Creating WebServerMaster\n");
  m_Server.on("/", std::bind(&WebServerMaster::handleRoot, this));
  m_Server.on("/SetupWiFi", std::bind(&WebServerMaster::handleSetupWiFi, this));
  m_Server.on("/SetupSystem", std::bind(&WebServerMaster::handleSetupSystem, this));
  m_Server.on("/GondolaMove", std::bind(&WebServerMaster::handleGondolaMove, this));
  m_Server.on("/AddWebAnchor", std::bind(&WebServerMaster::handleAddWebAnchor, this));
  m_Server.on("/ReportSpoolingFinished", std::bind(&WebServerMaster::handleReportSpoolingFinished, this));
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
  delete(m_Gondola);
}

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

  IAnchor *anchor = new WebAnchor(coord, spooledDistance, ip);
  m_Gondola->addAnchor(anchor);

  String answer(anchor->getID());

  m_Server.send(200, "text/plain", answer);
}

void WebServerMaster::handleReportSpoolingFinished()
{
  m_Server.send(200);

  uint8_t id = m_Server.arg("id").toInt();
  float newSpooledDistance = m_Server.arg("spooledDistance").toFloat();
  IAnchor *anchor = m_Gondola->getAnchor(id);
  anchor->setCurrentSpooledDistance(newSpooledDistance);
  m_Gondola->setAnchorReady(id, true);
}

void WebServerMaster::prepareHeader(std::string &s)
{
  s.append("<html>");
  s.append("<a href=\"/\">Root</a> ");
  s.append("<a href=\"/SetupWiFi\">SetupWiFi</a> ");
  s.append("<a href=\"/SetupSystem\">SetupSystem</a> ");
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
  }
  else
  {
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
  }

  s.append("<br><hr>");

  // Show Anchor information
  Gondola *gondola = Gondola::get();
  s.append("<h4>Gondola information</h4>");
  s.append("Number of Anchors registered: ");
  s.append(String(gondola->getNumAnchors()).c_str());

  s.append("<h4>Anchor information</h4>");

  char buf[20];

  for (uint8_t i = 0; i < gondola->getNumAnchors(); i++)
  {
    s.append("Anchor: ");
    s.append(String(i).c_str());
    s.append("<br>Spooled distance:");
    s.append(std::string(dtostrf(gondola->getAnchor(i)->getCurrentSpooledDistance(), 4, 2, buf)));
    s.append("<br>Target spooled distance:");
    s.append(std::string(dtostrf(gondola->getAnchor(i)->getTargetSpooledDistance(), 4, 2, buf)));
    s.append("<br>Speed:");
    s.append(std::string(dtostrf(gondola->getAnchor(i)->getSpeed(), 4, 2, buf)));
    s.append("<br><br>");
  }

  s.append("(Remote anchors will only update when spooling is complete.)");
  s.append("</form>");
  s.append("</html>");
}
