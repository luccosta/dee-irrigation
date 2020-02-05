#include <Arduino.h>

#include <SPI.h>
#include "ESP8266WiFi.h"
#include "ESP8266HTTPClient.h"
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "WiFiManager.h"
#include <WiFiUdp.h>
#include <EEPROM.h>
#include <stdio.h>
#include <string.h>

// Replace with AWS
#define INDEX_HTML "<!DOCTYPE html><html><head><title>SmartGarden-ESP8266 Configuration</title><link rel='shortcut icon' href='{favicon}' /><style>body { font-family: Helvetica, Arial, sans-serif; font-size: 16px ;padding: 10px }</style></head><body><h1>SmartGarden-ESP8266 Configuration</h1><div  style=' float:left;'><form method='GET' action='/conf' target='output_frame'><label><strong>Light start hour: </strong></label><br /><input type='number' min='0' max='23' step='1' pattern='([01]?[0-9]{1}|2[0-3]{1})' name='startTime' value='{startTime}' maxlength='2' size='1' style='font-size: 16px;' /><br /><br /><label><strong>Light end hour: </strong></label><br /><input type='number' min='0' max='23' step='1' pattern='([01]?[0-9]{1}|2[0-3]{1})' name='endTime' value='{endTime}' maxlength='2' size='1' style='font-size: 16px;' /><br /><br /><label><strong>Watering hour: </strong></label><br /><input type='number' min='0' max='23' step='1' pattern='([01]?[0-9]{1}|2[0-3]{1})' name='waterTime' value='{waterTime}' maxlength='2' size='1' style='font-size: 16px;' /><br /><br /><strong>Password: </strong></label><br /><input type='password' maxlength='32' name='pwd' value='{pwd}' size='32' style='font-size: 16px;' /><br /><br /><input type='submit' value='Submit'></form></div><div style=' float:left; margin-left: 160px;'><h3>Status</h3><p><Strong>Pot 1:</Strong> {pot1}</p><p><Strong>Pot 2:</Strong> {pot2}</p><p><Strong>Water tank:</Strong> {tank}</p><p><Strong>Already watered today:</Strong> {waterToday}</p></div><br /><br /><div style='clear:both'></div><h3>Set a password </h3><p>Please set a password of max 32 characters. </p><form method='GET' action='/newpwd' target='output_frame'><label><strong>New Password: </strong></label><br /><input type='password' maxlength='32' name='newPwd' value='{newPwd}' size='32' style='font-size: 16px;' /><br /><br /><input type='submit' value='Submit'></form><br /><br /><iframe id='output_frame' name='output_frame' allowtransparency='true' width='350' height='80' frameBorder='0'>Browser not compatible</iframe></body></html>"
#define FAVICON "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAACHklEQVR4XoWT70tTURjHzz/in9CbueaYW2rlikCIftELdXOTpeJPlqNITV9LTWQzInMZc4Qvyk0NhYKoqDdtc7C1lFWv1FigsQiMxr6e5xzP9d6gPPDc59z7nOfz/DqXlctl0CJNUqlUxDtpZVN7ZXv3KYHO+1ZxjoknX5mvrzWQAirb23wc/TN29M7YhPQ/ssM9US0BKsq1e1ZhGI978H49oUWffnlDOF2POTD4/AxGl50YnG9Ayx2TEdB614RbT0/h5pM69EVqhVPy8wuhhxZOC+fhRCO6Z23oiFjgUgCVJhG7o7UYisvDgbk6+KN2DMZO4PaSEwM8g46HVvQ8dojo/ukmYw9agia4gtXoitjE4ZHFRpFuIOqAN3wcbZNmtE9ZhLNKX+sB9WHnZ5EbzKBSvGELPCEz+mbt8HGnZOEVfOF6eIJWrCTntAnRVLQmEu33nz2MxFwiwpvsEnoenIQvJMelmq3f0zcxxn8ZV3MxuEM1/weoC0SnCsPHsOFjyB3Ihpch08SQOsuwphP9xdIuEgFKqTgylxh23AzfW6XevszlAkOR63UOI5B+Gd+45aOHoTTAsNsr9berDJvnJSTNnYsLk0aAvgTaZylqlwTsEuDKISB1jqH8q2T4TwxTIMCXcScK7dJZAbZ5BlTCGgf83XQDgIw/PjxDrk2mr0oo8vS3LvJmHgVQ5VAZeT4BkvTBFGgS2eYqkYH+V98HQY0eaNw1dPsAAABRdEVYdENvbW1lbnQAQ29weXJpZ2h0IElOQ09SUyBHbWJIICh3d3cuaWNvbmV4cGVyaWVuY2UuY29tKSAtIFVubGljZW5zZWQgcHJldmlldyBpbWFnZbaaaaYAAAA4dEVYdENvcHlyaWdodABDb3B5cmlnaHQgSU5DT1JTIEdtYkggKHd3dy5pY29uZXhwZXJpZW5jZS5jb20pTs6ZTgAAAFp6VFh0Q29tbWVudAAAeJxzzi+oLMpMzyhR8PRz9g8KVnDPTfJQ0CgvL9fLTM7PS60oSC3KTM1LTtVLzs/VVNBVCM3LyUxOzStOTVEoKEoty0wtV8jMTUxPBQC4jxoknLyY4wAAAEF6VFh0Q29weXJpZ2h0AAB4nHPOL6gsykzPKFHw9HD4DwpWcM9N8lDQKC8v18tMzs9LrShILcpMzUtO1UvOz9UEAH02EGgc3eaPAAAAAElFTkSuQmCC"

