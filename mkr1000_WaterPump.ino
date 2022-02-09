#include "secrets.h"
#include "wifi_controller.h"

void setup() {

  if(DEV_MODE)
  {
    Serial.begin(9600);

    while(!Serial){;}
  }
  


  WiFiController WiFiC;

  WiFiC.initializare();

}

void loop() {
  

}
