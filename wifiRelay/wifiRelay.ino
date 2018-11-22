#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

WiFiClient client;                                 //Sets the WiFiClient libraru to client
const int relayPin = 2;                            //The pin the relay is attached to
float cloudValue;
int relayState = 0;

const char *ssid = "M.I.6 SURVEILLANCE VAN 2";     //WiFi Name
const char *password = "JR38D0TdMLef6ml4";         //WiFi Password

// ThingSpeak information
char thingSpeakAddress[] = "api.thingspeak.com";   //Cloud service address
unsigned long channelID = 607501;                  //Channel ID
char* readAPIKey = "BVQ2L7CXBAOLN8II";             //Key to read from channel
long lastReadUpdateTime = 0;                       //Default value for the last Read Update
const unsigned long readingInterval = 30L * 1000L; //Time between reads
unsigned int dataFieldOne = 1;                     //Field to write occupied data

void setup() {

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);                             //Explicitly set the ESP8266 to be a WiFi-client
  WiFi.begin(ssid, password);                      //Starts connection to WiFi using given values
  Serial.print("Connecting");                      //Lets user know it's connecting
  while (WiFi.status() != WL_CONNECTED) {          //While waiting to connect
    Serial.print(".");                             //print a dot every
    delay(500);                                    //500 ms
  }
  Serial.print("Connected");                       //Once connected print connected
  ThingSpeak.begin(client);                        //Connect to ThingSpeak
}

void loop() {
  if (millis() - lastReadUpdateTime >=  readingInterval) { //If time is greater than posting time do
    lastReadUpdateTime = millis();
    cloudValue = (int)readTSData(channelID, dataFieldOne);
    Serial.println("Cloud value ");
    Serial.println(cloudValue);
    if (cloudValue > 0.5) {
      relayState = 1;
      Serial.println(relayState);
    } else {
      relayState = 0;
      Serial.println(relayState);
    }
  }
  if (relayState == 1) {
    digitalWrite(relayPin, HIGH);
    Serial.println("I'm meant to be on");
    delay(600*1000);
  } else {
    digitalWrite(relayPin, LOW);
    Serial.println("I'm meant to be off");
  }
}

float readTSData(long TSChannel, unsigned int TSField ) {
  float data =  ThingSpeak.readFloatField( TSChannel, TSField, readAPIKey );
  Serial.println( " Data read from ThingSpeak: " + String( data, 9 ) );
  return data;
}
