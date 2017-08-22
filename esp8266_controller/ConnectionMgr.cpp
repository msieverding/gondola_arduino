#include "ConnectionMgr.hpp"
#include "Config.hpp"
#include "WiFiConnection.hpp"
#include "APConnection.hpp"
#include "DualConnection.hpp"
#include "CommandInterpreter.hpp"
#include "Log.hpp"
#include "WebServerMaster.hpp"
#include "WebServerSlave.hpp"

ConnectionMgr *ConnectionMgr::s_Instance = NULL;

ConnectionMgr *ConnectionMgr::get()
{
  if (s_Instance == NULL)
    s_Instance = new ConnectionMgr();

  return s_Instance;
}

ConnectionMgr::ConnectionMgr()
 : m_ConType(Config::get()->getCM_CONTYPE())
 , m_ChangeRequest(false)
 , m_ChangeContype(CON_NONE)
 , m_Connection(NULL)
 , m_WebServer(NULL)
{
  CommandInterpreter::get()->addCommand(std::string("contype"), contypeCommand);

  changeConnection(m_ConType);
}

ConnectionMgr::~ConnectionMgr()
{
  CommandInterpreter::get()->deleteCommand(std::string("contype"), contypeCommand);
  delete(m_WebServer);
  s_Instance = NULL;
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

  if (m_WebServer != NULL)
  {
    delete(m_WebServer);
    m_WebServer = NULL;
  }

  delay(100);

  Log::logDebug("ConMgr::Change::0");

  if (Config::get()->getGO_MASTER())
    m_WebServer = new WebServerMaster(Config::get()->getWS_PORT());
  else
    m_WebServer = new WebServerSlave(Config::get()->getWS_PORT());

  Log::logDebug("ConMgr::Change::1\n");

  // initialize a new connection
  switch(contype)
  {
    case CON_ACCESS_POINT:
      m_Connection = APConnection::create(m_WebServer, config->getAP_SSID(), config->getAP_PASSPHRASE(), config->getAP_IPADDRESS(), config->getAP_GATEWAY(), config->getAP_NETMASK(), config->getAP_URL());
      break;

    case CON_WIFI_CONNECTION:
      m_Connection = WiFiConnection::create(m_WebServer, config->getWC_SSID(), config->getWC_PASSPHRASE(), config->getWC_HOSTNAME(), config->getWC_IPADDRESS(), config->getWC_GATEWAY(), config->getWC_NETMASK());
      break;

    case CON_DUAL_CONNECTION:
    Log::logDebug("ConMgr::Change::2\n");
      m_Connection = DualConnection::create(m_WebServer,
                                            config->getAP_SSID(), config->getAP_PASSPHRASE(), config->getAP_IPADDRESS(), config->getAP_GATEWAY(), config->getAP_NETMASK(), config->getAP_URL(),
                                            config->getWC_SSID(), config->getWC_PASSPHRASE(), config->getWC_HOSTNAME(), config->getWC_IPADDRESS(), config->getWC_GATEWAY(), config->getWC_NETMASK());
      break;

    case CON_NONE:
      break;

    default:
      Log::logWarning("Requested wrong Connection type!");
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
      else if (arg.compare("DUAL") == 0)
      {
        CM->changeConnection(CON_DUAL_CONNECTION);
      }
      else
      {
        Log::logWarning("Unsupported!\n");
        Log::logWarning("Types are:\n");
        Log::logWarning("\tAP\t- Access Point\n");
        Log::logWarning("\tWIFI\t- Connect to a WiFi network\n");
        Log::logWarning("\tDUAL\t- Connect to a WiFi network and open access point\n");
      }
      break;
  }
}
