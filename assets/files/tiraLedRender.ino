#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

// ====== CAMBIA ESTO ======
//const char* WIFI_SSID = "Primavera26";
//const char* WIFI_PASS = "Ib3r02026pR1m";

const char* WIFI_SSID = "SM-G935F2416";
const char* WIFI_PASS = "LuisCM52";

// Render base (sin /api/...)
const char* RENDER_BASE = "https://docker-flask-servidor-render.onrender.com";

// Endpoint que SÍ existe en tu Render
const char* STATE_PATH = "/api/state_leds";

#define LED_PIN   13     // GPIO al DIN
#define LED_COUNT 8      // 8 LEDs físicos
// =========================

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Usaremos updated_at como "revisión"
double lastUpdatedAt = -1.0;

unsigned long lastPoll = 0;
const unsigned long POLL_MS = 400;

String stateUrl() {
  return String(RENDER_BASE) + String(STATE_PATH);
}

bool parseHexColor(const char* hex, uint8_t &r, uint8_t &g, uint8_t &b) {
  if (!hex || strlen(hex) != 7 || hex[0] != '#') return false;
  char rs[3] = {hex[1], hex[2], 0};
  char gs[3] = {hex[3], hex[4], 0};
  char bs[3] = {hex[5], hex[6], 0};
  r = (uint8_t) strtoul(rs, nullptr, 16);
  g = (uint8_t) strtoul(gs, nullptr, 16);
  b = (uint8_t) strtoul(bs, nullptr, 16);
  return true;
}

void applyLeds(const bool ledOn[8], uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < LED_COUNT; i++) {
    if (ledOn[i]) strip.setPixelColor(i, strip.Color(r, g, b));
    else          strip.setPixelColor(i, 0);
  }
  strip.show();
}

bool fetchAndUpdate() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
    return false;
  }

  HTTPClient http;
  String url = stateUrl();

  Serial.print("GET -> ");
  Serial.println(url);

  http.begin(url);
  int code = http.GET();

  if (code < 0) {
    Serial.print("HTTP GET failed: ");
    Serial.println(http.errorToString(code));
    http.end();
    return false;
  }

  Serial.print("HTTP code: ");
  Serial.println(code);

  String body = http.getString();
  http.end();

  Serial.println("---- RAW BODY START ----");
  Serial.println(body);
  Serial.println("---- RAW BODY END ----");

  StaticJsonDocument<1024> doc;
  DeserializationError err = deserializeJson(doc, body);

  if (err) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(err.c_str());
    return false;
  }

  // Tu JSON: {"ok":true,"state":{...}}
  bool ok = doc["ok"] | false;
  if (!ok) {
    Serial.println("Server replied ok=false");
    return false;
  }

  // updated_at (double)
  double updatedAt = doc["state"]["updated_at"] | 0.0;

  // Si no cambió, no actualizamos (opcional)
  if (updatedAt == lastUpdatedAt) {
    Serial.println("No changes (updated_at same).");
    return true;
  }
  lastUpdatedAt = updatedAt;

  const char* hexStr = doc["state"]["hex"] | "#ff0000";

  Serial.print("Parsed hex=");
  Serial.print(hexStr);
  Serial.print(" updated_at=");
  Serial.println(updatedAt, 6);

  // leds array size 8
  JsonArray arr = doc["state"]["leds"].as<JsonArray>();
  if (arr.isNull() || arr.size() != 8) {
    Serial.println("Invalid leds array (expected size 8)");
    return false;
  }

  bool ledOn[8];
  for (int i = 0; i < 8; i++) {
    ledOn[i] = arr[i] | false;
  }

  uint8_t r, g, b;
  if (!parseHexColor(hexStr, r, g, b)) {
    Serial.println("Invalid hex format");
    return false;
  }

  applyLeds(ledOn, r, g, b);
  return true;
}

void connectWiFi() {
  Serial.printf("Connecting to WiFi: %s\n", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  unsigned long t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t0 < 15000) {
    delay(400);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    Serial.print("ESP32 IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("State URL: ");
    Serial.println(stateUrl());
  } else {
    Serial.println("\nWiFi NOT connected (timeout).");
  }
}

void setup() {
  Serial.begin(115200);

  strip.begin();
  strip.setBrightness(80);
  strip.show();

  connectWiFi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
    delay(500);
    return;
  }

  if (millis() - lastPoll >= POLL_MS) {
    lastPoll = millis();
    fetchAndUpdate();
  }

  delay(5);
}