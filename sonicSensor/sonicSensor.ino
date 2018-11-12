#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <NewPing.h>
#define trigPin 0
#define echoPin 2
#define MAX_DISTANCE 500
NewPing sonar(trigPin, echoPin, MAX_DISTANCE);
int pos = 20;

WiFiClient client;

const char *ssid = "M.I.6 SURVEILLANCE VAN 2";     //WiFi Name
const char *password = "JR38D0TdMLef6ml4";         //WiFi Password

// ThingSpeak information
char thingSpeakAddress[] = "https://api.thingspeak.com";   //Cloud service address
unsigned long channelID =  610016;                  //Channel ID
//char* readAPIKey = "BVQ2L7CXBAOLN8II";             //Key to read from channel
char* writeAPIKey = "JD9W52HRY51TZPXV";            //Key to write to channel
const unsigned long postingInterval = 20L * 1000L; //Left is how many seconds as right is
unsigned int dataFieldOne = 1;                     //Field to write temperature data
//unsigned int aField = 6;                           //Field to hold first constant of the thermistor calibration

int sendValue = 1;

void setup() {
  pinMode(trigPin, OUTPUT);                        //Sets Sensor input pin
  pinMode(echoPin, INPUT); 
  Serial.begin(115200);                            //Sets serial baud

  WiFi.mode(WIFI_STA);                             // Explicitly set the ESP8266 to be a WiFi-client
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
  int duration, distance, pos = 0, i;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
  Serial.print(distance);
  Serial.print(" cm");
  writeTSData(channelID, dataFieldOne, distance);
  delay(1000);
}

int writeTSData( long TSChannel, unsigned int TSField, float data ) {
  int  writeSuccess = ThingSpeak.writeField( TSChannel, TSField, data, writeAPIKey ); // Write the data to the channel
  if ( writeSuccess ) {

    Serial.println( String(data) + " written to Thingspeak." );
  }
  return writeSuccess;
}
