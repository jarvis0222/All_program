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
int clinet_receive, t = 300;  //接收由server廣播的資訊
int power = 180;
#include <DFRobotDFPlayerMini.h>
HardwareSerial myHardwareSerial(1); //ESP32可宣告需要一個硬體序列，軟體序列會出錯
DFRobotDFPlayerMini myDFPlayer;//啟動DFPlayer撥放器
void printDetail(uint8_t type, int value);//宣告播放控制程式

void setup() {
  Serial.begin(115200);
  pinMode(14 , OUTPUT); //LED腳位
  pinMode(27 , OUTPUT);
  //馬達腳位.........................................................................................................
  pinMode(23, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(15, OUTPUT);
  //................................................................................................................
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
  if (clinet_receive == 10) {
    digitalWrite(14 , HIGH); //no.1(trumpet)開燈
    //trumpet到中間................................................................
    forward();
    delay(1500);
    stopstop();
    delay(500);
    right2();
    delay(1500);
    stopstop();
    myDFPlayer.play(17);
    delay(4000);
    //............................................................................
  }
  else if (clinet_receive == 11) {
    //trumpet回原位................................................................
    left2();
    delay(1500);
    stopstop();
    delay(500);
    back();
    delay(1500);
    stopstop();
    //............................................................................
    digitalWrite(14 , LOW); //no.1(trumpet)關燈
  }
  else if (clinet_receive == 100) {
    digitalWrite(14 , HIGH);
    for (int j = 1; j <= 8; j++) {
      myDFPlayer.play(j);
      delay(t * 8);
      if (clinet_receive == 41) { //收到小數值播較快節奏
        BLE_loop();
        Serial.println(clinet_receive);
        t = 300;
      }
      else if (clinet_receive == 42) { //收到大數值播較慢節奏
        BLE_loop();
        Serial.println(clinet_receive);
        t = 500;
      }
      Serial.println(j);
    }
    delay(1500);
    digitalWrite(14 , LOW);
    clinet_receive = 0;
  }
  else if (clinet_receive == 71) {
    myDFPlayer.play(16);
    delay(600);
    while (clinet_receive == 71) {
      BLE_loop();
    }
  }
  else if (clinet_receive == 72) {
    myDFPlayer.play(15);
    delay(600);
    while (clinet_receive == 72) {
      BLE_loop();
    }
  }
  else if (clinet_receive == 73) {
    myDFPlayer.play(14);
    delay(600);
    while (clinet_receive == 73) {
      BLE_loop();
    }
  }
  else if (clinet_receive == 74) {
    myDFPlayer.play(13);
    delay(600);
    while (clinet_receive == 74) {
      BLE_loop();
    }
  }
  else if (clinet_receive == 75) {
    myDFPlayer.play(12);
    delay(600);
    while (clinet_receive == 75) {
      BLE_loop();
    }
  }
  else if (clinet_receive == 76) {
    myDFPlayer.play(11);
    delay(600);
    while (clinet_receive == 76) {
      BLE_loop();
    }
  }
  else if (clinet_receive == 77) {
    myDFPlayer.play(10);
    delay(600);
    while (clinet_receive == 77) {
      BLE_loop();
    }
  }
  else if (clinet_receive == 78) {
    myDFPlayer.play(9);
    delay(600);
    while (clinet_receive == 78) {
      BLE_loop();
    }
  }
  else if (clinet_receive == 200) {
    digitalWrite(14 , HIGH); //no.2(violin)開燈
    //....................................................車體左右擺動
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
    //........................................................................
  }
  else {
    BLE_loop();
    Serial.println(clinet_receive);
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
  analogWrite(23, power + 5);
  analogWrite(19, 0);
  analogWrite(18, power + 5);
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
