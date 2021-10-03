/*
 * @Author: your name
 * @Date: 2020-12-23 10:11:29
 * @LastEditTime: 2020-12-26 19:40:56
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /Drinking_Water/src/ESP_BOTTLE.cpp
 */
#include "network.h"
#include "storage.h"

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setMQTTServer(const char* server){
    Preferences prefs;
    prefs.begin("MQTT");
    prefs.putString("server",server);
    prefs.end();
}

void setMQTTPort(short port){
    Preferences prefs;
    prefs.begin("MQTT");
    prefs.putShort("port",port);
    prefs.end();
}

void setMQTTUsername(const char* user){
    Preferences prefs;
    prefs.begin("MQTT");
    prefs.putString("user",user);
    prefs.end();
}

void setMQTTPassword(const char* password){
    Preferences prefs;
    prefs.begin("MQTT");
    prefs.putString("password",password);
    prefs.end();
}

void MQTT_config(const char * server, short port, const char * user, const char * password){
    Preferences prefs;
    prefs.begin("MQTT");
    prefs.putString("server",server);
    prefs.putShort("port",port);
    prefs.putString("user",user);
    prefs.putString("password",password);
    prefs.end();

    MQTT_on();
}

void MQTT_on(){
    Preferences prefs;

    prefs.begin("MQTT");
    String server = prefs.getString("server");
    short port = prefs.getShort("port");
    MQTTclient.setServer(server.c_str(), port);
    
    String CilentID = "ESP32"+WiFi.macAddress();
    String user = prefs.getString("user");
    String password = prefs.getString("password");
    prefs.end();
    MQTTclient.setCallback(callback);
    if(user.isEmpty() || password.isEmpty()){
        MQTTclient.connect(CilentID.c_str());
    }else{
        MQTTclient.connect(CilentID.c_str(), user.c_str(), password.c_str());
    }
}

void MQTT_off(){
    MQTTclient.disconnect();
}

void MQTT_reconnect() {
  while (!MQTTclient.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (MQTTclient.connect("arduinoClient")) {
      Serial.println("connected");
      MQTTclient.publish("outTopic","hello world");
      MQTTclient.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(MQTTclient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

boolean MQTT_publish(const char * topic, const char * payload){
    return MQTTclient.publish(topic, payload);
}

boolean MQTT_subscribe(const char* topic){
    return MQTTclient.subscribe(topic);
}

boolean MQTT_readLog(const char * topic, fs::FS &fs, const char * path){
    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open log for reading");
        return false;
    }
    while(file.available()){
        String rec;
        rec = file.readStringUntil('\r');
        if(rec == ""){
            continue;
        }
        MQTT_publish(topic, rec.c_str());
    }
    file.close();
    return true;
}

PubSubClient MQTTclient(WiFiclient);
