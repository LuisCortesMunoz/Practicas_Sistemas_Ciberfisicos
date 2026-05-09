#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

// ======================= CONFIGURA ESTO =======================
const char* WIFI_SSID = "Primavera26";
const char* WIFI_PASS = "Ib3r02026pR1m";

// IP de la PC donde corre el BACKEND FLASK (PC B)
const char* SERVER_IP = "172.22.26.195";   // <- CAMBIA A TU IP REAL
const int SERVER_PORT = 5500;

// Tira WS2812
#define LED_PIN 13       // GPIO conectado a DIN
#define LED_COUNT 8    // TOTAL de LEDs físicos en tu tira
// =============================================================

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int lastRev = -1;
unsigned long lastPoll = 0;
const unsigned long POLL_MS = 300;  // cada 300ms (ajusta si quieres)

String stateUrl() {
  return String("http://") + SERVER_IP + ":" + String(SERVER_PORT) + "/api/state";
}

String healthUrl() {
  return String("http://") + SERVER_IP + ":" + String(SERVER_PORT) + "/api/health";
}

bool parseHexColor(const char* hex, uint8_t &r, uint8_t &g, uint8_t &b) {
  if (!hex) return false;
  if (strlen(hex) != 7) return false;
  if (hex[0] != '#') return false;

  char rs[3] = { hex[1], hex[2], 0 };
  char gs[3] = { hex[3], hex[4], 0 };
  char bs[3] = { hex[5], hex[6], 0 };

  r = (uint8_t) strtoul(rs, nullptr, 16);
  g = (uint8_t) strtoul(gs, nullptr, 16);
  b = (uint8_t) strtoul(bs, nullptr, 16);
  return true;
}

void applyState(uint8_t r, uint8_t g, uint8_t b, int countOn) {
  if (countOn < 0) countOn = 0;
  if (countOn > LED_COUNT) countOn = LED_COUNT;

  for (int i = 0; i < LED_COUNT; i++) {
    if (i < countOn) strip.setPixelColor(i, strip.Color(r, g, b));
    else strip.setPixelColor(i, 0);
  }
  strip.show();
}

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.printf("Conectando a WiFi: %s\n", WIFI_SSID);

  unsigned long t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t0 < 15000) {
    delay(400);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi conectado!");
    Serial.print("IP ESP32: ");
    Serial.println(WiFi.localIP());

    Serial.print("Backend health: ");
    Serial.println(healthUrl());

    Serial.print("Backend state:  ");
    Serial.println(stateUrl());
  } else {
    Serial.println("\n❌ No se pudo conectar a WiFi (timeout).");
  }
}

bool httpGetJson(String url, String &outBody) {
  HTTPClient http;
  http.setTimeout(4000);  // 4s
  http.begin(url);

  int code = http.GET();

  if (code <= 0) {
    Serial.print("HTTP GET failed: ");
    Serial.println(http.errorToString(code));
    http.end();
    return false;
  }

  outBody = http.getString();
  http.end();
  return true;
}

bool fetchAndUpdate() {
  if (WiFi.status() != WL_CONNECTED) return false;

  String body;
  if (!httpGetJson(stateUrl(), body)) return false;

  // JSON puede traer fecha larga, por eso 512
  StaticJsonDocument<512> doc;
  DeserializationError err = deserializeJson(doc, body);

  if (err) {
    Serial.print("deserializeJson failed: ");
    Serial.println(err.c_str());
    Serial.println("BODY:");
    Serial.println(body);
    return false;
  }

  const char* colorStr = doc["color"] | "#ff0000";
  int count = doc["count"] | LED_COUNT;
  int rev   = doc["rev"] | 0;

  // ✅ si no hay cambios, no hacemos nada
  if (rev == lastRev) return true;
  lastRev = rev;

  uint8_t r, g, b;
  if (!parseHexColor(colorStr, r, g, b)) {
    Serial.println("Color inválido en JSON.");
    return false;
  }

  applyState(r, g, b, count);

  Serial.printf("✅ UPDATED rev=%d color=%s count=%d\n", rev, colorStr, count);
  return true;
}

void setup() {
  Serial.begin(115200);

  strip.begin();
  strip.setBrightness(80); // 0-255
  strip.show();

  connectWiFi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
    delay(800);
    return;
  }

  if (millis() - lastPoll >= POLL_MS) {
    lastPoll = millis();
    fetchAndUpdate();
  }

  delay(5);
}
