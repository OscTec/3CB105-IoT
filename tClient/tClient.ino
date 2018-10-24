//working v1.3
#include <ESP8266WiFi.h>                           //Adds library for WiFi
#include <ThingSpeak.h>                            //Adds library for cloud service

WiFiClient client;                                 //Initialize WiFi Client
const int inputPin = 2;                            //
int pirState = LOW;                                //Set default sensor state
int val = 0;

//const char *ssid = "IoT";
//const char *password = "p@ssword321";

const char *ssid = "M.I.6 SURVEILLANCE VAN 2";     //WiFi Name
const char *password = "JR38D0TdMLef6ml4";         //WiFi Password

// ThingSpeak information
char thingSpeakAddress[] = "api.thingspeak.com";   //Cloud service address
unsigned long channelID = 607501;                  //Channel ID
char* readAPIKey = "BVQ2L7CXBAOLN8II";             //Key to read from channel
char* writeAPIKey = "GE8UZRGHVTQJV2VO";            //Key to write to channel
const unsigned long postingInterval = 10L * 1000L; //Left is how many seconds as right is
unsigned int dataFieldOne = 1;                     //Field to write temperature data
unsigned int aField = 6;                           //Field to hold first constant of the thermistor calibration

int sendValue = 1;

void setup() {
  pinMode(inputPin, INPUT);                        //Sets Sensor input pin
  Serial.begin(115200);                            //Sets serial baud
  delay(10);                                       //


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
  //int sendValue = 1;
  val = digitalRead(inputPin);
  if (val == HIGH) {
    if (sendValue == 1) {
      writeTSData(channelID, dataFieldOne, sendValue);
      sendValue = 0;
    } else {
      writeTSData(channelID, dataFieldOne, sendValue);
      sendValue = 1;
    }
    
    delay(500);
  }





  // Use WiFiClient class to create TCP connections

  //  const char *host = "192.168.4.1";
  //  const int httpPort = 80;
  //
  //  if (!client.connect(host, httpPort)) {
  //    Serial.println("connection failed");
  //    return;
  //  }

  // We now create a URI for the request. E.G. /data/?showerSensor=20
  //  String url = "/data/";
  //  url += "?showerSensor=";
  //  url += sendValue;
  //
  //  val = digitalRead(inputPin);
  //  if (val == HIGH) {
  //    //write2TSData(channelID, dataFieldOne, "1");
  //    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
  //                 "Host: " + host + "\r\n" +
  //                 "Connection: close\r\n\r\n");
  //    delay(150);
  //  }
  //  unsigned long timeout = millis();
  //  while (client.available() == 0) {
  //    if (millis() - timeout > 5000) {
  //      Serial.println(">>> Client Timeout !");
  //      client.stop();
  //      return;
  //    }
  //  }
  //  delay(500);

}




int writeTSData( long TSChannel, unsigned int TSField, float data ) {
  int  writeSuccess = ThingSpeak.writeField( TSChannel, TSField, data, writeAPIKey ); // Write the data to the channel
  if ( writeSuccess ) {

    Serial.println( String(data) + " written to Thingspeak." );
  }

  return writeSuccess;
}
