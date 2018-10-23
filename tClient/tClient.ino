//working v1.2
#include <ESP8266WiFi.h>

const int inputPin = 2;
int pirState = LOW;
int val = 0;

const char *ssid = "IoT";
const char *password = "p@ssword321";

void setup() {
  pinMode(inputPin, INPUT);
  Serial.begin(115200);
  delay(10);

  // Explicitly set the ESP8266 to be a WiFi-client
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

}

void loop() {
  int sendValue = 1;

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const char * host = "192.168.4.1";
  const int httpPort = 80;

  //const char * host2 = "192.168.4.1";//ip of relay
  //const int httpPort2 = 85;//port of relay

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request. E.G. /data/?showerSensor=20
  String url = "/data/";
  url += "?showerSensor=";
  url += sendValue;

  //New code
  val = digitalRead(inputPin);
  if (val == HIGH) {
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    delay(150);
//    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
//                 "Host: " + host2 + "\r\n" +
//                 "Connection: close\r\n\r\n");
  }

  //End of new code

  // This will send the request to the server

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  delay(500);
}
