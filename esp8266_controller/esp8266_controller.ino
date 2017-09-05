// #include <ESP8266WiFi.h>
// #include <PubSubClient.h>
//
// #define wifi_ssid "MqTT"
// #define wifi_password "passw0rd"
//
// #define mqtt_server "192.168.4.1"
// #define mqtt_user "admin"
// #define mqtt_password "passw0rd"
//
// WiFiClient espClient;
// PubSubClient client(espClient);
//
// void setup()
// {
//   Serial.begin(115200);
//   setup_wifi();
//   client.setServer(mqtt_server, 1883);
// }
//
// void setup_wifi()
// {
//   delay(10);
//   // We start by connecting to a WiFi network
//   Serial.println();
//   Serial.print("Connecting to ");
//   Serial.println(wifi_ssid);
//
//   WiFi.begin(wifi_ssid, wifi_password);
//
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//
//   Serial.println("");
//   Serial.println("WiFi connected");
//   Serial.println("IP address: ");
//   Serial.println(WiFi.localIP());
// }
//
// void reconnect()
// {
//   bool userAdded = false;
//   // Loop until we're reconnected
//   while (!client.connected())
//   {
//     Serial.print("Attempting MQTT connection...");
//     if (!userAdded)
//     {
//       if (client.connect("ESP8266Admin", "admin", "passw0rd"))
//       {
//         Serial.println("connected -> Add user");
//         client.publish("/SYS/MqTT/console", "ADDUSER pippo passw0rd", true);
//         client.disconnect();
//         userAdded = true;
//       }
//       else
//       {
//         Serial.println("Couldn't connect as admin");
//       }
//     }
//     // Attempt to connect
//     // If you do not want to use a username and password, change next line to
//     // if (client.connect("ESP8266Client")) {
//     if (client.connect("ESP8266Client", "pippo", "passw0rd"))
//     {
//       Serial.println("connected");
//     }
//     else
//     {
//       Serial.print("failed, rc=");
//       Serial.print(client.state());
//       Serial.println(" try again in 5 seconds");
//       // Wait 5 seconds before retrying
//       delay(5000);
//     }
//   }
// }
//
// bool checkBound(float newValue, float prevValue, float maxDiff)
// {
//   return !isnan(newValue) &&
//          (newValue < prevValue - maxDiff || newValue > prevValue + maxDiff);
// }
//
// long lastMsg = 0;
// float temp = 0.0;
// float hum = 0.0;
// float diff = 1.0;
//
// void loop()
// {
//   if (!client.connected())
//   {
//     reconnect();
//   }
//   client.loop();
//
//   long now = millis();
//   if (now - lastMsg > 1000)
//   {
//     lastMsg = now;
//
//     float newTemp = 0.0;
//     float newHum = 1.0;
//
//     Serial.print("New temperature:");
//     Serial.println(String(hum).c_str());
//     client.publish("/test/temperature", String(temp).c_str(), true);
//   }
// }