#define period 1000*10
#define DEBUG false //if true will ignore if already watered

// Pinos utilizados
#define D0 16 
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15
#define D9 3
#define D10 1
#define SD2 9
#define SD3 10

extern "C" {
#include "user_interface.h"
}

os_timer_t myTimer;
int timeSyncCounter = 0;
#define TIME_SYNC_SECONDS 3600

unsigned long localTime = 0;

bool eepromReadRequired = true;
int startTime, endTime, waterTime, day;
int isPasswordSet;
char pwd[33];
char server_pwd[33];
char new_pwd[33];

int status = WL_IDLE_STATUS;     // the Wifi radio's status

// Domoticz (Search where is used)
int idx = 2;
char* ipPort = "192.168.1.93:8080"; // Replace with AWS

void setTimeFromNTP(const int update);
void updateTimeFromInternalClock(const int seconds);
void timerCallback(void *pArg);
void readDataFromEeprom(int& startTime, int& endTime, int& waterTime);
void readIsPasswordSetFromEeprom(int& isPasswordSet);
void readPasswordFromEeprom();
void resetPassword();
void writeIsPasswordSetOnEeprom();
void resetPasswordFromButton();

//starts http server on port 8080
ESP8266WebServer server(8080); // Replace with AWS

int getHour(long epoch);
bool inRange(int currentHour, int startTime, int endTime);
bool waterToday(int currentDate, int savedDate);
int getDay(long epoch);
void writeDayOnEeprom(int day);
void readDayFromEeprom(int &day);

