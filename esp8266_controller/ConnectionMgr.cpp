#include "ConnectionMgr.hpp"
#include "Config.hpp"
#include "WiFiConnection.hpp"
#include "APConnection.hpp"
#include "CommandInterpreter.hpp"
#include "Log.hpp"

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
  s_Instance = NULL;
}

bool ConnectionMgr::initConnection(WebServer *webServer)
{
  Config *config = Config::get();
  m_ConType = config->getCM_CONTYPE();
  m_WebServer = webServer;
  changeConnection(m_ConType);
}

bool ConnectionMgr::changeConnection(conType_t contype)
{
  m_ConType = contype;
  Config *config = Config::get();

  // delete an old existing connection
  if (m_Connection != NULL)
  {
    delete(m_Connection);
    m_Connection = NULL;
  }

  delay(100);

  // initialize a new connection
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

  switch(args)
  {
    case 0:
      Log::logWarning("Unsupported!\n");
      Log::logWarning("Usage: contype type\n");
      Log::logWarning("type - type of new connection\n");
      Log::logWarning("\tAP\t- Access Point\n");
      Log::logWarning("\tWIFI\t- Connect to a WiFi network\n");
      break;

    case 1:
      CI->getArgument(s, arg, 0);
      if (arg.compare("WIFI") == 0)
      {
        CM->changeConnection(CON_WIFI_CONNECTION);
      }
      else if (arg.compare("AP") == 0)
      {
        CM->changeConnection(CON_ACCESS_POINT);
      }
      else
      {
        Log::logWarning("Unsupported!\n");
        Log::logWarning("Types are:\n");
        Log::logWarning("\tAP\t- Access Point\n");
        Log::logWarning("\tWIFI\t- Connect to a WiFi network\n");
      }
      break;
  }
}
