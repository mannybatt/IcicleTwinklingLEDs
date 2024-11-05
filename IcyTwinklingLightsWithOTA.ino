



/**
 * 
 *  An early and simple LED effect that replicates the feel of sparkling icicles!
 *    -Manny Batt
 * 
 */




// ***************************************
// ********** Global Variables ***********
// ***************************************


//Globals for Wifi Setup and OTA
#include <credentials.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

//WiFi Credentials
#ifndef STASSID
#define STASSID "your_ssid"
#endif
#ifndef STAPSK
#define STAPSK  "your_password"
#endif
const char* ssid = STASSID;
const char* password = STAPSK;

//Neopixel
#include <Adafruit_NeoPixel.h>
#define PIN D2
#define Pixels 23
Adafruit_NeoPixel strip = Adafruit_NeoPixel(Pixels, PIN, NEO_GRB + NEO_KHZ800);
float redStates[Pixels];
float blueStates[Pixels];
float greenStates[Pixels];
float fadeRate = 0.96;




// ***************************************
// *************** Setup *****************
// ***************************************


void setup() {

  //Initialize Serial
  Serial.begin(115200);
  Serial.println("Booting");

  //WiFi Initialization
  wifiSetup();

  //Start Neopixel
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  for(uint16_t l = 0; l < Pixels; l++) {
    redStates[l] = 0;
    greenStates[l] = 0;
    blueStates[l] = 0;
  }
}




// ***************************************
// ************* Da Loop *****************
// ***************************************


void loop () {
  ArduinoOTA.handle();
  
   if (random(20) == 1) {
      uint16_t i = random(Pixels);
      if (redStates[i] < 1 && greenStates[i] < 1 && blueStates[i] < 1) {
        redStates[i] = 255;
        greenStates[i] = 255;
        blueStates[i] = 255;
      }
    }
    
    for(uint16_t l = 0; l < Pixels; l++) {
      if (redStates[l] > 1 || greenStates[l] > 1 || blueStates[l] > 1) {
        strip.setPixelColor(l, redStates[l], greenStates[l], blueStates[l]);
        
        if (redStates[l] > 1) {
          redStates[l] = redStates[l] * fadeRate;
        } else {
          redStates[l] = 0;
        }
        
        if (greenStates[l] > 1) {
          greenStates[l] = greenStates[l] * fadeRate;
        } else {
          greenStates[l] = 0;
        }
        
        if (blueStates[l] > 1) {
          blueStates[l] = blueStates[l] * fadeRate;
        } else {
          blueStates[l] = 0;
        }
        
      } else {
        strip.setPixelColor(l, 33,28,40);
      }
    }
    strip.show();
    delay(4);
}




// ***************************************
// ********** Backbone Methods ***********
// ***************************************


void wifiSetup() {

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  ArduinoOTA.setHostname("IcyTwinklingLights");
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
