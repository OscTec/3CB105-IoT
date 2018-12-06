//working v1.3
#include <ESP8266WiFi.h>
#include <Servo.h>
#include <ThingSpeak.h>

WiFiClient client;                                 //Sets the WiFiClient libraru to client
Servo servo;                                       //Sets the Servo library to servo
const int buttonPin = 2;                           //The pin the button is attached to
int buttonState = 0;                               //Sets button state to 0
int angle = 0;                                     //Sets angle to 0
boolean btnPressed = false;                        //Sets btnPress to false to default not pressed

const char *ssid = "M.I.6 SURVEILLANCE VAN 2";     //WiFi Name
const char *password = "JR38D0TdMLef6ml4";         //WiFi Password

// ThingSpeak information
char thingSpeakAddress[] = "api.thingspeak.com";   //Cloud service address
unsigned long channelID = 607501;                  //Channel ID
char* readAPIKey = "BVQ2L7CXBAOLN8II";             //Key to read from channel
char* writeAPIKey = "GE8UZRGHVTQJV2VO";            //Key to write to channel
long lastReadUpdateTime = 0;                       //Default value for the last Read Update
long lastWriteUpdateTime = 0;                      //Default value for the last Write Update
const unsigned long readingInterval = 30L * 1000L; //Time between reads
const unsigned long postingInterval = 30L * 1000L; //Time between writes
unsigned int dataFieldOne = 1;                     //Field to write occupied data
unsigned int dataFieldTwo = 2;                     //Field to write angle data
//unsigned int aField = 6;                           //Field to hold first constant of the thermistor calibration

void setup() {
  pinMode(buttonPin, INPUT);                       //Sets the button pin to a input
  servo.attach(0);                                 //Attaches the servo to pin 0
  Serial.begin(115200);                            //Begins serial on baud 115200
  delay(5000);                                     //Waits 5 seconds
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
  buttonState = digitalRead(buttonPin);            //Reads the current state of the button
  if (buttonState == HIGH) {                       //If that state is high (pressed) do
    btnPressed = true;                             //Set btnPressed to true
    angle = angle - 3;                             //Add 3 to angle
    delay(50);                                     //wait 0.05 seconds
    if (angle <= 0) {                             //If angle is greater than 180 degrees
      delay(200);                                  //Wait 0.2 seconds
      angle = 91;                                   //Set angle to 0 degrees.
      delay(300);
    }
  }
  if (millis() - lastWriteUpdateTime >=  postingInterval) { //If time is greater than posting time do
    lastWriteUpdateTime = millis();
    if (btnPressed) {
      writeTSData(channelID, dataFieldTwo, angle);
      btnPressed = false;
    }
    angle = (int) readTSData(channelID, dataFieldTwo);
  }
  servo.write(angle);                               //Tells servo to more to angle
}

float readTSData(long TSChannel, unsigned int TSField ) {
  float data =  ThingSpeak.readFloatField( TSChannel, TSField, readAPIKey );
  Serial.println( " Data read from ThingSpeak: " + String( data, 9 ) );
  return data;
}

int writeTSData( long TSChannel, unsigned int TSField, float data ) {
  int  writeSuccess = ThingSpeak.writeField( TSChannel, TSField, data, writeAPIKey ); // Write the data to the channel
  if ( writeSuccess ) {
    Serial.println( String(data) + " written to Thingspeak." );
  }
  return writeSuccess;
}
