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

const int SEND_TIME = 10; // Time to send temperature to the database in seconds
int elapsedTime = 0;

void setup() {
  pinMode(RGB_RED, OUTPUT);
  pinMode(RGB_GREEN, OUTPUT);
  pinMode(RGB_BLUE, OUTPUT);
  
  Serial.begin(115200);
  sensor.begin(); // Start the DS18B20 sensor

  Serial.print("Conecting to network: " + String(ssid));
  setRed();
  
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Firebase.begin(URL, SECRET);
  Firebase.reconnectWiFi(true); // In case of a disconnection
  Serial.println("\nConnected to database");  
}

void loop() {
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
