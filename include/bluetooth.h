#ifndef BLE_UART
#define BLE_UART

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include "config.h"

void initBLEDevice(const char * devicename);
//UART
void initBLEUART();
void loopBLEUART();

//Scanner
void initScanner();
void loopScanner();
#endif