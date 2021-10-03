#include <Preferences.h>

#include "WiFi.h"

WiFiClient WiFiclient;

void WiFi_on(){
    String ssid;
    String password;
    Preferences prefs;

    prefs.begin("WiFi");
    ssid = prefs.getString("ssid");
    password = prefs.getString("password");
    prefs.end();

    WiFi.begin(ssid.c_str(),password.c_str());
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void WiFi_off(){
    WiFi.disconnect();
    while (WiFi.status() == WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi disconnected");
}

void WiFi_config(const char *ssid, const char *password){
    Preferences prefs;
    prefs.begin("WiFi");
    prefs.putString("ssid",ssid);
    prefs.putString("password",password);
    prefs.end();

    WiFi_off();
    WiFi_on();
}

bool connectionStatus(const char * host){
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.macAddress());

    if(host == NULL){
        if (!WiFiclient.connect("http://neverssl.com/", 80)) {
            Serial.println("connection failed");
            return false;
        }
        return true;
    
    }else{
        if (!WiFiclient.connect(host, 80)) {
            Serial.println("connection failed");
            return false;
        }
        return true;
    }
}

