#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
  #include <ESPAsyncWebServer.h>
  #include <AsyncElegantOTA.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
  #include <AsyncElegantOTA.h>
#endif

AsyncWebServer server(80);

#define BLYNK_TEMPLATE_ID "TMPL3Kgt330bN"
#define BLYNK_TEMPLATE_NAME "RAILWAY PLATFORM SAFETY"
#define BLYNK_AUTH_TOKEN "8NMx9GZbYWrjR_6bnKXg33Es-TmCMN5n"

#define BLYNK_PRINT Serial

#ifdef ESP32
  #include <WiFi.h>
  #include <BlynkSimpleEsp32.h>
#else
  #include <ESP8266WiFi.h>
  #include <BlynkSimpleEsp8266.h>
#endif

char auth[] = "8NMx9GZbYWrjR_6bnKXg33Es-TmCMN5n";
char ssid[] = "projectb20";
char pass[] = "projectb20";

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define sw 5
int sense;

int j = 0;

#define buzzer 13
#define red_led 4
#define yellow_led 16
#define green_led 17

#define temp 35
int temp_sense;

#define ldr 26
int ldr_sense;

#define led 33
#define fan 32

#include <ESP32Servo.h>
Servo servo1;
Servo servo2;

int a = 0;
int door = 0;

WidgetLCD lcd1(V0);
WidgetLED indicator(V1);

void setup() {
  Serial.begin(9600);

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  pinMode(fan, OUTPUT);
  digitalWrite(fan, LOW);

  pinMode(temp, INPUT);
  pinMode(ldr, INPUT);

  lcd.init();
  lcd.backlight();
  delay(50);

  pinMode(red_led, OUTPUT);
  pinMode(yellow_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  delay(50);
  digitalWrite(red_led, HIGH);
  digitalWrite(yellow_led, LOW);
  digitalWrite(green_led, LOW);
  delay(50);

  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  delay(50);

  pinMode(sw, INPUT_PULLUP);
  delay(50);

  lcd.clear();
  lcd.print("INITIATING SERVO");
  delay(100);

  servo1.attach(19);
  delay(50);
  servo2.attach(18);
  delay(50);

  servo1.write(120);
  delay(100);
  servo2.write(120);
  delay(100);
  delay(1000);

  lcd.clear();
  lcd.print("CONNECTING TO...");
  lcd.setCursor(0, 1);
  lcd.print(ssid);
  delay(1000);

  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }

  Blynk.begin(auth, ssid, pass);
  Serial.println("READY");

  lcd.clear();
  lcd1.clear();
  lcd.print(" WIFI CONNECTED ");
  lcd1.print(0, 0, " WIFI CONNECTED ");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  lcd1.print(0, 1, WiFi.localIP());
  delay(5000);
  lcd.clear();
  lcd1.clear();

  lcd.clear();
  lcd.print(" RAIL PLATFORM ");
  lcd.setCursor(0, 1);
  lcd.print(" SAFETY SYSTEM ");
  delay(2000);
  lcd.clear();
  lcd.print(" * READY * ");
  delay(2000);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! I am ESP32.");
  });

  AsyncElegantOTA.begin(&server);
  server.begin();
  Serial.println("HTTP server started");
}

BLYNK_CONNECTED() {
  Blynk.syncVirtual(V2);
}

BLYNK_WRITE(V2) {
  int door = param.asInt();
  if (door == 1) {
    j = j + 1;
    if (j >= 4) {
      j = 0;
    }
  }
}

void sensor_data() {
  ldr_sense = digitalRead(ldr);
  Serial.print("LDR : ");
  Serial.println(ldr_sense);

  temp_sense = analogRead(temp);
  temp_sense = temp_sense * 0.080;
  Serial.print("TEMP : ");
  Serial.println(temp_sense);

  Blynk.virtualWrite(V3, temp_sense);

  if (ldr_sense == 1) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }
  if (temp_sense >= 30) {
    digitalWrite(fan, HIGH);
  } else {
    digitalWrite(fan, LOW);
  }
}

void loop() {
  sensor_data();

  sense = digitalRead(sw);
  sense = 1 - sense;
  Serial.print("SW : ");
  Serial.println(sense);

  lcd.clear();
  lcd.print(" * WELCOME TO * ");
  lcd.setCursor(0, 1);
  lcd.print(" INDIAN RAILWAY ");
  digitalWrite(2, HIGH);
  indicator.on();
  delay(300);
  digitalWrite(2, LOW);
  indicator.off();
  delay(300);

  if (sense == 1) {
    j = j + 1;
    if (j >= 4) {
      j = 0;
    }
  }

  if (j == 1) {
    j = 2;
    digitalWrite(red_led, LOW);
  }
}
  lcd.clear();
  for(int i=5; i>=0; i--)
  {
    digitalWrite(yellow_led,HIGH);
    digitalWrite(buzzer,HIGH);
    lcd.clear();
    lcd.print("OPEN IN : ");lcd.print(i);
    delay(500);
    digitalWrite(buzzer,LOW);
    digitalWrite(yellow_led,LOW);
    delay(500);
    lcd.clear();
    lcd.print(" HAPPY  JOURNEY ");
  }
  for(int s=120;s>=0;s=s-10)
  {
    Serial.println(s);
    servo1.write(s);delay(20);
    servo2.write(s);delay(20);
    delay(500);
  }   
  
  digitalWrite(yellow_led,HIGH);
  digitalWrite(green_led,HIGH);
} else if(j==3) { j=0; digitalWrite(red_led,HIGH);
  lcd.clear();
  for(int i=5; i>0; i--)
  {
    digitalWrite(buzzer,HIGH);
    lcd.clear();
    lcd.print("CLOSE IN : ");lcd.print(i);
    delay(500);
    digitalWrite(buzzer,LOW);
    delay(500);
  }
  for(int s=0;s<=120;s=s+10)
  {
    Serial.println(s);
    servo1.write(s);delay(20);
    servo2.write(s);delay(20);
    delay(500);
  }
} else { pass; } digitalWrite(2,HIGH);indicator.on();delay(300);digitalWrite(2,LOW);indicator.off();delay(300); Blynk.run(); }