void setup() {
  // Moisture sensor in water tank
  pinMode(D0, INPUT);
  // Moisture sensor in pots
  pinMode(D3, INPUT);
  pinMode(D4, INPUT);
  // Led input
  pinMode(D1, OUTPUT);
  digitalWrite(D1, LOW);
  // Water tank input
  pinMode(D2, OUTPUT);
  digitalWrite(D2, LOW);
  //Password reset button
  pinMode(D6, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(D6), resetPasswordFromButton, CHANGE);
  
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  os_timer_setfn(&myTimer, timerCallback, NULL);
  os_timer_arm(&myTimer, 1000, true);
  
  /* SETUP WIFI CONNECTION THROUGH WIFIMANAGER */
  WiFiManager wifiManager;
  //wifiManager.resetSettings();    //Uncomment this to wipe WiFi settings from EEPROM on boot.  Comment out and recompile/upload after 1 boot cycle.
  wifiManager.setAPStaticIPConfig(IPAddress(192,168,1,94), IPAddress(192,168,1,94), IPAddress(255,255,255,0));
  wifiManager.autoConnect("Erobotica", "pi-e=0.42");
  //if you get here you have connected to the WiFi
  Serial.println("Connected");

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  delay(500);

  if (DEBUG) Serial.println("Starting UDP");
  udp.begin(localPort);
  if (DEBUG) Serial.print("Local port: ");
  if (DEBUG) Serial.println(udp.localPort());

  setTimeFromNTP(0);

  if(isPasswordSet){
    readPasswordFromEeprom();
      if (DEBUG) {
        Serial.println("PASSWORD FROM EEPROM: ");
        Serial.println(pwd);
      }
  }
  // Server configuration (Put here configuration for AWS IoT)
  
  
}

void loop() {  
  if (DEBUG)   delay(5000);
  
  // Handle Client of Server Here
  
  if(timeSyncCounter >= TIME_SYNC_SECONDS) {
    setTimeFromNTP(1);
    timeSyncCounter = 0;
    if(DEBUG) Serial.println("Time sync from NTP");
  }
  //if(DEBUG) Serial.print("Time: ");
  //if(DEBUG) Serial.println(localTime);

  //TODO send all info to domoticz
  //sendDataToServer();

  if (eepromReadRequired) {
    readDataFromEeprom(startTime, endTime, waterTime);    
    eepromReadRequired = false;
  }

  if(DEBUG) Serial.print("startTime, endTime, waterTime: ");
  if(DEBUG) Serial.print(startTime);
  if(DEBUG) Serial.print(" ");
  if(DEBUG) Serial.print(endTime);
  if(DEBUG) Serial.print(" ");
  if(DEBUG) Serial.println(waterTime);

  // LED control
  if (inRange(getHour(localTime), startTime, endTime)){
      if (DEBUG) Serial.println("In range and led off");
      digitalWrite(D1, HIGH);
   } else {
      if (DEBUG) Serial.println("Not in range and led on");
      digitalWrite(D1, LOW);
  }

  //WATER control
  readDayFromEeprom(day);
  
  if (DEBUG) Serial.print("Current Day: ");
  if (DEBUG) Serial.println(getDay(localTime));
  if (DEBUG) Serial.print("Saved Day: ");
  if (DEBUG) Serial.println(day);
  if (DEBUG) Serial.print("Water today? ");
  if (DEBUG) Serial.println(waterToday(getDay(localTime), day));
  if (DEBUG) Serial.print("Water tank and ground? ");
  if (DEBUG) Serial.println(String(digitalRead(D0)) + " " + String(digitalRead(D3)) + " " + String(digitalRead(D4)));
  
  if (waterTime == getHour(localTime) && (waterToday(getDay(localTime), day) || DEBUG)) {
    // if (tank not dry && pot1 dry && pot2 dry)
    if (!digitalRead(D0) && digitalRead(D3) && digitalRead(D4)){
      if (DEBUG) Serial.println("Watering... ");
      digitalWrite(D2, HIGH);
      delay(13000);
      digitalWrite(D2, LOW);
      if (DEBUG) Serial.println("Watered. ");
      writeDayOnEeprom(getDay(localTime));
    }
  }

  delay(100);
}

