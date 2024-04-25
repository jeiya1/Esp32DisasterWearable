#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "ESP8266_AP";  // SSID of the access point
const char *password = "password"; // Password of the access point

ESP8266WebServer server(80);

String latitude = "";
String longitude = "";
String altitude = "";
String speed = "";

void handleRoot() {
  String response = "<h1>Hello from ESP8266</h1>";
  response += "<p>Latitude: " + latitude + "</p>";
  response += "<p>Longitude: " + longitude + "</p>";
  response += "<p>Altitude: " + altitude + " meters</p>";
  response += "<p>Speed: " + speed + " km/h</p>";
  
  server.send(200, "text/html", response);
}

void handleUpdate() {
  latitude = server.arg("latitude");
  longitude = server.arg("longitude");
  altitude = server.arg("altitude");
  speed = server.arg("speed");
  
  Serial.println("Received GPS data:");
  Serial.println("Latitude: " + latitude);
  Serial.println("Longitude: " + longitude);
  Serial.println("Altitude: " + altitude + " meters");
  Serial.println("Speed: " + speed + " km/h");

  server.send(200, "text/plain", "GPS data received successfully");
}

void setup() {
  Serial.begin(115200);
  delay(100);

  // Set up the access point
  WiFi.softAP(ssid, password);
  delay(100);

  // Print the IP address of the server
  Serial.print("Server IP address: ");
  Serial.println(WiFi.softAPIP());

  // Set up the routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/update", HTTP_GET, handleUpdate);

  // Start the server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Handle client requests
  server.handleClient();
}
