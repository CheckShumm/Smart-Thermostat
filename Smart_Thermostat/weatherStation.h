
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include "JsonListener.h"
//#include "JsonStreamingParser.h"


//#ifnder weatherClient_H
//#define weatherClient_H

class weatherStation{

private:
  String language;
  String country;
  String city;
  String postalCode;
  
  const String apiKey = "4cc684f19075a4ad";
  String URL;

  HTTPClient http;
  
public:
 
  void setLanguage(String language);
  void setCountry(String country);
  void setCity(String city);
  void setURL();
  void setPostal(String postalCode);

  String getURL();
  String getLanguage();
  String getCountry(); 
  String getCity(); 
  String getPostal();
};
//#endif

