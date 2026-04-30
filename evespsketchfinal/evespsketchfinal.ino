#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <DHT.h>
#include <TinyGPS++.h>

// --- Configuration ---
#define DHTPIN 4
#define DHTTYPE DHT11
#define MQ_PIN 34
const char* ssid = "ESP32_Environment_Monitor";
const byte DNS_PORT = 53;

// --- Objects ---
DHT dht(DHTPIN, DHTTYPE);
HardwareSerial neogps(2);
TinyGPSPlus gps;
IPAddress apIP(172, 217, 28, 1); // Common gateway IP
DNSServer dnsServer;
WebServer server(80);

String getHTML() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int gas = analogRead(MQ_PIN);
  
  // Logic for Air Quality status
  String status = (gas < 1800) ? "Clean" : (gas < 2800 ? "Moderate" : "Heavy Pollution");
  String statusColor = (gas < 1800) ? "#27ae60" : (gas < 2800 ? "#f39c12" : "#e74c3c");

  String html = "<!DOCTYPE html><html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<style>";
  html += "body { font-family: 'Segoe UI', sans-serif; background: #eef2f3; color: #333; text-align: center; margin: 0; padding: 20px; }";
  html += ".container { max-width: 500px; margin: auto; }";
  html += ".card { background: white; padding: 20px; border-radius: 15px; box-shadow: 0 10px 20px rgba(0,0,0,0.05); margin-bottom: 20px; }";
  html += ".label { font-size: 0.9rem; color: #7f8c8d; text-transform: uppercase; letter-spacing: 1px; }";
  html += ".value { font-size: 2.5rem; font-weight: bold; margin: 10px 0; color: #2c3e50; }";
  html += ".status { font-weight: bold; color: " + statusColor + "; }";
  html += ".btn { display: inline-block; padding: 15px 25px; background: #3498db; color: white; text-decoration: none; border-radius: 30px; font-weight: bold; margin-top: 10px; box-shadow: 0 4px 10px rgba(52,152,219,0.3); }";
  html += "</style><script>setInterval(() => { location.reload(); }, 5000);</script></head><body>";

  html += "<div class='container'><h1>EcoStation</h1>";
  
  // Temp & Humidity
  html += "<div class='card'><div class='label'>Climate</div><div class='value'>" + String(t, 1) + "&deg;C</div><div class='label'>Humidity: " + String(h, 0) + "%</div></div>";
  
  // Air Quality
  html += "<div class='card'><div class='label'>Air Quality (MQ135)</div><div class='value'>" + String(gas) + "</div><div class='status'>" + status + "</div></div>";
  
  // GPS
  html += "<div class='card'><div class='label'>Location Services</div>";
  if (gps.location.isValid()) {
    String lat = String(gps.location.lat(), 6);
    String lng = String(gps.location.lng(), 6);
    html += "<div class='value' style='font-size: 1.2rem;'>" + lat + ", " + lng + "</div>";
    html += "<a href='https://www.google.com/maps/search/?api=1&query=" + lat + "," + lng + "' target='_blank' class='btn'>Open in Google Maps</a>";
  } else {
    html += "<div class='value' style='font-size: 1.2rem; color: #95a5a6;'>Waiting for Satellites...</div>";
  }
  html += "</div></div></body></html>";
  
  return html;
}

void setup() {
  Serial.begin(115200);
  neogps.begin(9600, SERIAL_8N1, 16, 17);
  dht.begin();

  // Set up Access Point with Captive Portal
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid);

  // Redirect all DNS requests to the ESP IP
  dnsServer.start(DNS_PORT, "*", apIP);

  server.on("/", []() {
    server.send(200, "text/html", getHTML());
  });

  // Captive Portal Redirect for Android/iOS
  server.onNotFound([]() {
    server.sendHeader("Location", String("http://") + apIP.toString(), true);
    server.send(302, "text/plain", "");
  });

  server.begin();
  Serial.println("Captive Portal Started.");
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
  
  while (neogps.available() > 0) {
    gps.encode(neogps.read());
  }
}