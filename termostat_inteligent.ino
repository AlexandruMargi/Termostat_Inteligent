#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Configurare DHT22
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Configurare HC-SR04
#define TRIG_PIN 3
#define ECHO_PIN 4
#define DIST_THRESHOLD 50 // prag distanta in cm

// Configurare LCD I2C
LiquidCrystal_I2C lcd(0x27, 16, 2); // adresa 0x27, 16x2

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  dht.begin();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = duration * 0.034 / 2;
  return distance;
}

void loop() {
  long distance = getDistance();
  Serial.print("Distanta: "); Serial.print(distance); Serial.println(" cm");

  if(distance > 0 && distance < DIST_THRESHOLD) {
    // Citire temperatura si umiditate
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    if(isnan(temp) || isnan(hum)) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Citire esuata");
    } else {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Temp: "); lcd.print(temp); lcd.print("C");
      lcd.setCursor(0,1);
      lcd.print("Umid: "); lcd.print(hum); lcd.print("%");
    }
  } else if(distance > DIST_THRESHOLD) {
    // Nimeni nu e aproape → afișează distanta
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Distanta: ");
    lcd.setCursor(0,1);
    lcd.print(distance); lcd.print(" cm");
  } else {
    // Citire esuata
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Distanta invalida");
  }

  delay(1000);
}
