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
 : m_ConnectionType(Config::get()->getCM_CONNECTIONTYPE())
 , m_ChangeConnectionType(CON_NONE)
 , m_ChangeConnectionRequest(false)
 , m_ServerType(Config::get()->getWS_TYPE())
 , m_ChangeServerType(SERV_NORMAL)
 , m_ChangeServerRequest(false)
 , m_Connection(NULL)
 , m_WebServer(NULL)
{
  CommandInterpreter::get()->addCommand(std::string("contype"), contypeCommand);

  changeServerType(m_ServerType, true);
  changeConnection(m_ConnectionType);
}

ConnectionMgr::~ConnectionMgr()
{
  CommandInterpreter::get()->deleteCommand(std::string("contype"), contypeCommand);
  delete(m_WebServer);
  s_Instance = NULL;
}

void ConnectionMgr::changeConnection(connectionType_t connectionType)
{
  m_ConnectionType = connectionType;
  Config *config = Config::get();

  // delete an old existing connection
  if (m_Connection != NULL)
  {
    delete(m_Connection);
    m_Connection = NULL;
  }

  delay(100);

  // initialize a new connection
  switch(m_ConnectionType)
  {
    case CON_ACCESS_POINT:
      m_Connection = APConnection::create(config->getAP_SSID(), config->getAP_PASSPHRASE(), config->getAP_IPADDRESS(), config->getAP_GATEWAY(), config->getAP_NETMASK(), config->getAP_URL());
      break;

    case CON_WIFI_CONNECTION:
      m_Connection = WiFiConnection::create(config->getWC_SSID(), config->getWC_PASSPHRASE(), config->getWC_HOSTNAME(), config->getWC_IPADDRESS(), config->getWC_GATEWAY(), config->getWC_NETMASK());
      break;

    case CON_DUAL_CONNECTION:
      m_Connection = DualConnection::create(config->getAP_SSID(), config->getAP_PASSPHRASE(), config->getAP_IPADDRESS(), config->getAP_GATEWAY(), config->getAP_NETMASK(), config->getAP_URL(),
                                            config->getWC_SSID(), config->getWC_PASSPHRASE(), config->getWC_HOSTNAME(), config->getWC_IPADDRESS(), config->getWC_GATEWAY(), config->getWC_NETMASK());
      break;

    case CON_NONE:
      break;

    default:
      Log::logWarning("Requested wrong Connection type!\n");
      break;
  }
}

void ConnectionMgr::requestChangeConnection(connectionType_t connectionType)
{
  m_ChangeConnectionRequest = true;
  m_ChangeConnectionType = connectionType;
}

void ConnectionMgr::changeServerType(serverType_t serverType, bool force)
{
  if (m_ServerType == serverType && force == false)
  {
    return;
  }

  m_ServerType = serverType;

  if (m_WebServer != NULL)
  {
    delete(m_WebServer);
    m_WebServer = NULL;
  }

  switch (Config::get()->getWS_TYPE())
  {
    case SERV_MASTER:
      m_WebServer = new WebServerMaster(Config::get()->getWS_PORT());
      break;

    case SERV_SLAVE:
      m_WebServer = new WebServerSlave(Config::get()->getWS_PORT());
      break;

    case SERV_NORMAL:
      m_WebServer = new WebServer(Config::get()->getWS_PORT());
      break;

    case SERV_NONE:
      break;

    default:
      Log::logWarning("Requested wrong server type!\n");
      break;
  }

}

void ConnectionMgr::requestChangeServerType(serverType_t serverType)
{
  m_ChangeServerType = serverType;
  m_ChangeServerRequest = true;
}

void ConnectionMgr::loop()
{
  if (m_Connection)
    m_Connection->loop();
    
  if (m_WebServer)
    m_WebServer->loop();

  if (m_ChangeConnectionRequest)
  {
    m_ChangeConnectionRequest = false;
    changeConnection(m_ChangeConnectionType);
  }

  if (m_ChangeServerRequest)
  {
    m_ChangeServerRequest = false;
    changeServerType(m_ChangeServerType);
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
