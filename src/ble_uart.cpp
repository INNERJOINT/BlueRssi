#include "network.h"
#include "bluetooth.h"

BLEServer *pServer = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;

void (* resetFunc)(void) = 0;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();

      if (rxValue.length() > 0) {
        Serial.println("*********");
        Serial.print("Received Value: ");
        for (int i = 0; i < rxValue.length(); i++)
          Serial.print(rxValue[i]);
        Serial.println();
        Serial.println("*********");
      }

      char action[10];
      char parm1[20];
      char parm2[20];
      sscanf(rxValue.c_str(),"%[a-z]\n%s\n%s\n",action,parm1,parm2);
      if(strcmp(action, "upload") == 0){
        MQTT_readLog("outTopic", FFat, "/log");
      }
      if(strcmp(action, "setMS") == 0){
        setMQTTServer(parm1);
        MQTT_off();
        MQTT_on();
        return;
      }
      if(strcmp(action, "setMP") == 0){
        int port = atoi(parm1);
        setMQTTPort(port);
        MQTT_off();
        MQTT_on();
        return;
      }
      if(strcmp(action, "setMU") == 0){
        setMQTTUsername(parm1);
        MQTT_off();
        MQTT_on();
        return;
      }
      if(strcmp(action, "setMW") == 0){
        setMQTTPassword(parm1);
        MQTT_off();
        MQTT_on();
        return;
      }
      if(strcmp(action, "wifi") == 0){
        WiFi_config(parm1, parm2);
        return;
      }
      if(strcmp(action, "wifistatus") == 0){
        connectionStatus();
        return;
      }
      if(strcmp(action, "format") == 0){
        Preferences prefs;
        prefs.begin("Fat");
        prefs.putBool("hasFormat", false);
        prefs.end();
        resetFunc();
        return;
      }
    }
};


void initBLEUART() {
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pTxCharacteristic = pService->createCharacteristic(
										CHARACTERISTIC_UUID_TX,
										BLECharacteristic::PROPERTY_NOTIFY
									);
  pTxCharacteristic->addDescriptor(new BLE2902());
  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
											 CHARACTERISTIC_UUID_RX,
											BLECharacteristic::PROPERTY_WRITE
										);

  pRxCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void loopBLEUART() {
    if (deviceConnected) {
        pTxCharacteristic->setValue(&txValue, 1);
        pTxCharacteristic->notify();
        txValue++;
		  delay(10); // bluetooth stack will go into congestion, if too many packets are sent
	}

    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
		// do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}