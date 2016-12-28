
#include <DHT11.h> // 온습도 센서 헤더파일 호출
#include <LiquidCrystal.h> // CLCD 헤더파일 호출
#include <Servo.h> // 서보모터 헤더파일
#define RELAY1 5      // 릴레이1에 5V 신호를 보낼 핀 정의
#define RELAY2 6      // 릴레이2에 5V 신호를 보낼 핀 정의
#define servoPin 2    // 서보모터 제어 핀 지정
#define pin1 4        // 센서 1
#define pin2 3        // 센서 2
#define pin3 13       // 센서 3

DHT11 dht11_1(pin1);  // 온습도센서 핀 정의
DHT11 dht11_2(pin2);
DHT11 dht11_3(pin3);
LiquidCrystal lcd(7, 8, 9, 10, 11, 12); // CLCD 사용 핀 지정

int RELAY1_State = LOW;    // 릴레이1 가동
int RELAY2_State = LOW;    // 릴레이2 가동
Servo servo;

float temp, humi; // 편균 온도와 습도
float temp1, humi1;  // 센서1의 온도와 습도
float temp2, humi2; // 센서2의 온도와 습도
float temp3, humi3;// 센서3의 온도와 습도

int angle = 0; // 서보모터 구동 각 저장 변수
//////////////////적정 온습도 구간///////////////////////////////
int high_t = 70;  // 적정 산한 온도
int low_t = 50;   // 적정 하한 온도
int high_h = 30;  // 적정 상한 습도
int low_h = 10;   // 적정 하한 습도
/////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600); // 시작 전 딜레이
  pinMode (RELAY1, OUTPUT); // RELAY1를 output으로 설정한다.
  pinMode (RELAY2, OUTPUT); // RELAY2를 output으로 설정한다.
  pinMode(pin1, OUTPUT);    // 센서1 제어 핀을 output으로 설정
  pinMode(pin2, OUTPUT);    // 센서2 제어 핀을 output으로 설정
  pinMode(pin3, OUTPUT);    // 센서2 제어 핀을 output으로 설정
  lcd.begin(16, 2);         // 16x2 lcd 사용
  servo.attach(servoPin);  // 서보모터 제어 핀 servoPin 사용
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
}

void loop()
{
  Display();    // clcd 출력함수
  if (temp > high_t && humi > high_h) { // 고온+다습
    FAN_Run();   // 환풍 Fan 작동
    Spray_Off(); // 분무기의 서보모터 작동x
    Heater_Off(); // 히터인 전기방석 작동x
    lcd.setCursor(0, 1);  // lcd화면 내 커서 위치
    lcd.print("Hot and Wet"); // 상태를 나타내는 글자 출력
  }
  else if (temp > high_t && humi <= high_h && humi >= low_h) { // 고온
    FAN_Stop();
    Spray_On();
    Heater_Off();
    lcd.setCursor(0, 1);
    lcd.print("Hot");
  }
  else if (temp > high_t && humi < low_h) { // 고온+건조
    FAN_Run();
    Spray_On();
    Heater_Off();
    lcd.setCursor(0, 1);
    lcd.print("Hot and Dry");
  }
  else if (temp >= low_t && temp <= high_t && humi > high_h) { // 다습
    FAN_Run();
    Spray_Off();
    Heater_On();
    lcd.setCursor(0, 1);
    lcd.print("Wet");
  }
  else if (temp >= low_t && temp <= high_t && humi <= high_h && humi >= low_h) { // 중중
    FAN_Stop();
    Spray_Off();
    Heater_Off();
    lcd.setCursor(0, 1);
    lcd.print("GOOD~!");
  }
  else if (temp >= low_t && temp <= high_t && humi < low_h) { // 중하
    FAN_Stop();
    Spray_On();
    Heater_Off();
    lcd.setCursor(0, 1);
    lcd.print("Dry");
  }
  else if (temp < low_t && humi > high_h) { // 하상
    FAN_Stop();
    Spray_Off();
    Heater_On();
    lcd.setCursor(0, 1);
    lcd.print("Cold and Wet");
  }
  else if (temp < low_t && humi <= high_h && humi >= low_h) { // 하중
    FAN_Stop();
    Spray_Off();
    Heater_On();
    lcd.setCursor(0, 1);
    lcd.print("Cold");
  }
  else if (temp < low_t && humi < low_h) { // 하하
    FAN_Stop();
    Spray_On();
    Heater_On();
    lcd.setCursor(0, 1);
    lcd.print("Cold and Dry");
  }
  else {
    lcd.setCursor(4, 0);
    lcd.print("ERROR!!");
    delay(100);
  }
  //lcd.clear();
  delay(960);
}
void Display() { // 센서로부터 읽은 값을 출력하는 함수
  int err;
  if ((err = dht11_1.read(humi1, temp1)) == 0)
  {
    Serial.print("temperature1 :");
    Serial.print(temp1);
    Serial.print(" humidity1 :");
    Serial.print(humi1);
    Serial.println();
  }
  else
  {
    Serial.println();
    Serial.print("Error No :");
    Serial.print(err);
    Serial.println();
  }

  if ((err = dht11_2.read(humi2, temp2)) == 0)
  {
    Serial.print("temperature2 :");
    Serial.print(temp2);
    Serial.print(" humidity2 :");
    Serial.print(humi2);
    Serial.println();
  }
  else
  {
    Serial.println();
    Serial.print("Error No :");
    Serial.print(err);
    Serial.println();
  }

  if ((err = dht11_3.read(humi3, temp3)) == 0)
  {
    Serial.print("temperature3 :");
    Serial.print(temp3);
    Serial.print(" humidity3 :");
    Serial.print(humi3);
    Serial.println();
    Serial.println();

    temp = (temp1 + temp2 + temp3) / 3; // 세 센서의 값의 평균을 구하여 적용
    humi = (humi1 + humi2 + humi3) / 3;
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.setCursor(2, 0);
    lcd.print(temp);
    lcd.setCursor(7, 0);
    lcd.print(" H:");
    lcd.setCursor(10, 0);
    lcd.print(humi); // 위의 한줄에 온 습도 표시

    Serial.print("temperature :");
    Serial.print(temp);
    Serial.print(" humidity :");
    Serial.print(humi);
    Serial.println();
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
}

// FAN 작동 함수
void FAN_Run() {
  digitalWrite (RELAY2, LOW); // 릴레이 ON
  delay (1000);              //10초 delay
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
  for (angle = 0; angle < 180; angle++)
  {
    servo.write(angle);
    delay(0);
  }
}

void Spray_Off() {
  servo.write(0);
  delay(100);
}



