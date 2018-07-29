
#include "weatherStation.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include "JsonListener.h"
//#include <JsonStreamingParser.h>


void weatherStation::setLanguage(String language){
  this->language = language;
}
void weatherStation::setCountry(String country){
  this->country = country;
}
void weatherStation::setCity(String city){
  this->city = city;
}

void weatherStation::setPostal(String postalCode){
  this->postalCode = postalCode;
}
void weatherStation::setURL(){
  URL = "http://api.wunderground.com/api/" + apiKey + "/conditions/q/" + postalCode + ".json";
}

String weatherStation::getURL(){
  return this->URL;
}
String weatherStation::getLanguage(){
  return this->language;
}
String weatherStation::getCountry(){
  return this->country; 
}

String weatherStation::getPostal(){
  return this->postalCode;
}
String weatherStation::getCity(){
  return this->city; 
}

