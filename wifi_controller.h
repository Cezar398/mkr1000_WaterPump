#include <SPI.h>
#include <WiFi101.h>

WiFiServer server(80);

class WiFiController{
private:
  int ConnectToDefinedWiFi(int &status)
  {
      int current_try = 1;
      int max_try = 3;
      int waitTime = 10000;
      if(DEV_MODE)
          {
            Serial.println();
            Serial.print("Conectare la retea definita: ");
            Serial.print(WIFI_SSID);
          }

      while(status != WL_CONNECTED && current_try <= max_try)
      {
        
        status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        
        delay(waitTime);
        current_try++;

        waitTime += 5000;
      }

      if(status == WL_CONNECTED)
        return true;

    return false;
}

bool ConnectToAllWifi(int &status)
  {
      int current_try = 1;
      int max_try = 3;
      int waitTime = 10000;

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

        delay(waitTime);

        waitTime += 5000;
      }


      if(status == WL_CONNECTED)
        return true;

      return false;

  }

void printWifiStatus() {

  // print the SSID of the network you're attached to:

  Serial.print("SSID: ");

  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:

  IPAddress ip = WiFi.localIP();

  Serial.print("IP Address: ");

  Serial.println(ip);

  // print the received signal strength:

  long rssi = WiFi.RSSI();

  Serial.print("signal strength (RSSI):");

  Serial.print(rssi);

  Serial.println(" dBm");
}

public:
  int status = WL_IDLE_STATUS;

  bool connect()
  {
    if(DEV_MODE)
    {
        Serial.println("*** INITIALIZARE WIFI ****");
    }

    if(!(this->ConnectToDefinedWiFi(status)))
    {
        if(DEV_MODE)
        {
          Serial.println();
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
     if(this->status == WL_CONNECTED)
     {
       if(DEV_MODE)
       {
          printWifiStatus();
       }
       
        server.begin();
       return true;
     }
        

    return false;
  }

  String header = "";

  
  bool BTN_START = false;

  void start_server()
  {
    WiFiClient client = server.available();

    if (client) {

      if(DEV_MODE)
      {
        Serial.println("new client");
      }
      

      // an http request ends with a blank line

      bool currentLineIsBlank = true;

      while (client.connected()) {

        if (client.available()) {

          char c = client.read();

          if(DEV_MODE)
          {
              Serial.write(c);
          }

          header = header + c;
          

          // if you've gotten to the end of the line (received a newline

          // character) and the line is blank, the http request has ended,

          // so you can send a reply

          if (c == '\n' && currentLineIsBlank) {

            // send a standard http response header

            client.println("HTTP/1.1 200 OK");

            client.println("Content-Type: text/html");

            client.println("Connection: close");  // the connection will be closed after completion of the response

            client.println();

            client.println("<!DOCTYPE HTML>");

            client.println("<html>");

            client.println("<header>");
            client.println("<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-1BmE4kWBq78iYhFldvKuhfTAU6auU8tT94WrHftjDbrCEXSU1oBoqyl2QvZ6jIW3\" crossorigin=\"anonymous\">");


            if(header.indexOf("GET /btn/start_on") >= 0)
            {
              BTN_START = true;
              digitalWrite(2, HIGH);
            }
            else if(header.indexOf("GET /btn/start_off") >= 0)
            {
              BTN_START = false;
              digitalWrite(2, LOW);
            }


            client.println("</header>");

            client.println("<body>");

            client.println("<a href='/btn/start_on'>LED ON</a>");
            client.println("<a href='/btn/start_off'>LED OFF</a>");
            client.println("</body>");

            client.println("</html>");

            break;

          }

          if (c == '\n') {

            // you're starting a new line

            currentLineIsBlank = true;

          } else if (c != '\r') {

            // you've gotten a character on the current line

            currentLineIsBlank = false;

          }

        }

      }

      // give the web browser time to receive the data

      delay(10);

      // close the connection:

      client.stop();
      header = "";
      if(DEV_MODE)
      {
          Serial.println("client disconnected");
      }
    }

  }

};
