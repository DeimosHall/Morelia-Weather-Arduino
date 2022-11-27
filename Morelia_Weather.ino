#include <WiFi.h>
#include <FirebaseESP32.h>

#define sensor 25
#define URL "https://morelia-weather-default-rtdb.firebaseio.com/"
#define SECRET "5kyFjuSQG2FAh01JluZ1qMQPJg9kw9bRnPofOsTj"
#define ssid "Pixel_4"
#define pass "hello123"

float temperature = 0.0;

FirebaseData myFirebaseData;

void setup() {
  Serial.begin(115200);
  pinMode(sensor, INPUT);

  Serial.print("Conecting to network: " + String(ssid));
  
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Firebase.begin(URL, SECRET);
  Firebase.reconnectWiFi(true); // In case of a disconnecton
  Serial.print("Connected to database");  
}

void loop() {
  delay(1000);
  temperature += 0.1;
  sendTemperature(temperature);
}

void sendTemperature(float temperature) {
  Firebase.set(myFirebaseData, "/Temperature", String(temperature));
}
