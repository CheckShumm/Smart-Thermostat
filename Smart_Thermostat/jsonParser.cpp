 #include "jsonParser.h"
 #include <WiFiClient.h>


  int jsonParser::findValue(String response, String condition){

    for (int i=0; i<response.length(); i++){

       int j=0;
       
       if (response.charAt(i) == condition.charAt(j)){
        while(response.charAt(i) == condition.charAt(j)){
          i++;
          j++; 

          if (j = condition.length()){
            Serial.print(i);
          }
        }
       }
    }
  }

