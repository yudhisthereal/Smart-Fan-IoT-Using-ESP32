#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6d1SSQkz-"
#define BLYNK_TEMPLATE_NAME "Smart Fan"
#define BLYNK_AUTH_TOKEN "R2TkTQEzGlbjyqcoBYRTlY-9UUohjrXG"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include <DHT.h>
#include <LiquidCrystal_I2C.h>

#include "config.h"
#include "my_wifi.h"
#include "wifi_communicator.h"

#include <ESP32_Servo.h>

#define DHTTYPE DHT11   

BlynkTimer timer1000;
BlynkTimer timer500;
BlynkTimer timer3;

// Communication messages
char incoming_msg[MAX_BUFFER_LEN] = {0};
char response[MAX_BUFFER_LEN] = {0};

// WiFi credentials
char ssid[] = "AndroidAP";
char pass[] = "NMXPVML0507";

float t = 0.0;
unsigned short t_thres = 30;
DHT dht(DHT_PIN, DHTTYPE);
LiquidCrystal_I2C lcd (0x27, 16, 2);

Servo servo;

unsigned short person_count = 0;
unsigned short turnoff_delay_count = 0;
unsigned int rotation = 90;
unsigned short swing_speed = 10;
bool clockwise = false;
bool is_auto = true;
bool manual_fan_on = false;
bool swing_on = true;

BLYNK_WRITE(V0) {
  is_auto = param.asInt() == 1;
}

BLYNK_WRITE(V1) {
  manual_fan_on = param.asInt() == 1;
}

BLYNK_WRITE(V2) {
  swing_on = param.asInt() == 1;
}

BLYNK_WRITE(V4) {
  t_thres = param.asInt();
}

BLYNK_WRITE(V5) {
  if (!swing_on)
    rotation = param.asInt() + 45;
}

void update_blynk_t() {
  Blynk.virtualWrite(V3, t);
}

void maintain_client_connection() {
  if(!is_client_connected()){ connect_client_nonblocking(); }
}

void updateFanLCD() {
  t = dht.readTemperature();

  lcd.clear();

  lcd.setCursor(0, 0);

  if (!isnan(t)){
    lcd.print(t); 
  }

  lcd.print((char)223);
  lcd.print("C");

  lcd.setCursor(0, 1);

  if (is_auto) {

    lcd.print("Automatic");

    if (t >= t_thres && person_count > 0){
      digitalWrite(FAN_PIN,0);
      turnoff_delay_count = 0;
    } else if (turnoff_delay_count >= TURNOFF_DELAY){
      digitalWrite(FAN_PIN,1);
    } else {
      turnoff_delay_count++;
    }
  } else {

    lcd.print("Manual");

    if (manual_fan_on) {
      digitalWrite(FAN_PIN, 0);
    } else {
      digitalWrite(FAN_PIN, 1);
    }
  }

  lcd.setCursor(0, 2);

  if (is_client_connected())
    lcd.print("Camera Online");
  else 
    lcd.print("Camera Offline");
}

void update_servo() {
  if (swing_on && (manual_fan_on || (person_count > 0 && is_auto))) {
    if (clockwise) {
      rotation -= swing_speed;
    } else {
      rotation += swing_speed;
    }

    if (rotation > 135  || rotation < 45) {
      clockwise = !clockwise;
    }
    Blynk.virtualWrite(V5, rotation - 45);
  }

  servo.write(rotation); 
}

void ngeprint(){
  if (t >= t_thres && person_count > 0 && is_auto)
  {
    lcd.setCursor(1,0);
    lcd.print("gerah");
  }
}

void processWebSocketCommunication() {
  bool received = get_message(incoming_msg);

  if(received){
    uint8_t start = 0;
    if(incoming_msg[0] == 'A'){
      sprintf(response, "%s", ACK);
      start++;
    } else {
      person_count = atoi(incoming_msg);
    }

    // If start is bigger than 0, then we have to acknowledge the reception
    if(start > 0){
      send_message(response);
      // Clear the response buffer
      memset(response, 0, MAX_BUFFER_LEN);
    }
  }
}

void process500() {
  update_blynk_t();
  maintain_client_connection();
}

void process1000() {
  updateFanLCD();
}

void setup(){
  Serial.begin(9600);

  setup_wifi();
  setup_wifi_communicator();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer1000.setInterval(1000UL, process1000);
  timer500.setInterval(500UL, process500);
  timer3.setInterval(3UL, update_servo);

  Blynk.virtualWrite(V0, is_auto);
  Blynk.virtualWrite(V1, manual_fan_on);
  Blynk.virtualWrite(V2, swing_on);
  Blynk.virtualWrite(V4, t_thres);

  pinMode(FAN_PIN,OUTPUT);
  servo.attach(SERVO_PIN);
  digitalWrite(FAN_PIN,0);

  servo.write(90);

  lcd.init();                    
  lcd.backlight();

  dht.begin();
}

void loop(){
  Blynk.run();
  timer500.run();
  timer1000.run();
  timer3.run();

  if (is_auto)
    if(is_client_connected())
      processWebSocketCommunication();
}

