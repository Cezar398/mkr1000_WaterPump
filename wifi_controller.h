#include <SPI.h>
#include <WiFi101.h>

class WiFiController{
private:
  int ConnectToDefinedWiFi(int &status)
  {
      int current_try = 1;
      int max_try = 3;
      if(DEV_MODE)
          {
            Serial.println();
            Serial.print("Conectare la retea definita: ");
            Serial.print(WIFI_SSID);
          }

      while(status != WL_CONNECTED && current_try <= max_try)
      {
        
        status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        
        delay(10000);
        current_try++;
      }

      if(status == WL_CONNECTED)
        return 1;

    return 0;
}

int ConnectToAllWifi(int &status)
  {
      int current_try = 1;
      int max_try = 3;

      if(DEV_MODE)
          {
            Serial.println();
            Serial.print("Incercare conectare la toate retelele: ");
         
          }

      int WSSID = WiFi.scanNetworks();

      for(int i=0;i<WSSID;i++)
      {
        if(DEV_MODE)
        {
          Serial.println();
          Serial.println(WiFi.SSID(i));
        }

        WiFi.begin(WiFi.SSID(i), WIFI_PASSWORD);

        delay(10000);
      }


      if(status == WL_CONNECTED)
        return 1;

      return 0;

  }

public:
  int status = WL_IDLE_STATUS;

  void initializare()
  {
    if(DEV_MODE)
    {
        Serial.println("*** INITIALIZARE WIFI ****");
    }

    if(!(this->ConnectToDefinedWiFi(status)))
    {
        if(DEV_MODE)
        {
          Serial.println("Conectarea la retea definita a esuat.");
        }

        //this->ConnectToAllWifi(status);
    }
    else
    {
      if(DEV_MODE)
      {
        Serial.println("Conectat la WiFi");
      }
    }
  }
};
