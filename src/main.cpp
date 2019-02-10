#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "HOME-CEBE-2.4";
const char* pswd = "AN3TM7D7HWYLH9HH";
int attempts = 0;

ESP8266WebServer server(80);

const int led = LED_BUILTIN;

void handleRoot() {
  // digitalWrite(led, LOW);
  server.send(200, "text/html", 
    "<h1>Hello from esp8266!</h1>"
    "<a href='on'>"
      "<div style=\"padding: 50 50 50 50; background-color: blue; color: white;\">ON</div>"
    "</a>"
    "<a href='off'>"
      "<div style=\"padding: 50 50 50 50; background-color: red; color: white;\">OFF</div>"
    "</a>"
    );
  delay(200);
  // digitalWrite(led, HIGH);
}

void turnOn(){
  digitalWrite(led, LOW);
  Serial.println("Received request at /on");
  server.send(200,"text/html", "<h1>Turned Light On</h1><a href='/'>Home</a>");
}

void turnOff(){
  digitalWrite(led, HIGH);
  Serial.println("Received request at /off");
  server.send(200, "text/html", "<h1>Turned Light Off</h1><a href='/'>Home</a>");
}

void setup(void) {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
  Serial.begin(9600);
  while (!Serial);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pswd);
  Serial.println("");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) Serial.println("MDNS responder started");

  server.on("/", handleRoot);
  server.on("/on", turnOn);
  server.on("/off", turnOff);
  server.onNotFound([](){server.send(404, "text/plain", "Error, not found.");});
  server.begin();
  Serial.print("HTTP server started @ http://");
  Serial.print(WiFi.localIP());
  Serial.println(":80");
}

void loop(void) {
  // put your main code here, to run repeatedly:
  if(WiFi.status() == WL_CONNECTED && attempts < 5){
    Serial.print("Server running @ ");
    Serial.print(WiFi.localIP());
    Serial.println(":80");
    delay(1000);
    attempts++;
  }
  server.handleClient();
}
