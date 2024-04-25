#include <WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <TinyGPS++.h>

const char *ssid = "ESP8266_AP";      // SSID of the ESP8266 access point
const char *password = "password";    // Password of the ESP8266 access point
const char *host = "192.168.4.1"; // IP address of the ESP8266


#define LED_PIN 19     // Pin connected to the NeoPixel LED ring
#define NUM_LEDS 12    // Number of LEDs in the ring

Adafruit_NeoPixel ring(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// GPS related variables
HardwareSerial gpsSerial(1); // Assuming Neo-8M is connected to Serial1 (GPIO2 - TX, GPIO3 - RX)
TinyGPSPlus gps;

const int buttonPin = 33; // Pin connected to the button

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  
  int connectionAttempts = 0; // Counter for connection attempts
  
  while (connectionAttempts < 2) {
    WiFi.begin(ssid, password);
    
    int frontLeds = 3;  // Number of LEDs in the front
    uint32_t frontColor = Adafruit_NeoPixel::Color(0, 0, 255); // Blue
    uint32_t backColor = Adafruit_NeoPixel::Color(0, 0, 200); // Lighter shade of blue
    uint16_t wait = 3000; // Duration of loading sequence in milliseconds
    
    // Display loading sequence while connecting to WiFi
    for(int i = 0; i < NUM_LEDS; i++) {
      if (i < frontLeds) {
        ring.setPixelColor(i, frontColor);
      } else {
        // Calculate lighter shade of blue for LEDs at the back
        uint8_t red = ((backColor >> 16) & 0xFF) / 2;
        uint8_t green = ((backColor >> 8) & 0xFF) / 2;
        uint8_t blue = (backColor & 0xFF) / 2;
        ring.setPixelColor(i, ring.Color(red, green, blue));
      }
      ring.show();
      delay(wait / NUM_LEDS);
      ring.clear();
      
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        
        // Display green around the entire circle for 3 seconds
        for (int i = 0; i < NUM_LEDS; i++) {
          ring.setPixelColor(i, Adafruit_NeoPixel::Color(0, 255, 0));
        }
        ring.show();
        delay(3000);
        
        ring.clear(); // Turn off the LEDs
        ring.show();
        
        return; // Exit the function if WiFi connection is successful
      }
    }
    
    connectionAttempts++; // Increment connection attempt counter
  }
  
  // If all connection attempts failed, blink blue for 3 seconds and then skip the process
  uint32_t blueColor = Adafruit_NeoPixel::Color(0, 0, 200);
  for (int i = 0; i < 3; i++) {
    ring.fill(blueColor, 0, NUM_LEDS);
    ring.show();
    delay(250);
    ring.clear();
    ring.show();
    delay(250);
  }
}

void sendGPSData() {
  Serial.println("Sending GPS data");

  // Construct the HTTP request with GPS data
  String request = "GET /update?latitude=" + String(gps.location.lat(), 6) + "&longitude=" + String(gps.location.lng(), 6);
  request += "&altitude=" + String(gps.altitude.meters(), 1) + "&speed=" + String(gps.speed.kmph(), 1);
  request += " HTTP/1.1\r\n";
  request += "Host: ";
  request += host;
  request += "\r\n";
  request += "Connection: close\r\n\r\n";

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  if (!client.connect(host, 80)) {
    Serial.println("Connection failed");
    return;
  }

  // Send the HTTP request
  client.print(request);

  Serial.println("Request sent");
}

void sendSOS() {
  // Define the Morse code for "SOS"
  int dotDuration = 250;
  int dashDuration = dotDuration * 3;
  int letterSpaceDuration = dotDuration;
  int wordSpaceDuration = dotDuration * 3;

  // Define colors
  uint32_t red = ring.Color(255, 0, 0);
  uint32_t off = ring.Color(0, 0, 0);

  // S
  delay(wordSpaceDuration);
  ring.fill(red, 0, NUM_LEDS); // Turn all LEDs red
  ring.show();
  delay(dotDuration);
  ring.clear(); // Turn off LEDs
  ring.show();
  delay(letterSpaceDuration);

  ring.fill(red, 0, NUM_LEDS); // Turn all LEDs red
  ring.show();
  delay(dotDuration);
  ring.clear(); // Turn off LEDs
  ring.show();
  delay(letterSpaceDuration);

  ring.fill(red, 0, NUM_LEDS); // Turn all LEDs red
  ring.show();
  delay(dotDuration);
  ring.clear(); // Turn off LEDs
  ring.show();
  delay(letterSpaceDuration);

  // O
  ring.fill(red, 0, NUM_LEDS); // Turn all LEDs red
  ring.show();
  delay(dashDuration);
  ring.clear(); // Turn off LEDs
  ring.show();
  delay(letterSpaceDuration);

  ring.fill(red, 0, NUM_LEDS); // Turn all LEDs red
  ring.show();
  delay(dashDuration);
  ring.clear(); // Turn off LEDs
  ring.show();
  delay(letterSpaceDuration);

  ring.fill(red, 0, NUM_LEDS); // Turn all LEDs red
  ring.show();
  delay(dashDuration);
  ring.clear(); // Turn off LEDs
  ring.show();
  delay(letterSpaceDuration);

  // S
  ring.fill(red, 0, NUM_LEDS); // Turn all LEDs red
  ring.show();
  delay(dotDuration);
  ring.clear(); // Turn off LEDs
  ring.show();
  delay(letterSpaceDuration);

  ring.fill(red, 0, NUM_LEDS); // Turn all LEDs red
  ring.show();
  delay(dotDuration);
  ring.clear(); // Turn off LEDs
  ring.show();
  delay(letterSpaceDuration);

  ring.fill(red, 0, NUM_LEDS); // Turn all LEDs red
  ring.show();
  delay(dotDuration);
  ring.clear(); // Turn off LEDs
  ring.show();
  delay(letterSpaceDuration);
}

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT);
  delay(100);
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17); // Neo-8M connected to GPIO16 (RX) and GPIO17 (TX)
  delay(100);
  connectToWiFi();
}

void loop() {
  // Output the state every second
  if (digitalRead(buttonPin) == HIGH) {
    // system here
    // Retry connecting to WiFi until successful or until a certain number of attempts
      
    if (WiFi.status() != WL_CONNECTED) {
      // WiFi connection failed, blink blue LED for 2 seconds
      connectToWiFi();
    }
      
    // Send SOS signal regardless of WiFi connection status
    sendSOS();
    Serial.println("finished SOS");
    delay(1000);
      
    // Proceed with GPS data sending if WiFi connected
    if (WiFi.status() == WL_CONNECTED) {
      while (gpsSerial.available() > 0) {
        if (gps.encode(gpsSerial.read())) {
          sendGPSData(); // Send GPS data when a new valid sentence is parsed
        }
      }
      uint32_t whiteColor = Adafruit_NeoPixel::Color(10, 10, 10);
      ring.fill(whiteColor, 0, NUM_LEDS);
      ring.show();
      delay(500);
      ring.clear();
      ring.show();
      Serial.println("finished GPS");
      delay(1000);
    }
  } else {
  }
  delay(1000);
}