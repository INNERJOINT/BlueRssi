/*
 * @Author: your name
 * @Date: 2020-12-22 09:51:35
 * @LastEditTime: 2020-12-24 17:05:00
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /Drinking_Water/include/ESP_Wifi.h
 */
#ifndef ESP_WIFI
#define ESP_WIFI
#include <Preferences.h>
#include <PubSubClient.h>
#include <WebServer.h>

#include "WiFi.h"
#include "EEPROM.h"
#include "storage.h"

//wifi
void WiFi_on();
void WiFi_off();
void WiFi_config(const char *ssid, const char *password);
bool connectionStatus(const char * host = "");

//webserver
void Web_on();
void Web_off();

//MQTT
void setMQTTServer(const char* server);
void setMQTTPort(short port);
void setMQTTUsername(const char* user);
void setMQTTPassword(const char* password);
void MQTT_config(const char * server, short port = 1883, const char * user = "", const char * password = "");
void MQTT_on();
void MQTT_off();
void MQTT_reconnect();
boolean MQTT_publish(const char * topic, const char * payload);
boolean MQTT_subscribe(const char* topic);
boolean MQTT_readLog(const char * topic, fs::FS &fs, const char * path);

extern WiFiClient WiFiclient;
extern PubSubClient MQTTclient;
extern WebServer server;
#endif