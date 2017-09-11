#include "ConnectionMgr.hpp"
#include "Config.hpp"
#include "WiFiConnection.hpp"
#include "APConnection.hpp"
#include "DualConnection.hpp"
#include "CommandInterpreter.hpp"
#include "Log.hpp"
#include "IMQTTService.hpp"
#include "MQTTServer.hpp"
#include "MQTTClient.hpp"
#include "MQTTClientAsync.hpp"

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
 , m_MqTTType(Config::get()->getCM_MQTTTYPE())
 , m_changeMqTTType(MQTT_NONE)
 , m_ChangeMqTTRequest(false)
 , m_Connection(NULL)
 , m_WebServer(Config::get()->getWS_PORT())
 , m_MqTTService(NULL)
{
  CommandInterpreter::get()->addCommand(std::string("contype"), std::bind(&ConnectionMgr::contypeCommand, this, std::placeholders::_1));

  changeConnection(m_ConnectionType);
  changeMqTTType(m_MqTTType);
}

ConnectionMgr::~ConnectionMgr()
{
  CommandInterpreter::get()->deleteCommand(std::string("contype"));
  delete(m_Connection);
  delete(m_MqTTService);
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
      m_Connection = new APConnection(config->getAP_SSID(), config->getAP_PASSPHRASE(), config->getAP_IPADDRESS(), config->getAP_GATEWAY(), config->getAP_NETMASK(), config->getAP_URL());
      break;

    case CON_WIFI_CONNECTION:
      m_Connection = new WiFiConnection(config->getWC_SSID(), config->getWC_PASSPHRASE(), config->getWC_HOSTNAME(), config->getWC_IPADDRESS(), config->getWC_GATEWAY(), config->getWC_NETMASK());
      break;

    case CON_DUAL_CONNECTION:
      m_Connection = new DualConnection(config->getAP_SSID(), config->getAP_PASSPHRASE(), config->getAP_IPADDRESS(), config->getAP_GATEWAY(), config->getAP_NETMASK(), config->getAP_URL(),
                                            config->getWC_SSID(), config->getWC_PASSPHRASE(), config->getWC_HOSTNAME(), config->getWC_IPADDRESS(), config->getWC_GATEWAY(), config->getWC_NETMASK());
      break;

    case CON_NONE:
      break;

    default:
      logWarning("Requested wrong Connection type!\n");
      break;
  }
}

void ConnectionMgr::requestChangeConnection(connectionType_t connectionType)
{
  m_ChangeConnectionRequest = true;
  m_ChangeConnectionType = connectionType;
}

connectionType_t ConnectionMgr::getConnectionType()
{
  return m_ConnectionType;
}

void ConnectionMgr::changeMqTTType(mqttType_t mqttType)
{
  m_MqTTType = mqttType;

  if (m_MqTTService != NULL)
  {
    delete(m_MqTTService);
    m_MqTTService = NULL;
  }

  switch (m_MqTTType)
  {
    case MQTT_SERVER:
      m_MqTTService = new MQTTServer();
      break;

    case MQTT_CLIENT:
      m_MqTTService = new MQTTClient();
      break;

    case MQTT_CLIENT_ASYNC:
      m_MqTTService = new MQTTClientAsync();
      break;

    case MQTT_NONE:
      break;

    default:
      logWarning("Requested wrong MqTT service!\n");
      break;
  }
}

void ConnectionMgr::requestChangeMqTTType(mqttType_t mqttType)
{
  m_changeMqTTType = mqttType;
  m_ChangeMqTTRequest = true;
}

mqttType_t ConnectionMgr::getMqTTType()
{
  return m_MqTTType;
}

void ConnectionMgr::loop()
{
  if (m_Connection)
    m_Connection->loop();

  m_WebServer.loop();

  if (m_MqTTService)
    m_MqTTService->loop();

  if (m_ChangeConnectionRequest)
  {
    m_ChangeConnectionRequest = false;
    changeConnection(m_ChangeConnectionType);
  }

  if (m_ChangeMqTTRequest)
  {
    m_ChangeMqTTRequest = false;
    changeMqTTType(m_changeMqTTType);
  }
}

bool ConnectionMgr::contypeCommand(std::string &s)
{
  CommandInterpreter *CI = CommandInterpreter::get();
  uint8_t args = CI->getNumArgument(s);

  if (args == 1)
  {
    std::string arg;
    CI->getArgument(s, arg, 0);
    if (arg.compare("WIFI") == 0)
    {
      changeConnection(CON_WIFI_CONNECTION);
      return true;
    }
    else if (arg.compare("AP") == 0)
    {
      changeConnection(CON_ACCESS_POINT);
      return true;
    }
    else if (arg.compare("DUAL") == 0)
    {
      changeConnection(CON_DUAL_CONNECTION);
      return true;
    }
  }
  logWarning("Unsupported!\n");
  logWarning("Types are:\n");
  logWarning("\tAP\t- Access Point\n");
  logWarning("\tWIFI\t- Connect to a WiFi network\n");
  logWarning("\tDUAL\t- Connect to a WiFi network and open access point\n");
  return false;
}

void ConnectionMgr::reset()
{
  delete(s_Instance);
  s_Instance = NULL;
  ConnectionMgr::get();
}