void setTimeFromNTP(const int update) {
  WiFi.hostByName(ntpServerName, timeServerIP); 
  sendNTPpacket(timeServerIP); // send an NTP packet to a time server
  // wait to see if a reply is available
  delay(1000);
  
  int cb = udp.parsePacket();

  if(update == 0) {
    while (!cb) {
      if (DEBUG)Serial.println("no packet yet");
    }
  }
  if((update == 1 && cb) || update == 0) {
    if (DEBUG) Serial.print("packet received, length=");
    if (DEBUG) Serial.println(cb);
    // We've received a packet, read the data from it
    udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
  
    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:
  
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    if (DEBUG) Serial.print("Seconds since Jan 1 1900 = " );
    if (DEBUG) Serial.println(secsSince1900);
  
    // now convert NTP time into everyday time:
    if (DEBUG) Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    // print Unix time:
    Serial.println(epoch);
    
    // print the hour, minute and second:
    if (DEBUG) Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
    if (DEBUG) Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
    if (DEBUG) Serial.print(':');
    if ( ((epoch % 3600) / 60) < 10 ) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      if (DEBUG) Serial.print('0');
    }
    if (DEBUG) Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
    if (DEBUG) Serial.print(':');
    if ( (epoch % 60) < 10 ) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      if (DEBUG) Serial.print('0');
    }
    if (DEBUG) Serial.println(epoch % 60); // print the second
  
    localTime = epoch + (3600 * GMT);
  }
}

void updateTimeFromInternalClock(const int seconds) {
  localTime += seconds;
  //if (DEBUG) Serial.println(localTime);
}

void timerCallback(void *pArg) {
  updateTimeFromInternalClock(1);
  timeSyncCounter++;
}

int getHour(long epoch){
  if (DEBUG) {
    Serial.print ("getHour: ");
    Serial.println ((epoch  % 86400L) / 3600);
  }
  return (epoch  % 86400L) / 3600;
}

bool inRange(int currentHour, int startTime, int endTime){
  if (startTime >= endTime){
    return (currentHour >= startTime && currentHour <= 23) || (currentHour >= 0 && currentHour < endTime);
  }
  return currentHour >= startTime && currentHour < endTime;
}

void readDataFromEeprom(int& startTime, int& endTime, int& waterTime) {
  EEPROM.begin(512);
  EEPROM.get(0, startTime);
  EEPROM.get(sizeof(startTime), endTime);
  EEPROM.get(sizeof(startTime) + sizeof(endTime), waterTime);
  EEPROM.end();
}

void writeDayOnEeprom(int day){
  EEPROM.begin(512);
  EEPROM.put(sizeof(startTime) + sizeof(endTime) + sizeof(waterTime), day);
  EEPROM.commit();
  EEPROM.end();
}

void readDayFromEeprom(int &day){
  EEPROM.begin(512);
  EEPROM.get(sizeof(startTime) + sizeof(endTime) + sizeof(waterTime), day);
  EEPROM.end();
}

void readIsPasswordSetFromEeprom(int& isPasswordSet){
  EEPROM.begin(512);
  EEPROM.get(sizeof(startTime) + sizeof(endTime) + sizeof(waterTime) + sizeof(day), isPasswordSet);
  EEPROM.end();
}

void readPasswordFromEeprom() {
  EEPROM.begin(512);
  EEPROM.get(sizeof(startTime) + sizeof(endTime) + sizeof(waterTime) + sizeof(day) + sizeof(isPasswordSet), pwd);
  EEPROM.end();
}

void resetPassword(){
  EEPROM.begin(512);
  EEPROM.put(sizeof(startTime) + sizeof(endTime) + sizeof(waterTime) + sizeof(day), 0);
  EEPROM.commit();
  EEPROM.end();
}

void writeIsPasswordSetOnEeprom(){
  EEPROM.begin(512);
  EEPROM.put(sizeof(startTime) + sizeof(endTime) + sizeof(waterTime) + sizeof(day), 1);
  EEPROM.commit();
  EEPROM.end();
}

void resetPasswordFromButton(){
  resetPassword();
  readIsPasswordSetFromEeprom(isPasswordSet);
  if (DEBUG) {
    Serial.print("Resetting Password. isPasswordSet: ");
    Serial.println(isPasswordSet);
  }
}

bool waterToday(int currentDate, int savedDate){
  return currentDate != savedDate;
}

int getDay(long epoch){
  return (epoch / 86400L);
}