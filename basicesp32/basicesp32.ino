#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include <TinyGPS++.h>

// Sensor Pins
#define DHTPIN 4
#define DHTTYPE DHT11
#define MQ_PIN 34
DHT dht(DHTPIN, DHTTYPE);

// GPS setup
HardwareSerial neogps(2);
TinyGPSPlus gps;

// Network Credentials
const char* ssid = "ESP32_Monitor";
const char* password = "password123";

WebServer server(80);

String getPage() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int gas = analogRead(MQ_PIN);
  
  String page = "<!DOCTYPE html><html lang='en'><head>";
  page += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  page += "<title>ESP32 Dashboard</title>";
  page += "<style>";
  page += "body { font-family: sans-serif; background: #f0f2f5; display: flex; justify-content: center; padding: 20px; }";
  page += ".card-container { width: 100%; max-width: 400px; }";
  page += ".card { background: white; padding: 20px; margin-bottom: 15px; border-radius: 12px; box-shadow: 0 4px 6px rgba(0,0,0,0.1); }";
  page += "h2 { margin: 0 0 10px; font-size: 14px; color: #666; text-transform: uppercase; }";
  page += ".value { font-size: 28px; font-weight: bold; color: #1a73e8; }";
  page += ".unit { font-size: 16px; color: #888; margin-left: 5px; }";
  page += "</style><script>setInterval(() => { location.reload(); }, 5000);</script></head><body>";
  
  page += "<div class='card-container'>";
  page += "<h1>Device Status</h1>";
  
  // Temperature Card
  page += "<div class='card'><h2>Temperature</h2><div class='value'>" + String(t, 1) + "<span class='unit'>&deg;C</span></div></div>";
  
  // Humidity Card
  page += "<div class='card'><h2>Humidity</h2><div class='value'>" + String(h, 0) + "<span class='unit'>%</span></div></div>";
  
  // Air Quality Card
  page += "<div class='card'><h2>Air Quality (MQ135)</h2><div class='value'>" + String(gas) + "<span class='unit'>ADC</span></div></div>";
  
  // GPS Card
  page += "<div class='card'><h2>Location</h2><div class='value' style='font-size: 18px;'>";
  if (gps.location.isValid()) {
    page += String(gps.location.lat(), 6) + ", " + String(gps.location.lng(), 6);
  } else {
    page += "Acquiring Satellites...";
  }
  page += "</div></div>";
  
  page += "</div></body></html>";
  return page;
}

void handleRoot() {
  server.send(200, "text/html", getPage());
}

void setup() {
  Serial.begin(115200);
  neogps.begin(9600, SERIAL_8N1, 16, 17);
  dht.begin();
  
  WiFi.softAP(ssid, password);
  Serial.println("AP IP address: " + WiFi.softAPIP().toString());
  
  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
  while (neogps.available() > 0) {
    gps.encode(neogps.read());
  }
}