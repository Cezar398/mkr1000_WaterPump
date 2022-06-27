#include <SHT1x.h>
/* 
  Sketch generated by the Arduino IoT Cloud Thing "Untitled"
  https://create.arduino.cc/cloud/things/e622bb9c-11c1-4ea5-a90d-810c5be06424 

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  float humidity;
  float temperature;
  int auto_off;
  bool pump_status;
  bool relay_status;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/
#define dataPin  2
#define clockPin 5
SHT1x sht1x(dataPin, clockPin);

#include "thingProperties.h"

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  pump_status = 0;
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}
int counter = 0;
int counter_resp = 0;
int mode_auto = 0;
int time_counter = 100;
int delay_val = 10;

void auto_loop()
{
  if(humidity <= 50)
  {
      analogWrite(A1, 255);
      pump_status = 1;
      relay_status = 1;
      time_counter = 20;
  }
 
  if(humidity >= 65)
  {
    analogWrite(A1, 0);
    pump_status = 0;
    relay_status = 0;
    time_counter = 200;
  }
}

void loop() {
  ArduinoCloud.update();
  
  if(counter >= time_counter)
  {
    temperature = sht1x.readTemperatureC();
  
    humidity = sht1x.readHumidity();
   
    if(WiFi.status() != WL_CONNECTED)
    {
      mode_auto = 1;
    }
    else
    {
      mode_auto = 0;
    }
    
    if(mode_auto == 1)
    {
      auto_loop();
    }
    
    counter = 0;
  }
  
  
  if(counter_resp >= 60)
  {
    if(pump_status == 1)
    {
      analogWrite(A1, 0);
      pump_status = 0;
      relay_status = 0;
    }
    
    counter_resp = 0;
  }
  
  counter_resp++;
  
 // Serial.println(WL_CONNECTED);
  Serial.println(WiFi.status());
  counter++;
  delay(delay_val);
  
}

/*
  Since Temperature is READ_WRITE variable, onTemperatureChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onTemperatureChange()  {
  // Add your code here to act upon Temperature change
}

/*
  Since RelayStatus is READ_WRITE variable, onRelayStatusChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onRelayStatusChange()  {
  if(mode_auto == 0)
  {
    if(relay_status)
    {
        analogWrite(A1, 255);
        pump_status = 1;
        counter_resp = 0;
    }
    else
    {
        analogWrite(A1, 0);
        pump_status = 0;
    }
  }
}

/*
  Since Humidity is READ_WRITE variable, onHumidityChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onHumidityChange()  {
  // Add your code here to act upon Humidity change
}

/*
  Since PumpStatus is READ_WRITE variable, onPumpStatusChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onPumpStatusChange()  {
  // Add your code here to act upon PumpStatus change
}




/*
  Since AutoOff is READ_WRITE variable, onAutoOffChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onAutoOffChange()  {
  // Add your code here to act upon AutoOff change
}
