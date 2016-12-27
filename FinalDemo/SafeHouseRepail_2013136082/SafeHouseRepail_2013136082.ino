#include <LiquidCrystal.h>
#include <DHT11.h>             // 온습도센서 라이브러리
#include <Servo.h>

#define SERVO_PIN 2;
#define pin 13     // DHT의 DATA핀을 4번으로 정의
//#define pin2 3
//#define pin3 13
#define RELAY1 5      // 릴레이1에 5V 신호를 보낼 핀 정의
#define RELAY2 6      // 릴레이1에 5V 신호를 보낼 핀 정의
#define LED 30        // LED 핀 정의
#define servoPin 2
DHT11 dht11(pin);
//DHT11 dht11_2(pin2);
//DHT11 dht11_3(pin3);
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

int RELAY1_State = LOW;    // 릴레이1(팬모터) 상태
int RELAY2_State = LOW;    // 릴레이2(온열장판) 상태
int ledState = LOW;     // LED 상태
bool r2Run = false;
Servo servo;
int angle = 0;

////////////////////////////////
int high_t = 10;   //  적정 상한 온도
int low_t = 0;    //  적정 하한 온도
int high_h = 90;  //  적정 상한 습도
int low_h = 80;   //  적정 하한 습도
//////////////////////////////////////

float temp, humi;

void setup ()
{
  Serial.begin(9600);
  pinMode (RELAY1, OUTPUT); // RELAY1를 output으로 설정한다.
  pinMode (RELAY2, OUTPUT); // RELAY1를 output으로 설정한다.
  pinMode(pin, OUTPUT);
  //  pinMode(pin2, OUTPUT);
  // pinMode(pin3, OUTPUT);

  lcd.begin(16, 2);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  digitalWrite (RELAY1, HIGH);  // RELAY111 OFF
  digitalWrite (RELAY2, HIGH);  // RELAY111 OFF
  servo.attach(servoPin);
}
void loop()
{
  int err;
  float temp, humi;
  if ((err = dht11.read(humi, temp)) == 0)
  {
    Serial.print("temperature:");
    Serial.print(temp);
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(temp);
    Serial.print(" humidity:");
    Serial.print(humi);
    //lcd.setCursor(0, 1);
    lcd.print(" H:");
    lcd.print(humi);
    Serial.println();
  }
  else
  {
    Serial.println();
    Serial.print("Error No :");
    Serial.print(err);
    Serial.println();
  }
  delay(DHT11_RETRY_DELAY); //delay for reread

  //  Display();

  if (temp > high_t && humi > high_h) { // 상상
    FAN_Run();
    Spray_Off();
    Heater_Off();
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Hot and Wet..");
  }
  else if (temp > high_t && humi <= high_h && humi >= low_h) { // 상중
    FAN_Run();
    Spray_On();
    Heater_Off();
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Too Hot..");
  }
  else if (temp > high_t && humi < low_h) { // 상하
    FAN_Run();
    Spray_On();
    Heater_Off();
    lcd.clear();
    lcd.setCursor(1, 1);
    lcd.print("Hot and Dry..");
  }
  else if (temp >= low_t && temp <= high_t && humi > high_h) { // 중상
    FAN_Run();
    Spray_Off();
    Heater_On();
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Too Wet..");
  }
  else if (temp >= low_t && temp <= high_t && humi <= high_h && humi >= low_h) { // 중중
    FAN_Stop();
    Spray_Off();
    Heater_Off();
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Very Good!");
  }
  else if (temp >= low_t && temp <= high_t && humi < low_h) { // 중하
    FAN_Stop();
    Spray_On();
    Heater_Off();
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Too Dry..");
  }
  else if (temp < low_t && humi > high_h) { // 하상
    FAN_Stop();
    Spray_Off();
    Heater_On();
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Cold and Wet..");
  }
  else if (temp < low_t && humi <= high_h && humi >= low_h) { // 하중
    FAN_Stop();
    Spray_Off();
    Heater_On();
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Too Cold..");
  }
  else if (temp < low_t && humi < low_h) { // 하하
    FAN_Stop();
    Spray_On();
    Heater_On();
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Cold and Dry..");
  }
  else {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("ERROR!!");
    delay(1000);
  }
  lcd.clear();


  /*
    int err;
    float temp, humi;
    if((err=dht11.read(humi, temp))==0)
    {
      Serial.print("temperature:");
      Serial.print(temp);
      Serial.print(" humidity:");
      Serial.print(humi);
      Serial.println();
    }
    else
    {
      Serial.println();
      Serial.print("Error No :");
      Serial.print(err);
      Serial.println();
    }
    delay(DHT11_RETRY_DELAY); //delay for reread}
  */


  /*
    // 온습도 상태 출력 함수
    void Display() {
    while (!Serial) {;}
    int err;
    if((err=dht11.read(humi, temp))==0)
    {
      Serial.print("temperature1 :");
      Serial.print(temp);
      Serial.print("humidity1 :");
      Serial.print(humi);
    }
  */
  delay(DHT11_RETRY_DELAY); //delay for reread
  /*
     if((err=dht11_2.read(humi, temp))==0)
    {
      Serial.print("temperature2 :");
      Serial.print(temp);
      lcd.setCursor(0, 0);
      Serial.print("humidity2 :");
      Serial.print(humi);
    }

    delay(DHT11_RETRY_DELAY); //delay for reread

     if((err=dht11_3.read(humi, temp))==0)
    {
      Serial.print("temperature3 :");
      Serial.print(temp);
      lcd.setCursor(0, 0);
      Serial.print("humidity3 :");
      Serial.print(humi);
    }
    else
    {
      Serial.println();
      Serial.print("Error No :");
      Serial.print(err);
      Serial.println();
    }
    delay(DHT11_RETRY_DELAY); //delay for reread


  */


  lcd.print("T : ");
  lcd.setCursor(4, 0);
  lcd.print(temp);
  lcd.setCursor(6, 0);
  lcd.print("H : ");
  lcd.setCursor(10, 0);
  lcd.print(humi);
  Serial.println();
}

// FAN 작동 함수
void FAN_Run() {
  digitalWrite (RELAY2, LOW); // 릴레이 ON
  delay (10000);              //10초 delay
  digitalWrite (RELAY2, HIGH); // 릴레이 OFF
  delay (5000);               //5초 delay
}

// FAN 정지 함수
void FAN_Stop() {
  digitalWrite (RELAY2, HIGH); // 릴레이 OFF
}

// Heater 작동함수
void Heater_On() {
  digitalWrite (RELAY1, LOW); // 릴레이 ON
}

// Heater 정지 함수
void Heater_Off() {
  digitalWrite (RELAY1, HIGH); // 릴레이 OFF
}

// 분무장치 On
void Spray_On() {
  for (angle = 0; angle < 90; angle++)
  {
    servo.write(angle);
    delay(5);
  }
}

void Spray_Off() {
  servo.write(0);
  delay(15);
}







