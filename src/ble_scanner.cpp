#include "bluetooth.h"
#include "storage.h"

double CalcDistByRSSI(int);

uint32_t scanTime = 1;
const double FBASELINE_RSSI_1M = 59.0;
const double CONST_FACTOR_DIST_N = 2.0;
const double CONST_MAX_DIST_RANGE = 1000.0;
BLEScan *pBLEScan;

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
        int rssi = advertisedDevice.getRSSI();
        const char *Name = advertisedDevice.getName().c_str();
        std::string AddrStr = advertisedDevice.getAddress().toString();
        double dist = CalcDistByRSSI(rssi);
        char message[100];
        if (dist <= 5.0)
        {
            if (advertisedDevice.haveName())
            {
                // device meassage
                sprintf(message, "Name: %s, MAC: %s, RSSI: %d, Dist: %.2f M\r",
                        Name,
                        AddrStr.c_str(),
                        rssi,
                        dist);
            }
            else
            {
                //unknown device meassage
                sprintf(message, "Name: unknown, MAC: %s, RSSI: %d, Dist: %.2f M\r",
                        AddrStr.c_str(),
                        rssi,
                        dist);
            }
            Serial.print("Contact__");
            Serial.print(message);
            appendFile(FFat, "/log", message);
        }
    }
};

//通过RSSI计算距离
double CalcDistByRSSI(int iRSSI)
{
    double fPower, fDist;
    int iPosiRssi = abs(iRSSI);

    fDist = 0;
    fPower = (double)(iPosiRssi - FBASELINE_RSSI_1M) / (10 * CONST_FACTOR_DIST_N);

    if (fPower > 5.0)
        return CONST_MAX_DIST_RANGE; //disapeared
    else if (fPower < -3.0)
        return 0.0; //touched
    else
    {                            //5--~-3
        fDist = pow(10, fPower); //changed to CM,in term of negi val
    }

    return fDist;
}

//初始化Scanner
void initScanner()
{
    pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
    pBLEScan->setInterval(1000);
    pBLEScan->setWindow(999);
}

//循环扫描
void loopScanner()
{
    BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
    Serial.print("Devices found: ");
    Serial.println(foundDevices.getCount());
    Serial.println("Scan done!");
    pBLEScan->clearResults(); // delete results fromBLEScan buffer to release memory
    delay(2000);
}

void initBLEDevice(const char *devicename)
{
    BLEDevice::init(devicename);
}