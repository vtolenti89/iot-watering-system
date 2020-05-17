#include "helper.h"
#include "WiFiEsp.h"

enum waterLevelStates {
  WL_0 = 0, // water level is empty
  WL_20 = 20, // water level is 20%
  WL_40 = 40, // water level is 40% 
  WL_60 = 60, // water level is 60%
  WL_80 = 80, // water level is 80%
  WL_100= 100 // water level is 100%
};


// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(PD2, PD3); // RX, TX
#endif

//wifi settings
char ssid[] = "UPC6E2796A";            // your network SSID (name)
char pass[] = "Jmre8szre4ez";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

//router setting
const char url[] = "votolentino.com";
const long httpPort = 80; //default 80
const char epGetWaterLevel [] = "/garden";
const char epSetWaterLevel [] = "/garden/setwaterlevel/";
const char epUptHeartBeat [] = "/garden/updateheartbeat";
char epSetWaterLevelBuffer [sizeof(epSetWaterLevel) + 1]; 

//pin settings
const int yellowLed = PD6;
const int redLed = PD7;
const int motorPWM = PB1;
const int lWaterLevelPin = A0;
const int sWaterLevelPin = PC1;

//motor and water level settings
enum waterLevelStates wlState;
enum waterLevelStates previouswlState;
const long motorTurnOnDelay = 360; // minutes
const long motorOnTime = 20; //minutes 
long motorTimeDelay = 359; //minutes
long int lWaterLevel = 0
;

Helper helper;

// Initialize the Ethernet client object
WiFiEspClient client;

void setup()
{
    //Setting up pin inputs and outputs
  DDRD=0x00;
  DDRB=0x00;
  //DDRC=0x00;
  DDRD|=(1 << yellowLed) | (1 << redLed);
  DDRB|=(1 << motorPWM);
  //DDRC&=~((1 << lWaterLevelPin) | (1 << sWaterLevelPin));
  
  // initialize serial for debugging
  Serial.begin(115200);
  // initialize serial for ESP module
  Serial1.begin(9600);
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");
  
  printWifiStatus();
  
  Serial.println();
  Serial.println("Starting connection to server...");
  // if you get a connection, report back via serial
  getReq(url, epUptHeartBeat);
}

void loop()
{
  // if there are incoming bytes available
  // from the server, read them and print them
  while (client.available()) {
    String c = client.readStringUntil('\n');
    Serial.println(c);
  }

  // if the server's disconnected, stop the client
  if (!client.connected()) {
    client.stop();
  }

    if(helper.getTimerFlag(2)){
      //PORTD^=(1 << yellowLed);
      //esp8266.getRequest(url , endpoint);
      
    }

   if(helper.getTimerFlag(3)){
    wlState = uptWaterState();
    //check water level
   }

   if(helper.getTimerFlag(4)){ 
    //only increment the motor time
    //if there is water in the tank
    if(wlState > WL_0) {
      motorTimeDelay++;      
    }

    getReq(url, epUptHeartBeat);
    Serial.print("motorTimeDelay:");
    Serial.println(motorTimeDelay);
    
   }

   if(motorTimeDelay >= motorTurnOnDelay && wlState > WL_0) {
      PORTB|=(1 << motorPWM);
      PORTD|=(1 << redLed);
      if(motorTimeDelay - motorTurnOnDelay > motorOnTime) {
        uptDBWaterLevel(wlState);
          Serial.println("Turning off the motor");
          PORTB&=~(1 << motorPWM);
          PORTD&=~(1 << redLed);
          motorTimeDelay=0;
        }
    }
    helper.updateTimer();
}


void getReq (char * url, char * endpoint) {
  // close any connection before send a new request
  // this will free the socket on the WiFi shield
  client.stop();
  
  if (client.connect(url, httpPort)) {
    Serial.println("Connecting...");
    // Make a HTTP request
    client.println("GET " + String(endpoint) +" HTTP/1.1");
    client.println("Host: " + String(url));
    client.println("Connection: close");
    client.println();
  }else {
    // if you couldn't make a connection
    Serial.println("Connection failed");
  }
}


void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}


waterLevelStates uptWaterState() {
    lWaterLevel = analogRead(lWaterLevelPin);
    Serial.print("Water Level:");
    Serial.print(lWaterLevel);
    Serial.print(",WLState:");
    Serial.println(wlState);
    
  previouswlState = wlState;
  if(lWaterLevel >= 900) {
    return WL_0;
    } else if (lWaterLevel >= 800){
      return WL_20;
      }else if (lWaterLevel >= 650){
      return WL_40;
      }else if (lWaterLevel >= 600){
      return WL_60;
      }else if (lWaterLevel >= 500){
      return WL_80;
      } else {
        return WL_100;
        }
  //update the database if the wlState changed
  //if(previouswlState != wlState) {
      
  //}
}


void uptDBWaterLevel(int waterLevel) {
  sprintf(epSetWaterLevelBuffer, "%s%i", epSetWaterLevel, waterLevel);
  Serial.print("Setting Water level - Calling:");
  Serial.println(epSetWaterLevelBuffer);
  getReq(url, epSetWaterLevelBuffer);
}
