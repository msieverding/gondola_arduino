#include "ConnectionMgr.hpp"
#include "Config.hpp"
#include "WiFiConnection.hpp"
#include "APConnection.hpp"
#include "CommandInterpreter.hpp"

ConnectionMgr *ConnectionMgr::s_Instance = NULL;

ConnectionMgr *ConnectionMgr::get()
{
  if (s_Instance == NULL)
    s_Instance = new ConnectionMgr();

  return s_Instance;
}

ConnectionMgr::ConnectionMgr()
 : m_ConType(CON_NONE)
 , m_ChangeRequest(false)
 , m_ChangeContype(CON_NONE)
 , m_Connection(NULL)
 , m_WebServer(NULL)
{
  CommandInterpreter::get()->addCommand(std::string("contype"), contypeCommand);
}

ConnectionMgr::~ConnectionMgr()
{
  CommandInterpreter::get()->deleteCommand(std::string("contype"), contypeCommand);
}

bool ConnectionMgr::initConnection(WebServer *server)
{
  Config *config = Config::get();
  m_ConType = config->getCM_CONTYPE();
  m_WebServer = server;
  changeConnection(m_ConType);
}

bool ConnectionMgr::changeConnection(conType_t contype)
{
  m_ConType = contype;
  Config *config = Config::get();

  if (m_Connection != NULL)
  {
    Serial.println("delete old connection");
    delete(m_Connection);
  }

  // TODO remove delay if possible
  delay(100);

  switch(contype)
  {
    case CON_ACCESS_POINT:
      m_Connection = APConnection::create(m_WebServer, config->getAP_SSID(), config->getAP_PASSPHRASE(), config->getAP_IPADDRESS(), config->getAP_GATEWAY(), config->getAP_NETMASK(), config->getAP_URL());
      break;

    case CON_WIFI_CONNECTION:
      m_Connection = WiFiConnection::create(m_WebServer, config->getWC_SSID(), config->getWC_PASSPHRASE(), config->getWC_HOSTNAME(), config->getWC_IPADDRESS(), config->getWC_GATEWAY(), config->getWC_NETMASK());
      break;

    default:
      break;
  }
}

void ConnectionMgr::requestChangeConnection(conType_t contype)
{
  m_ChangeRequest = true;
  m_ChangeContype = contype;
}

void ConnectionMgr::loop()
{
  if (m_Connection)
    m_Connection->loop();

  if (m_ChangeRequest)
  {
    Serial.println("Change connection to: ");
    Serial.println(static_cast<int>(m_ChangeContype));
    m_ChangeRequest = false;
    changeConnection(m_ChangeContype);
  }
}

void ConnectionMgr::contypeCommand(std::string &s)
{
  CommandInterpreter *CI = CommandInterpreter::get();
  ConnectionMgr *CM = ConnectionMgr::get();
  String arduinoS(s.c_str());
  std::string arg;

  uint8_t args = CI->getNumArgument(s);
  for (uint8_t i = 0; i < args; i++)
  {
    CI->getArgument(s, arg, i);
    Serial.print("Arg ");
    Serial.print(i);
    Serial.print(" ");
    Serial.println(arg.c_str());
  }

  switch(args)
  {
    case 0:
      Serial.println("Unsupported.");
      break; // TODO make help serial output to clarify usage
    case 1:
      CI->getArgument(s, arg, 0);
      if (arg.compare("WIFI") == 0)
        CM->changeConnection(CON_WIFI_CONNECTION);
      else if (arg.compare("AP") == 0)
        CM->changeConnection(CON_ACCESS_POINT);
      else
        Serial.println("Unsupported");
      break;
  }
}
