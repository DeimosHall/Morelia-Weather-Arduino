#include <WiFi.h>
#include <FirebaseESP32.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define TEMP_SENSOR 33
#define RGB_RED 25
#define RGB_GREEN 26
#define RGB_BLUE 27

#define URL "https://morelia-weather-default-rtdb.firebaseio.com/"
#define SECRET "5kyFjuSQG2FAh01JluZ1qMQPJg9kw9bRnPofOsTj"
#define ssid "Pixel_4"
#define pass "hello123"

FirebaseData myFirebaseData;
OneWire oneWire(TEMP_SENSOR);
DallasTemperature sensor(&oneWire);

const int SEND_TIME = 60; // Time to send the temperature to the database in seconds
int elapsedTime = SEND_TIME; // Time counter to validate SEND_TIME

void setup() {
  pinMode(RGB_RED, OUTPUT);
  pinMode(RGB_GREEN, OUTPUT);
  pinMode(RGB_BLUE, OUTPUT);
  
  Serial.begin(115200);
  sensor.begin(); // Start the DS18B20 sensor

  connectToWiFi();

  Firebase.begin(URL, SECRET);
  Firebase.reconnectWiFi(true); // In case of a disconnection
  Serial.println("\nConnected to database");  
}

void loop() {
  connectToWiFi();
  checkSensorStatus();

  Serial.println("Temperature: " + String(getSensorTemp()));

  if (elapsedTime == SEND_TIME) {
    elapsedTime = 0;
    sendTemperature(getSensorTemp());
  } else {
    elapsedTime++;
  }
  
  delay(1000); 
}

void connectToWiFi() {
  Serial.print("Conecting to network: " + String(ssid));
  
  while(WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, pass);
    Serial.print(".");
    setRed();
    delay(500);
  }
}

void checkSensorStatus() {
  if (getSensorTemp() == -127.0) { // Sensor does not work
    setGreen();
  } else {
    setBlue();
  }
}

float getSensorTemp() {
  sensor.requestTemperatures();
  return sensor.getTempCByIndex(0);
}

void sendTemperature(float temperature) {
  Firebase.set(myFirebaseData, "/Temperature", String(temperature));
}

// Not connected to database
void setRed() {
  analogWrite(RGB_RED, 255);
  analogWrite(RGB_GREEN, 0);
  analogWrite(RGB_BLUE, 0);
}

// Connected to database but the sensor does not work
void setGreen() {
  analogWrite(RGB_RED, 0);
  analogWrite(RGB_GREEN, 255);
  analogWrite(RGB_BLUE, 0);
}


// Connected to database and the sensor works
void setBlue() {
  analogWrite(RGB_RED, 0);
  analogWrite(RGB_GREEN, 0);
  analogWrite(RGB_BLUE, 255);
}
