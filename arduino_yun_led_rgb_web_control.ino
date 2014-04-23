/*
This code controls an RGB LED via a webpage and an Arduino Yun

Use this to change the light color:
http://[arduino].local/arduino/rgb/[redValue]/[greenValue]/[blueValue]/

Circuit:
http://d.pr/i/JrDw/5zHxAbG6 (NOTE: requires a Yun to work - schematic is an Uno)

Authors:
Robby Grant | @fourgrant
David Taylor | @SloshDev

With help from:
Dan Price | @prixdc
John Markham | @johnmarkham

Made for the lab @ archer>malmo - http://thelab.archermalmo.com/

 */

#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
#include <String.h>

const int redPin = 11; 
const int greenPin = 10; 
const int bluePin = 9; 

String redString; 
String greenString; 
String blueString; 
int redValue;
int greenValue;
int blueValue;


// Listen on default port 5555, the webserver on the Yun
// will forward there all the HTTP requests for us.
YunServer server;

void setup() {
  // Bridge startup
  Bridge.begin();

  // Listen for incoming connection only from localhost
  // (no one from the external network could connect)
  server.listenOnLocalhost();
  server.begin();
}
void loop() {
  // Get clients coming from server
  YunClient client = server.accept();
  // There is a new client?
  if (client) {
    // Process request
    process(client);
    // Close connection and free resources.
    client.stop();
  }
  delay(50); // Poll every 50ms
}

void process(YunClient client) {
  // read the command in the URL
  String command = client.readStringUntil('/'); 
  
  // read the full URL (everything AFTER /arudino/)
  String fullUrl = client.readString();
 
// looks for rgb
// could later use this command to make the lights do other stuff like OFF and ON, etc. 
  if (command == "rgb") {
    
      // separate color values from URL using getValue command (see code at bottom of page)
      redString = (getValue(fullUrl, '/', 0));
      greenString = (getValue(fullUrl, '/', 1));
      blueString = (getValue(fullUrl, '/', 2));

      // getValue stores it as a string. use this function to convert it over to an interger so we can pass to the Arduino
      int redValue = redString.toInt(); 
      int greenValue = greenString.toInt(); 
      int blueValue = blueString.toInt(); 
      
      //send color values to Arudino
      analogWrite(redPin, redValue);
      analogWrite(greenPin, greenValue);
      analogWrite(bluePin, blueValue);
  }
  
}

// getValue function that parses and separates URL value
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
