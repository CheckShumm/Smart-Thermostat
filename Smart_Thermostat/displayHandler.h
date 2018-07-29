
/*
*
* Oled Display 
*
*/

#ifndef displayHandler
#define displayHandler

#include "icon.h"
#include "wundergroundClient.h"
#include "jsonParser.h"
#include <Adafruit_SSD1306.h>

class displayHandler(){

private:

  double temp;
  double humidity;
  String condition;
  bool isSunnny;
  bool isRainy;
  bool isClear;
  bool isCloudy;
  
  wundergroundClient client;
  
public:

  void displayTemp(double temp);
  void displayHumidity(double humidity);
  void displayIcon(String condition);
  
  void screenTransition();
  void displayWifi();
  
  void checkConditions();


};

#endif
