
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <Arduino.h>

#ifndef jsonParser_H
#define jsonParser_H

class jsonParser{

  private: 


  public:

    int findValue(String response, String condition);
    String findWord(char* response, char* word);
  
};
#endif
