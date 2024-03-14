#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "wifi.id";
const char* password = "nanti aja";

ESP8266WebServer server(80);  // Port untuk web server

// Define pins
const int LED_PIN = D1;
const int BUZZER_PIN = D2;
const int BUTTONUP = D3;
const int BUTTONDOWN = D4;
const int BUTTONONOFF = D5;

// Define variables
int btnUP = 0;
int btnDown = 0;
int btnONOFF = 0;
int BUZZER = 100;
int LED = 0;
bool on = false;

void setup() {
  Serial.begin(9600);
  delay(1000);
  // Initialize pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTONUP, INPUT_PULLUP);
  pinMode(BUTTONDOWN, INPUT_PULLUP);
  pinMode(BUTTONONOFF, INPUT_PULLUP);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi.");
  Serial.println(WiFi.localIP());
  server.on("/", []() {
    String html = "<html><head><meta http-equiv='refresh' content='5'></head><body><h1>Monitoring Value LED & Piezzo Buzzer</h1>";
    html += "<p>LED : " + String(LED) + "</p><p>Piezzo Buzzer : " + String(BUZZER) + "</p></body></html>";
    server.send(200, "text/html", html);
  });
  server.begin();  // Memulai web server
}

void loop() {
  // Check button state
  btnUP = digitalRead(BUTTONUP);
  btnDown = digitalRead(BUTTONDOWN);
  btnONOFF = digitalRead(BUTTONONOFF);

  // kondisi jika tombol on/off ditekan
  if (btnONOFF == LOW) {
    on = !on;
    delay(1000);
  }

  // kondisi jika tombol UP Value ditekan
  if (btnUP == LOW) {
    // kondisi jika komponen on
    if (on) {
      if (LED < 249) {
        LED += 3;
      }
      if (BUZZER < 2500) {
        BUZZER += 10;
      }
      // Panggil fungsi untuk memperbarui halaman web
      updateWebPage();
      delay(150);
    }
  }

  // kondisi jika tombol Down Value ditekan
  if (btnDown == LOW) {
    // kondisi jika komponen on
    if (on) {
      if (LED > 0) {
        LED -= 3;
      }
      if (BUZZER > 100) {
        BUZZER -= 10;
      }
      // Panggil fungsi untuk memperbarui halaman web
      updateWebPage();
      delay(150);
    }
  }

  // Kondisi jika komponen on
  if (on) {
    analogWrite(LED_PIN, LED);
    tone(BUZZER_PIN, BUZZER);

  // Kondisi jika komponen off
  } else {
    analogWrite(LED_PIN, 0);
    noTone(BUZZER_PIN);
  }
  server.handleClient();  // Menangani permintaan dari klien
}

// Fungsi untuk memperbarui halaman web
void updateWebPage() {
  String html = "<html><head><meta http-equiv='refresh' content='1'></head><body><h1>Monitoring Value LED & Piezzo Buzzer</h1>";
  html += "<p>LED : " + String(LED) + "</p><p>Piezzo Buzzer : " + String(BUZZER) + "</p></body></html>";
  server.send(200, "text/html", html);
}