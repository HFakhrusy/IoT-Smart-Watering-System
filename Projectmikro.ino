#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// Initialize the LCD display
LiquidCrystal_I2C lcd(0x27, 16, 2);

char auth[] = "v-f7H_K_BQH2Iq74L73o1IjNZiDPCNfT";
char ssid[] = "ESP";
char pass[] = "12332145";

BlynkTimer timer;
bool Relay = 0;

// Define component pins
#define sensor 34    
#define waterPump 14 
#define DHTPIN 4     
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(9600);
  pinMode(waterPump, OUTPUT);
  digitalWrite(waterPump, HIGH);
  lcd.init();
  lcd.backlight();
  dht.begin();

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  lcd.setCursor(1, 0);
  lcd.print("System Loading");
  for (int a = 0; a <= 15; a++)
  {
    lcd.setCursor(a, 1);
    lcd.print(".");
    delay(500);
  }
  lcd.clear();

  timer.setInterval(100L, soilMoistureSensor);
  timer.setInterval(2000L, readDHTSensor);
}

BLYNK_WRITE(V1)
{
  Relay = param.asInt();

  if (Relay == 1)
  {
    digitalWrite(waterPump, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Motor is ON ");
  }
  else
  {
    digitalWrite(waterPump, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Motor is OFF");
  }
}

void soilMoistureSensor()
{
  int value = analogRead(sensor);
  value = map(value, 0, 4095, 0, 100);
  value = (value - 100) * -1;

  Blynk.virtualWrite(V0, value);
  lcd.setCursor(0, 0);
  lcd.print("Moisture :");
  lcd.print(value);
  lcd.print(" ");
}

void readDHTSensor()
{
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  Blynk.virtualWrite(V2, temperature);
  Blynk.virtualWrite(V3, humidity);
}

void loop()
{
  Blynk.run();
  timer.run();
}
