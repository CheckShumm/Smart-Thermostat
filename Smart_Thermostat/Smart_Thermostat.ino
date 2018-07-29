/*
 * Smart Thermostat
 * 
 * Description: 
 * 
 * Author: Nathan Shummoogum
 * 
 */


// Including libraries

#include <ESP8266WiFi.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include "weatherStation.h"
#include "SSD1306.h"
#include <DHT.h>
#include "jsonParser.h"


// Defining pins 
#define SDA D3    // Serial Data
#define SCL D5    // Serial Clock
#define I2C 0x3C  // OLED

#define DHT_PIN 13
#define DHTTYPE DHT22

// Wifi Credentials (Temporarily Static)
char ssid[] = "BELL909";
char pass[] = "babeface";

float roomTemperature;
float roomHumidity;
float roomHeatIndex;

float localTemperature;
float localHumidity;
float localHeatIndex;

String response;

weatherStation wunder;
jsonParser parser;
const char* host = "api.wunderground.com";
//const char* host = "api.coindesk.com"; // crypto currency API
DHT dht(DHT_PIN, DHTTYPE);
SSD1306 display(I2C, SDA, SCL);
WiFiClient client;
WiFiServer Server(80);

///////////////////////////////////////////////////////////////////////////
/*
 *  Set up pins, Wifi, Display and run script in loop()
 */
///////////////////////////////////////////////////////////////////////////

void setup() {
  // Pin Setup
  pinMode(13, OUTPUT); // DHT22 Pin
  
  Serial.begin(9600);

  displaySetup();

  if(!WiFiConnect(ssid, pass)){
    // Redirect to WIFI Credentials
    // Check Network Availability
    Serial.println("Ending Process"); 
  }

  wunder.setPostal("J5R5X7");
  wunder.setURL();
    
}

/////////////////////////////////////////////////////
// MAIN LOOP
/////////////////////////////////////////////////////

void loop() {

//Main code that loops infinitely
  
  roomHumidity = dht.readHumidity();
  roomTemperature = dht.readTemperature();
  
  delay(2000);
  
  //oledDisplay(roomHumidity, roomTemperature);

  // fetch resopnse from API
  response = retrieveAPI();

  // Parse Json and retrieve data
  localTemperature = getTemperature(response);
  localHumidity = getHumidity(response);

  oledDisplay(localHumidity, localTemperature);
  
  // Retrieving Heat Indices
  roomHeatIndex = getHeatIndex(roomTemperature, roomHumidity);
  localHeatIndex = getHeatIndex(localTemperature, localHumidity);


  Serial.print("Local Humidity: ");
  Serial.println(localHumidity);

  Serial.print("Local Temperature: ");
  Serial.println(localTemperature);

  Serial.print("Local Heat Index: ");
  Serial.println(localHeatIndex);

  Serial.println("");
  
  Serial.print("Room Humidity: ");
  Serial.println(roomHumidity);

  Serial.print("Room Temperature: ");
  Serial.println(roomTemperature);
  
  Serial.print("Room Heat Index: ");
  Serial.println(roomHeatIndex);
  
  Serial.println(" ");
  Serial.println(" ");
  Serial.println(" ");

  delay(5000);
}


//////////////////////////////////////////////////////////////////////////////////
/*
 *  Functions
 */
 /////////////////////////////////////////////////////////////////////////////////

void displaySetup(){
  display.init();
  display.flipScreenVertically();
  display.clear();
  display.display();
}

bool WiFiConnect(char SSID[], char PASS[]){

  if(WiFi.status() != WL_CONNECTED){
    WiFi.disconnect();
  }
  
  delay(2000);
  Serial.println("");
  Serial.println("Begin Connection");

  WiFi.begin(SSID, PASS); 

  while( WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(" ... ");
  }

  Serial.println("");
  
  if(WiFi.status() == WL_CONNECTED){
    Serial.println("WiFi Connection Successful");
    Serial.print("Local IP Adress: ");
    Serial.println(WiFi.localIP());
    Server.begin();
    return true;
  }else{
    Serial.println("WiFi Connection Unsuccessful");
    Serial.println("Check credentials and network availability");
    return false;
  }  
}


void oledDisplay(float humidity, float temperature){
  String temperature_string = String(temperature);
  String humidity_string = String(humidity);
  //Display to OLED Monitor
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, "Temp: ");
  display.drawString(50, 0, temperature_string);

  display.drawString(0, 25, "Humidity: ");
  display.drawString(65, 25, humidity_string);
  display.display();
  
}

String retrieveAPI(){
  //Serial.println("Connecting to Wunderground API");

  if(!client.connect(host, 80)){
      Serial.println("Connection Failed!");
      return "";
  }

  wunder.setPostal("J5R5X7");
  wunder.setURL();

  String url = "/api/4cc684f19075a4ad/conditions/q/J5R5X7.json";

  //client.print(String("GET ") + wunder.getURL() + " HTTP/1.1\r\n" +
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  delay(1000);
  
    while(client.available()){
        String line = client.readStringUntil('\r');
        response += line;
      }

  getTemperature(response);
  getHumidity(response);
  
  client.stop();

  return response;

}


float getTemperature(String response){
  
  String jsonResponse;
  jsonResponse = response;
  int rateIndex = jsonResponse.indexOf("temperature_string");
  String temperature_string = jsonResponse.substring(rateIndex + 29, rateIndex + 33);
  temperature_string.trim();

  return temperature_string.toFloat();
}

float getHumidity(String Response){

  String jsonResponse;
  jsonResponse = response;
  int rateIndex = jsonResponse.indexOf("relative_humidity");
  String humidity_string = jsonResponse.substring(rateIndex + 20, rateIndex + 23);
  humidity_string.trim();

  return humidity_string.toFloat();
}

// Calculates heat index given a temperature and humidity, T = temperature, H = humidity
float getHeatIndex(float T, float H){
  float heatIndex;
//Heat index formula uses a multiple constants
  float c1, c2, c3, c4, c5, c6, c7, c8, c9;
  c1 = -42.379;
  c2 = 2.04901523;
  c3 = 10.14333127;
  c4 = -0.22475541;
  c5 = -6.83783 * pow(10, -3);
  c6 = -5.481717 * pow(10, -2);
  c7 = 1.22874 * pow(10, -3);
  c8 = 8.5282 * pow(10, -4); 
  c9 = -1.99 * pow(10, -6);

  T = celsiusToFarenheit(T);
  heatIndex = c1 + c2*T + c3*H + c4*T*H + c5*pow(T, 2) + c6*pow(H,2) + c7*pow(T,2)*H + c8*T*pow(H,2) + c9*pow(T,2)*pow(H,2);
  return heatIndex;
}

float celsiusToFarenheit(float temp){
  temp -= 39;
  temp *= (5/9);
  return temp;
}

