#include "BLEDevice.h"
// The remote service we wish to connect to.
static BLEUUID serviceUUID("fcf6993d-ea05-4940-92f1-c7746b70b8aa");
// The characteristic of the remote service we are interested in.
static BLEUUID    charUUID("5038e12d-498d-4763-b546-cc4f5b6f3ec1");
static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;
int clinet_receive;   //接收由server廣播的資訊
int canon_speed = 300, speed_rate = 0, Bool = 0;
int power = 180, a = 1;
#include <DFRobotDFPlayerMini.h>
HardwareSerial myHardwareSerial(1); //ESP32可宣告需要一個硬體序列，軟體序列會出錯
DFRobotDFPlayerMini myDFPlayer;//啟動DFPlayer撥放器
void printDetail(uint8_t type, int value);//宣告播放控制程式
void setup() {
  Serial.begin(115200);
  pinMode(14 , OUTPUT); //LED腳
  pinMode(27 , OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(15, OUTPUT);

  setup_BLE();
  myHardwareSerial.begin(9600, SERIAL_8N1, 16, 17); // Serial的TX,RX ，實際上只用到TX傳送指令，因此RX可不接（接收player狀態）
  myDFPlayer.begin(myHardwareSerial);//將DFPlayer播放器宣告在HardwareSerial控制
  delay(500);
  myDFPlayer.volume(30);  //設定聲音大小（0-30）
  digitalWrite(14 , LOW);
  Serial.println("OK");
}
void loop() {
  BLE_loop();
  Serial.println(clinet_receive);
  if (clinet_receive >= 0) {
    digitalWrite(27 , HIGH);
  }
  if (clinet_receive == 20) {
    digitalWrite(14 , HIGH); //no.2(violin)開燈
    forward();
    delay(1500);
    stopstop();
    myDFPlayer.play(36);
    delay(4000);
  }
  else if (clinet_receive == 21) {
    back();
    delay(1500);
    stopstop();
    digitalWrite(14 , LOW); //no.2(violin)關燈
  }
  else if (clinet_receive == 61) {
    myDFPlayer.play(35);
    delay(600);
    while (clinet_receive == 61) {
      BLE_loop();
    }
  }
  else if (clinet_receive == 62) {
    myDFPlayer.play(34);
    delay(600);
    while (clinet_receive == 62) {
      BLE_loop();
    }
  }
  else if (clinet_receive == 63) {
    myDFPlayer.play(33);
    delay(600);
    while (clinet_receive == 63) {
      BLE_loop();
    }
  }
  else if (clinet_receive == 64) {
    myDFPlayer.play(32);
    delay(600);
    while (clinet_receive == 64) {
      BLE_loop();
    }
  }
  else if (clinet_receive == 65) {
    myDFPlayer.play(31);
    delay(600);
    while (clinet_receive == 65) {
      BLE_loop();
    }
  }
  else if (clinet_receive == 66) {
    myDFPlayer.play(30);
    delay(600);
    while (clinet_receive == 66) {
      BLE_loop();
    }
  }
  else if (clinet_receive == 67) {
    myDFPlayer.play(29);
    delay(600);
    while (clinet_receive == 67) {
      BLE_loop();
    }
  }
  else if (clinet_receive == 68) {
    myDFPlayer.play(29);
    delay(600);
    while (clinet_receive == 68) {
      BLE_loop();
    }
  }
  else if (clinet_receive == 100) {
    while (a <= 28) {
      Serial.println("in");
      digitalWrite(14 , HIGH); //no.2(violin)開燈
      canon_violin();
      digitalWrite(14 , LOW); //no.2(violin)關燈
      clinet_receive = 0;
      break;
    }
    Serial.println("OUT");
  }
  else if (clinet_receive == 200) {
    digitalWrite(14 , HIGH); //no.2(violin)開燈
    myDFPlayer.play(37);
    //........................................................車體左右擺動
    for (int i = 0; i <= 12; i++) {
      right();
      delay(400);
      stopstop();
      delay(100);
      left();
      delay(800);
      stopstop();
      delay(100);
      right();
      delay(400);
      stopstop();
      delay(100);
    }
    left();
    delay(100);
    stopstop();
    //.......................................................
  }
  else {
    BLE_loop();
    Serial.println(clinet_receive);
  }
}

void canon_violin() {
  for (int i = 1; i <= 27; i++) {
    a = i + 1;
    Serial.println("canon_piano");
    Serial.print("i : ");
    Serial.println(i);
    BLE_loop();
    if (clinet_receive == 41) { //收到較數值播較快節奏
      BLE_loop();
      Serial.println(clinet_receive);
      canon_speed = 300;
    }
    else if (clinet_receive == 42) { //收到大數值播較慢節奏
      BLE_loop();
      Serial.println(clinet_receive);
      canon_speed = 500;
    }
    if (Bool == 0) {
      if (i == 3 || i == 6 || i == 9 || i == 12 || i == 15 || i == 18 || i == 21 || i == 24 || i == 27) {
        speed_rate = 4;
        Bool = 1;
      }
      else if ( i >= 0) {
        speed_rate = 2;
        Bool = 1;
      }
    }
    if (Bool == 1) {
      switch (speed_rate) {
        case 4:
          myDFPlayer.play(i);
          delay(4 * canon_speed);
          Bool = 0;
          break;
        case 2:
          myDFPlayer.play(i);
          delay(2 * canon_speed);
          Bool = 0;
          break;
      }
    }
  }
}

void forward() {
  analogWrite(13, power);
  analogWrite(12, 0);
  analogWrite(15, power);
  analogWrite(4, 0);
  analogWrite(22, power);
  analogWrite(23, 0);
  analogWrite(19, power);
  analogWrite(18, 0);
}
void back() {
  analogWrite(13, 0);
  analogWrite(12, power);
  analogWrite(15, 0);
  analogWrite(4, power);
  analogWrite(22, 0);
  analogWrite(23, power);
  analogWrite(19, 0);
  analogWrite(18, power);
}
void right() {//原地轉
  analogWrite(13, 0);
  analogWrite(12, power);
  analogWrite(15, 0);
  analogWrite(4, power);
  analogWrite(22, power);
  analogWrite(23, 0);
  analogWrite(19, power);
  analogWrite(18, 0);
}
void left() {//原地轉
  analogWrite(13, power);
  analogWrite(12, 0);
  analogWrite(15, power);
  analogWrite(4, 0);
  analogWrite(22, 0);
  analogWrite(23, power);
  analogWrite(19, 0);
  analogWrite(18, power);
}
void right2() {//右平移
  analogWrite(13, 0);
  analogWrite(12, power);
  analogWrite(15, power);
  analogWrite(4, 0);
  analogWrite(22, power);
  analogWrite(23, 0);
  analogWrite(19, 0);
  analogWrite(18, power);
}
void left2() {//左平移
  analogWrite(13, power);
  analogWrite(12, 0);
  analogWrite(15, 0);
  analogWrite(4, power);
  analogWrite(22, 0);
  analogWrite(23, power);
  analogWrite(19, power);
  analogWrite(18, 0);
}

void stopstop() {
  analogWrite(22, 0);
  analogWrite(23, 0);
  analogWrite(18, 0);
  analogWrite(19, 0);
  analogWrite(12, 0);
  analogWrite(13, 0);
  analogWrite(4, 0);
  analogWrite(15, 0);
}
