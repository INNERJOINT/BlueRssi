#include <Arduino.h>
#include <FFat.h>

#include "bluetooth.h"
#include "config.h"
#include "storage.h"
#include "network.h"

void setup()
{
  Serial.begin(115200);
  Serial.setDebugOutput(true); //开启调试信息

  //wifi
  //临时设置
  //WiFi账号密码
  WiFi_config("404NotFound", "Gzhu#509");
  //MQTT服务器IP
  MQTT_config("106.75.147.148");
  // wifi_on();

  //Fat初始化
  initFat();
  Serial.printf("Total space: %10u\n", FFat.totalBytes());
  Serial.printf("Free space: %10u\n", FFat.freeBytes());

  //BLE SCANNER 初始化
  initBLEDevice(DEVICE_NAME);
  initScanner();
  initBLEUART();

  Web_on();
  // MQTT_readLog("outTopic", FFat, "/log");
}

void loop()
{
  loopScanner();
  loopBLEUART();
  readLog(FFat,"/log");
  delay(10000);
  MQTT_subscribe("inTopic");
  if (!MQTTclient.connected()) {
    MQTT_reconnect();
  }
  MQTTclient.loop();
}
