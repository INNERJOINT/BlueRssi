#ifndef CUS_STORAGE
#define CUS_STORAGE

#include <Preferences.h>
#include <FFat.h>
#include <WString.h>
#include "CONFIG.h"

#pragma once

// struct of Advertisment information
typedef struct
{
  uint8_t MAC;
  uint8_t device_name;
  uint8_t rssi;
  uint8_t setting2;
} ad_inform_t;

// struct of ESP init data
typedef struct
{
  uint8_t ble_name[BLE_NAME_LENGTH];
  bool wifi_enable;
  uint8_t wifi_name[WIFI_NAME_LENGTH];
  uint8_t wifi_pwd[WIFI_PWD_LENGTH];
} init_data_t;

// 
typedef struct
{
  char sTimeStmp[24];
  char sMac[18];
  char sTagName[16];
  float fDist;
  int iContcTime;    //contact time
  int iReservedPool; //used for store config infor

} NB_tag_type;

void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
void readFile(fs::FS &fs, const char * path);
void writeFile(fs::FS &fs, const char * path, const char * message);
void appendFile(fs::FS &fs, const char * path, const char * message);
void renameFile(fs::FS &fs, const char * path1, const char * path2);
void deleteFile(fs::FS &fs, const char * path);
void readLog(fs::FS &fs, const char * path);

bool hasFormat();
void initFat();

#endif