#include <FlashAsEEPROM.h>
#include <FlashStorage.h>
#include "secrets.h"
#include "wifi_controller.h"

WiFiController WiFiC;

void setup() {

  if(DEV_MODE)
  {
    Serial.begin(9600);

    while(!Serial){;}
  }
  
  if(!(WiFiC.connect()))
  {
    while(true)
    {
      delay(10);
    }
  }

}

void loop() {
    WiFiC.start_server();
}
