// B. Joshua Samuel
// 950023104301

#include <Wire.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal.h>

#define DHTPIN A4
#define DHTTYPE DHT22
#define BUZZER_PIN 6
#define LED_PIN 5

#define TEMP_THRESHOLD 30.0
#define TEMP_HYSTERESIS 0.5

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

bool overheat = false;

void setup() {
  dht.begin();
  lcd.begin(16, 2);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW); 
}

void loop() {
  delay(2000);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  
  if (isnan(humidity) || isnan(temperature)) {
    lcd.clear();
    lcd.print("Sensor error");
    return;
  }

 
  if (!overheat && temperature >= TEMP_THRESHOLD) {
    overheat = true;
  } else if (overheat && temperature <= (TEMP_THRESHOLD - TEMP_HYSTERESIS)) {
    overheat = false;
  }

  digitalWrite(LED_PIN, overheat ? HIGH : LOW);
  if (overheat) {
    tone(BUZZER_PIN, 1000);  
  } else {
    noTone(BUZZER_PIN);  
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature, 1);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  if (overheat) {
    lcd.print("Overheat!       ");
  } else {
    lcd.print("Hum: ");
    lcd.print(humidity, 1);
    lcd.print(" %  ");
  }
}
