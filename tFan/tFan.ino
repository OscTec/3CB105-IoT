#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>


const int relayPin = 2;
const char *ssid = "IoT";
const char *password = "p@ssword321";

boolean fan = false;

ESP8266WebServer server(85);

void setup() {
  pinMode(relayPin, OUTPUT);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  server.on("/data/", HTTP_GET, handleSentVar); // when the server receives a request with /data/ in the string then run the handleSentVar function
  server.begin();
  Serial.println(myIP);
  Serial.print("Setup done");

}

void loop() {
  server.handleClient();
  if (fan) {
    //turn on fan
    delay(600000);//How long to wait in ms
    fan = false;
  }
}


void handleSentVar() {
  if (server.hasArg("showerSensor")) { // this is the variable sent from the client
    Serial.println("Recieved");
    fan = true;
    server.send(200, "text/html", "Data received");
  }
}
