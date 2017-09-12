// /*
//   Original file by:y
//   Francesco Barone 2017(c)
//   <barone_f@yahoo.com>
//   ME PROJECT
//   mqttserver.h - Config ESP to Crumb IOT Platform
// */
//
// #ifndef MQTTSERVER_h
// #define MQTTSERVER_h
//
// #define VERSION 2
// #define PATCH 011
// // 1.0   - implementazione base
// // 2.0   - BETA TESTER
// // 2.010 - aggiunta security
// // 2.011 - fixed client lost during transmission 28.07
// // 3.0   - RELEASE
//
// #if defined(ESP8266)
//   #include <pgmspace.h>
// #else
//   #include <avr/pgmspace.h>
// #endif
//
// #include <Arduino.h>
// #include <PubSubClient.h>
// #include <ESP8266WiFi.h>
//
// #include <FS.h>
//
// #if defined(__SAM3X8E__)
//     #undef __FlashStringHelper::F(string_literal)
//     #define F(string_literal) string_literal
// #endif
//
// extern "C" {
//   #include "user_interface.h"
// }
//
// #ifdef ESP8266
// 	#include <functional>
//   #ifdef MQTT_CALLBACK_SIGNATURE
//     #undef MQTT_CALLBACK_SIGNATURE
//   #endif
//
// 	#define MQTT_CALLBACK_SIGNATURE std::function<boolean(char*, uint8_t*, uint32_t)> mqtt_callback
// #endif
//
// class ME
// {
//   public:
//
// 	ME(String MqTT_Server_InstanceID);
//
// 	//BASE
// 	void begin(HardwareSerial *param_serial);
// 	void loop();
//
// 	//LOG
// 	void setloglevel(uint8_t level);
// 	void log(uint8_t level, String payload);
//
// 	//SERVER
// 	ME& setCallback(MQTT_CALLBACK_SIGNATURE);
// 	boolean subscribe(String topic_filter, boolean CleanSession);
// 	boolean unsubscribe(String topic_filter);
// 	void stop_callback();
// 	void play_callback();
// 	boolean publish(const char* topic, const char* payload);
//   boolean publish(const char* topic, const char* payload, boolean retained);
//   boolean publish(const char* topic, const uint8_t * payload, unsigned int plength);
//   boolean publish(const char* topic, const uint8_t * payload, unsigned int plength, boolean retained);
//
//
// 	//MqTT DEF
// 	#define MAX_DATA_LEN 512
//
//   private:
//
// 	//BASE
// 	HardwareSerial *serial;
// 	String MqTT_Server_InstanceID;
//
// 	//FS
// 	boolean hasSPIFFS=false;
//
// 	struct control_packet_connect {
//
// 		boolean username_flag=false;
// 		boolean password_flag=false;
// 		String ClientID="";
// 		boolean willretain_flag=false;
// 		int willqos_flag=0x00;
// 		boolean will_flag=false;
// 		boolean cleansession_flag=false;
// 		int protocol_version=4;
// 		long keep_alive_time=0;
// 		};
//
// 	control_packet_connect C_CONNECT;
//
// 	//CONFIG
// 	void load_cfg();
//
// 	//STATE
// 	int STATE=0;
// 	int STATE_CALL=0;
// 	#define NOP 9999
// 	#define START 10
// 	#define CHECK_CONFIG 20
// 	// #define WAIT_CONFIG 30
// 	// #define CONFIG_CORRUPTED 21
// 	#define CONFIGURED 40
// 	#define MQTT_LISTEN 50
// 	#define START_SERVER 60
// 	#define MQTT_EVENT 70
// 	#define MQTT_CONNECT 80
// 	#define MQTT_SUBSCRIBE 90
// 	#define MQTT_PINGREQ 100
// 	#define MQTT_PUBLISH 110
// 	#define MQTT_PROCESS 120
// 	#define UPDATE_STATS 130
// 	#define CLEAN_FS 140
// 	#define MQTT_SRVTIME 150
// 	#define SET_SRVTIME 151
// 	#define MSG_EXPIRETIME 160
// 	#define CLEAN_EXPIRED 170
//
// 	//VAR
// 	String SERVER_INSTANCE_NAME = "NONAME";
// 	String SERVER_SERIALNUMBER = "";
// 	int MQTT_PORT = 1883;
// 	boolean slowDownInternalCallback=false;
// 	long t_slowDownInternalCallback=0;
// 	int c_slowDownInternalCallback=0;
//
// 	//TIMERS
// 	void init_timers();
// 	boolean add_timer(String id);
// 	void start_timer(String id);
// 	void fire_timer(String id);
// 	boolean trigger_timer(String id, long timeout);
// 	#define TIMERS 4
// 	String timer_id[TIMERS];
// 	long timer_ms[TIMERS];
//
// 	//LOG
// 	#define SUPER 3
// 	#define TRACE 2
// 	#define DEBUG 1
// 	#define PROD 0
// 	uint8_t LOG_LEVEL=TRACE;
//
// 	//SERVER
// 	#define MAX_SRV_CLIENTS 16
// 	#define SESSION_FREE    0
// 	#define SESSION_ALLOCATED    1
// 	#define MAX_EXPIRE_MSGTIME 20 //minutes
// 	WiFiServer *mqtt_server;
// 	WiFiClient mqtt_clients[MAX_SRV_CLIENTS];
// 	void freeSession(int idx);
// 	boolean allocSession(int c, String ClientID, long KeepAliveTime, boolean CleanSession, String UserName, String Password);
// 	boolean sendData(int idx,uint8_t DATA[], int n);
// 	boolean checkMatchTopic(String topic1, String topic2);
// 	uint8_t c;
// 	int idx;
// 	boolean event_interrupt=false;
// 	int subsid_inc=0;
// 	int msgid_inc=0;
// 	long t_check;
// 	MQTT_CALLBACK_SIGNATURE;
// 	#define MAX_LISTENER 16
// 	String topic_listener[MAX_LISTENER];
// 	boolean storeMessage(String ClientID, String topic_filter, String payload, uint8_t dup, uint8_t qos, uint8_t retain);
// 	void CleanClientSession(String CLIENTID);
// 	boolean internal_callback_paused=false;
// 	long inc_uptime=0;
// 	long uptime_ms=0;
// 	void updateConfigFiles();
//
// 	//OPTIMIZE WORKLOAD
// 	int offset_process=0;
// 	#define MAX_PROCESS 32
//
// 	//STATS
// 	long t_stats;
// 	long in_msg=0;
// 	long out_msg=0;
//
// 	#define MQTT_CHECK_TIME 30
// 	#define MQTT_STATS 60
//
// 	struct mqtt_server_session {
//
// 	  int State=SESSION_FREE;
// 	  boolean CleanSession;
// 	  String ClientID;
// 	  long KeepAliveTime;
// 	  long AliveTime;
// 	  String UserName;
// 	  String Password;
// 	  };
//
// 	mqtt_server_session SERVER_SESSION[MAX_SRV_CLIENTS];
//
// 	#define MAX_PACKET_LEN 1024
// 	uint8_t IN_PACKET[MAX_PACKET_LEN];
//
// 	//MQTT
// 	#define MQTTCONNECT     1 << 4  // Client request to connect to Server
// 	#define MQTTCONNACK     2 << 4  // Connect Acknowledgment
// 	#define MQTTPUBLISH     3 << 4  // Publish message
// 	#define MQTTPUBACK      4 << 4  // Publish Acknowledgment
// 	#define MQTTPUBREC      5 << 4  // Publish Received (assured delivery part 1)
// 	#define MQTTPUBREL      6 << 4  // Publish Release (assured delivery part 2)
// 	#define MQTTPUBCOMP     7 << 4  // Publish Complete (assured delivery part 3)
// 	#define MQTTSUBSCRIBE   8 << 4  // Client Subscribe request
// 	#define MQTTSUBACK      9 << 4  // Subscribe Acknowledgment
// 	#define MQTTUNSUBSCRIBE 10 << 4 // Client Unsubscribe request
// 	#define MQTTUNSUBACK    11 << 4 // Unsubscribe Acknowledgment
// 	#define MQTTPINGREQ     12 << 4 // PING Request
// 	#define MQTTPINGRESP    13 << 4 // PING Response
// 	#define MQTTDISCONNECT  14 << 4 // Client is Disconnecting
// 	#define MQTTReserved    15 << 4 // Reserved
// };
//
// #endif
