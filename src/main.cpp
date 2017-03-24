#include <SPI.h> // included because Adafruit_PCD8544.h need it
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// WiFi information
const char WIFI_SSID[] = "xperia jc";
const char WIFI_PSK[] = "jacekkkk";

const char http_site[] = "http://mysterious-coast-1799.herokuapp.com/freeParkplaces";
const int http_port = 80;

Adafruit_PCD8544 display = Adafruit_PCD8544(14, 13, 12, 5, 4);

void setup()   {
  Serial.begin(115200);

  display.begin();
  display.setContrast(60);

  display.display(); // show splashscreen

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.begin(WIFI_SSID, WIFI_PSK);

      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
  }


void loop() {
    StaticJsonBuffer<1024> jsonBuffer;


     if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

       HTTPClient http;  //Declare an object of class HTTPClient

       http.begin(http_site);  //Specify request destination
       int httpCode = http.GET();                                                                  //Send the request

       if (httpCode > 0) { //Check the returning code

                    String payload = http.getString();   //Get the request response payload
                    Serial.println(payload);                     //Print the response payload

          JsonObject& root = jsonBuffer.parseObject(payload);

            // Test if parsing succeeds.
            if (root.success()) {
                long freeParkplaces = root["freeParkplaces"];
        long lastDateUpdate = root["lastDateUpdate"];

        Serial.print("freeParkplaces: ");
        Serial.println(freeParkplaces);
        Serial.print("lastDateUpdate: ");
        Serial.println(lastDateUpdate);

        display.clearDisplay();   // clears the screen and buffer

      display.setTextSize(4);
      display.setTextColor(BLACK);
      display.println(freeParkplaces, DEC);
     // display.println(lastDateUpdate, DEC);
      display.display();
            } else {
              Serial.println("parseObject() failed");
              return;
            }


       }

       http.end();   //Close connection

     }

     delay(30000);    //Send a request every 30 seconds

}
