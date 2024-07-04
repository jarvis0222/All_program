//Bluetooth variable settings and set UID to sent
#include "BLEDevice.h"
static BLEUUID serviceUUID("fcf6993d-ea05-4940-92f1-c7746b70b8aa");
static BLEUUID    charUUID("5038e12d-498d-4763-b546-cc4f5b6f3ec1");
static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;
int clinet_receive;  //Receive information broadcast by the server

int canon_speed = 300;
int speed_rate = 0;
bool Bool = 0;
int power = 180, a = 1;
#include <DFRobotDFPlayerMini.h>
HardwareSerial myHardwareSerial(1);
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

void setup() {
  Serial.begin(115200);
  pinMode(14 , OUTPUT);  //LED pin position
  pinMode(27 , OUTPUT);
  pinMode(23, OUTPUT);   //Motor pin position (the following)
  pinMode(22, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(15, OUTPUT);
  setup_BLE();  //Start bluetooth
  myHardwareSerial.begin(9600, SERIAL_8N1, 16, 17);  //Serial TX,RX
  myDFPlayer.begin(myHardwareSerial);                //Declare the DFPlayer player in the HardwareSerial control
  delay(500);
  myDFPlayer.volume(30);                             //Set sound volume
  digitalWrite(14 , LOW);
  Serial.println("OK");
}


void loop() {
  BLE_loop();
  Serial.println(clinet_receive);
  if (clinet_receive >= 0) {
    digitalWrite(27 , HIGH);
  }
  if (clinet_receive == 30) {
    digitalWrite(14 , HIGH);  //Turn on the light(piano)
    //piano to the middle of the stage
    forward();
    delay(1500);
    stopstop();
    delay(500);
    left2();
    delay(1500);
    stopstop();
    myDFPlayer.play(65);  //Play starting music
    delay(4000);
  }
  else if (clinet_receive == 31) {
    //piano return to original position
    right2();
    delay(1500);
    stopstop();
    delay(200);
    back();
    delay(2000);
    stopstop();
    digitalWrite(14 , LOW); //Turn on the light(piano)
  }
  //music scale program.................................................
  else if (clinet_receive == 51) {
    myDFPlayer.play(11);
    delay(400);
    while (clinet_receive == 51) {
      BLE_loop();
    }
  }
  else if (clinet_receive == 52) {
    myDFPlayer.play(64);
    delay(400);
    while (clinet_receive == 52) {
      BLE_loop();
    }
  }
  else if (clinet_receive == 53) {
    myDFPlayer.play(63);
    delay(400);
    while (clinet_receive == 53) {
      BLE_loop();
    }
  }
  else if (clinet_receive == 54) {
    myDFPlayer.play(62);
    delay(400);
    while (clinet_receive == 54) {
      BLE_loop();
    }
  }
  else if (clinet_receive == 55) {
    myDFPlayer.play(61);
    delay(400);
    while (clinet_receive == 55) {
      BLE_loop();
    }
  }
  else if (clinet_receive == 56) {
    myDFPlayer.play(60);
    delay(400);
    while (clinet_receive == 56) {
      BLE_loop();
    }
  }
  else if (clinet_receive == 57) {
    myDFPlayer.play(59);
    delay(400);
    while (clinet_receive == 57) {
      BLE_loop();
    }
  }
  else if (clinet_receive == 58) {
    myDFPlayer.play(58);
    delay(400);
    while (clinet_receive == 58) {
      BLE_loop();
    }
  }
  //....................................................................
  else if (clinet_receive == 100) {
    while (a <= 27) {
      Serial.println("in");
      digitalWrite(14 , HIGH); //(Turn on the light(piano)
      canon_piano();
      digitalWrite(14 , LOW); //Turn off the light(piano)
      clinet_receive == 0;
      break;
    }
    Serial.println("OUT");
  }
  else if (clinet_receive == 200) {
    //Play pop music
    digitalWrite(14 , HIGH);  //Turn on the light(piano)
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
    right();
    delay(100);
    stopstop();
    else {
      BLE_loop();
      Serial.println(clinet_receive);
    }
  }
}

void canon_piano() {
  for (int i = 1; i <= 57; i++) {
    a = i + 1;
    Serial.println("canon_piano");
    Serial.print("i : ");
    Serial.println(i);
    BLE_loop();
    if (clinet_receive == 41) {
      //Play at a faster pace
      BLE_loop();
      Serial.println(clinet_receive);
      canon_speed = 300;
    }
    else if (clinet_receive == 42) {
      ////Play at a slower pace
      BLE_loop();
      Serial.println(clinet_receive);
      canon_speed = 500;
    }
    if (Bool == 0) {
      if (i == 57) {
        speed_rate = 6;
        Bool = 1;
      }
      else if (i >= 2 && i <= 56 && i != 4 && i != 15 && i != 18 && i != 29 && i != 32 && i != 43 && i != 46) {
        speed_rate = 1;
        Bool = 1;
      }
      else if ( i >= 0) {
        speed_rate = 2;
        Bool = 1;
      }
    }
    Serial.print("Bool1 : ");
    Serial.println(Bool);
    if (Bool == 1) {
      switch (speed_rate) {
        case 6:
          myDFPlayer.play(i);
          delay(canon_speed * speed_rate);
          Bool = 0;
          break;
        case 1:
          myDFPlayer.play(i);
          delay(canon_speed * speed_rate);
          Bool = 0;
          break;
        case 2:
          myDFPlayer.play(i);
          delay(canon_speed * speed_rate);
          Bool = 0;
          break;
      }
    }
    Serial.print("Bool0 : ");
    Serial.println(Bool);
  }
}

//function for moving the motor forward, backward, left and right
void forward() {
  //forward
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
  //backward
  analogWrite(13, 0);
  analogWrite(12, power);
  analogWrite(15, 0);
  analogWrite(4, power);
  analogWrite(22, 0);
  analogWrite(23, power + 3);
  analogWrite(19, 0);
  analogWrite(18, power + 3);
}
void right() {
  //Turn right
  analogWrite(13, 0);
  analogWrite(12, power);
  analogWrite(15, 0);
  analogWrite(4, power);
  analogWrite(22, power);
  analogWrite(23, 0);
  analogWrite(19, power);
  analogWrite(18, 0);
}
void left() {
  //Turn left
  analogWrite(13, power);
  analogWrite(12, 0);
  analogWrite(15, power);
  analogWrite(4, 0);
  analogWrite(22, 0);
  analogWrite(23, power);
  analogWrite(19, 0);
  analogWrite(18, power);
}
void right2() {
  //Pan right
  analogWrite(13, 0);
  analogWrite(12, power + 25);
  analogWrite(15, power + 30);
  analogWrite(4, 0);
  analogWrite(22, power + 25);
  analogWrite(23, 0);
  analogWrite(19, 0);
  analogWrite(18, power + 25);
}
void left2() {
  //Pan left
  analogWrite(13, power);
  analogWrite(12, 0);
  analogWrite(15, 0);
  analogWrite(4, power + 5);
  analogWrite(22, 0);
  analogWrite(23, power);
  analogWrite(19, power);
  analogWrite(18, 0);
}
void stopstop() {
  //stop
  analogWrite(22, 0);
  analogWrite(23, 0);
  analogWrite(18, 0);
  analogWrite(19, 0);
  analogWrite(12, 0);
  analogWrite(13, 0);
  analogWrite(4, 0);
  analogWrite(15, 0);
}
