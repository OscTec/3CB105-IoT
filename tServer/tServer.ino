//working v1.3
#include <ESP8266WiFi.h>
//#include <ESP8266WebServer.h>
#include <Servo.h>
#include <ThingSpeak.h>

WiFiClient client;
const int buttonPin = 2;
int buttonState = 0;
int angle = 0;
boolean btnPressed = false;

//const char *ssid = "IoT";
//const char *password = "p@ssword321";


const char *ssid = "M.I.6 SURVEILLANCE VAN 2";     //WiFi Name
const char *password = "JR38D0TdMLef6ml4";         //WiFi Password

// ThingSpeak information
char thingSpeakAddress[] = "api.thingspeak.com";   //Cloud service address
unsigned long channelID = 607501;                  //Channel ID
char* readAPIKey = "BVQ2L7CXBAOLN8II";             //Key to read from channel
char* writeAPIKey = "GE8UZRGHVTQJV2VO";            //Key to write to channel
long lastReadUpdateTime = 0;
long lastWriteUpdateTime = 0;
const unsigned long readingInterval = 30L * 1000L; //Time between reads
const unsigned long postingInterval = 30L * 1000L; //Time between writes
unsigned int dataFieldOne = 1;                     //Field to write occupied data
unsigned int dataFieldTwo = 2;                     //Field to write angle data
unsigned int aField = 6;                           //Field to hold first constant of the thermistor calibration



//ESP8266WebServer server(80);
Servo servo;



void setup() {
  pinMode(buttonPin, INPUT);
  servo.attach(0);
  Serial.begin(115200);
  delay(10000);


  WiFi.mode(WIFI_STA);                             // Explicitly set the ESP8266 to be a WiFi-client
  WiFi.begin(ssid, password);                      //Starts connection to WiFi using given values
  Serial.print("Connecting");                      //Lets user know it's connecting
  while (WiFi.status() != WL_CONNECTED) {          //While waiting to connect
    Serial.print(".");                             //print a dot every
    delay(500);                                    //500 ms
  }
  Serial.print("Connected");                       //Once connected print connected
  ThingSpeak.begin(client);                        //Connect to ThingSpeak

  //  WiFi.softAP(ssid, password);
  //  IPAddress myIP = WiFi.softAPIP();
  //
  //  server.on("/data/", HTTP_GET, handleSentVar); // when the server receives a request with /data/ in the string then run the handleSentVar function
  //  server.begin();
  //  Serial.println(myIP);
  //  Serial.print("Setup done");



}

void loop() {
  //server.handleClient();
  //  if ((int)readTSData(channelID, dataFieldOne) == 1) {
  //    angle = 160;
  //  }
  //angle = (int)readTSData(channelID, dataFieldTwo);
  //writeTSData(channelID, dataFieldOne, sendValue);
  //  if (millis() - lastReadUpdateTime >=  readingInterval) {
  //    lastReadUpdateTime = millis();
  //
  //  }
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    btnPressed = true;
    //servo.attach(0);
    angle = angle + 5;
    delay(50);
    if (angle > 180) {
      delay(200);
      angle = 0;
    }

  } else {
    //servo.detach();
  }
  if (millis() - lastWriteUpdateTime >=  postingInterval) {
    lastWriteUpdateTime = millis();
    if (btnPressed) {
      writeTSData(channelID, dataFieldTwo, angle);
      btnPressed = false;
    }
    angle = (int) readTSData(channelID, dataFieldTwo);
  }

  servo.write(angle);
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


//void handleSentVar() {
//  if (server.hasArg("showerSensor")) { // this is the variable sent from the client
//    int readingInt = server.arg("showerSensor").toInt();
//    Serial.println("Recieved");
//    Serial.println(readingInt);
//    //Code that doesn't work
//    if (readingInt == 1) {
//      angle = 160;
//    }
//    //End of code that doesn't work
//    server.send(200, "text/html", "Data received");
//  }
//}
