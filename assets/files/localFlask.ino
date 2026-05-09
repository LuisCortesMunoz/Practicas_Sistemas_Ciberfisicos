#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

// ====== CAMBIA ESTO ======
const char* WIFI_SSID = "SM-G935F2416";
const char* WIFI_PASS = "LuisCM52";

// IP de tu PC donde corre Flask
const char* SERVER_IP = "10.128.126.132";   // <- TU IP (la que ya probaste)
const int   SERVER_PORT = 5500;

#define LED_PIN   5     // GPIO al DIN
#define LED_COUNT 8     // total LEDs físicos (cámbialo si tu tira es 30/60/etc)
// =========================

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int lastRev = -1;
unsigned long lastPoll = 0;
const unsigned long POLL_MS = 400;

String stateUrl() {
  return String("http://") + SERVER_IP + ":" + String(SERVER_PORT) + "/api/state";
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

void applyState(uint8_t r, uint8_t g, uint8_t b, int countOn) {
  for (int i = 0; i < LED_COUNT; i++) {
    if (i < countOn) strip.setPixelColor(i, strip.Color(r, g, b));
    else            strip.setPixelColor(i, 0);
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

  // DOC más grande por el updated_at largo
  StaticJsonDocument<512> doc;
  DeserializationError err = deserializeJson(doc, body);

  if (err) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(err.c_str());
    return false;
  }

  const char* colorStr = doc["color"] | "#ff0000";
  int count = doc["count"] | LED_COUNT;
  int rev   = doc["rev"] | 0;

  Serial.print("Parsed color=");
  Serial.print(colorStr);
  Serial.print(" count=");
  Serial.print(count);
  Serial.print(" rev=");
  Serial.println(rev);

  if (rev == lastRev) return true;
  lastRev = rev;

  if (count < 0) count = 0;
  if (count > LED_COUNT) count = LED_COUNT;

  uint8_t r, g, b;
  if (!parseHexColor(colorStr, r, g, b)) {
    Serial.println("Invalid color format");
    return false;
  }

  applyState(r, g, b, count);
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