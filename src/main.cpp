#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Si7021.h>
#include <MD_MAX72XX.h>
#include <DHT.h>
#include <MD_Parola.h>

// I2C Pins
#define I2C_SDA 21
#define I2C_SCL 22

// Display settings
#define LED_BUILTIN 2
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define DIN_PIN 23  // MOSI pin for SPI
#define CLK_PIN 18  // SCK pin for SPI
#define CS_PIN  5   // SS pin for SPI

// DHT settings
#define DHTPIN 2
#define DHTTYPE DHT22

Adafruit_Si7021 sensor = Adafruit_Si7021();
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DIN_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
DHT dht(DHTPIN, DHTTYPE);
MD_Parola display = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// put function declarations here:
int myFunction(int, int);
void displayValue(float value, const char* unit);
float celsiusToFahrenheit(float celsius);

void setup() {
  Wire.begin(I2C_SDA, I2C_SCL);
  Serial.begin(9600);
  
  // Initialize LED matrix
  mx.begin();
  mx.control(MD_MAX72XX::INTENSITY, 2);  // brightness (0-15)
  mx.clear();
  
  pinMode(LED_BUILTIN, OUTPUT);

  if (!sensor.begin()) {
    Serial.println("Si7021 sensor not found!");
    while (true);
  }

  dht.begin();

  display.begin();
  display.setIntensity(0);
  display.setTextAlignment(PA_CENTER);
  display.setSpeed(100);  // Lower = faster
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // read temperature and humidity
  float temperature = sensor.readTemperature();
  float humidity = sensor.readHumidity();
  float fahrenheit = celsiusToFahrenheit(temperature);

  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print("C / ");
  Serial.print(fahrenheit);
  Serial.print("F, Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  if (!isnan(humidity) && !isnan(temperature)) {
    // Alternate between temp and humidity every 2 seconds
    displayValue(temperature, " C");
    delay(2000);
    displayValue(fahrenheit, " F");
    delay(2000);
    displayValue(humidity, " %");
    delay(3000);
  }

  // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED_BUILTIN, HIGH);
  // wait for a second
  delay(1000);
  // turn the LED off by making the voltage LOW
  digitalWrite(LED_BUILTIN, LOW);
  // wait for a second
  delay(1000);
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}

void displayValue(float value, const char* unit) {
  char str[10];
  dtostrf(value, 4, 1, str);
  strcat(str, unit);
  display.setTextAlignment(PA_CENTER);  // Center the text
  display.print(str);  // Static display
}

float celsiusToFahrenheit(float celsius) {
  return (celsius * 9.0 / 5.0) + 32.0;
}