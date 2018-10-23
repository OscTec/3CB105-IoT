//working v1.2
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

const int buttonPin = 2;
int buttonState = 0;
int angle = 0;

const char *ssid = "IoT";
const char *password = "p@ssword321";

ESP8266WebServer server(80);
Servo servo;

void handleSentVar() {
  if (server.hasArg("showerSensor")) { // this is the variable sent from the client
    int readingInt = server.arg("showerSensor").toInt();
    Serial.println("Recieved");
    Serial.println(readingInt);
    //Code that doesn't work
    if (readingInt == 1) {
      angle = 160;
    }
    //End of code that doesn't work
    server.send(200, "text/html", "Data received");
  }
}

void setup() {
  pinMode(buttonPin, INPUT);
  servo.attach(0);
  Serial.begin(115200);
  delay(10000);

  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();

  server.on("/data/", HTTP_GET, handleSentVar); // when the server receives a request with /data/ in the string then run the handleSentVar function
  server.begin();
  Serial.println(myIP);
  Serial.print("Setup done");

}

void loop() {
  server.handleClient();
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
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
//  if (correctPosition()) {
//    servo.detach()
//  } else {
//    servo.attach(0);
//  }
  servo.write(angle);
}

//boolean correctPosition() {
//  servo.attach(0);
//  if (servo.read() == angle) {
//    return true;
//  } else {
//    return false;
//  }
//  servo.detach();
//}
