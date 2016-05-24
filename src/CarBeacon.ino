#include <SoftwareSerial.h>
#include <Wire.h>
#include <Arduino.h>
#define MAX_BTCMDLEN 128
SoftwareSerial BTSerial(A14,A15); // Arduino TX/RX
byte cmd[MAX_BTCMDLEN];
int len = 0;
const byte FENB  = 2;
const byte FRin4 = 3; //FR=右前輪
const byte FRin3 = 4;
const byte FLin2 = 5; //FL=左前輪
const byte FLin1 = 6;
const byte FENA  = 7;

const byte BENB  = 8;
const byte BLin4 = 9; //BL=左後輪
const byte BLin3 = 10;
const byte BRin2 = 11;//BR=右後輪
const byte BRin1 = 12;
const byte BENA  = 13;
byte speed;

void setup() {
	Serial.begin(9600);
    BTSerial.begin(9600);
	pinMode(FLin1, OUTPUT);
	pinMode(FLin2, OUTPUT);
	pinMode(FENA,  OUTPUT);

	pinMode(FRin3, OUTPUT);
	pinMode(FRin4, OUTPUT);
	pinMode(FENB,  OUTPUT);

	pinMode(BRin1, OUTPUT);
	pinMode(BRin2, OUTPUT);
	pinMode(BENA,  OUTPUT);

	pinMode(BLin3, OUTPUT);
	pinMode(BLin4, OUTPUT);
	pinMode(BENB,  OUTPUT);
	speed = 100; //初始化速度
}

void forward(){
    digitalWrite(FLin1, LOW);
    digitalWrite(FLin2, HIGH);
    digitalWrite(FRin3, HIGH);
    digitalWrite(FRin4, LOW);

	digitalWrite(BRin1, HIGH);
    digitalWrite(BRin2, LOW);
    digitalWrite(BLin3, LOW);
    digitalWrite(BLin4, HIGH);
	delay(100);
    // analogWrite(FENA, speed);
	// analogWrite(FENB, speed);
	// analogWrite(BENA, speed);
	// analogWrite(BENB, speed);

}

void backward(){
    digitalWrite(FLin1, HIGH);
    digitalWrite(FLin2, LOW);
    digitalWrite(FRin3, LOW);
    digitalWrite(FRin4, HIGH);

	digitalWrite(BRin1, LOW);
    digitalWrite(BRin2, HIGH);
    digitalWrite(BLin3, HIGH);
    digitalWrite(BLin4, LOW);
	delay(100);
    // analogWrite(FENA, speed);
	// analogWrite(FENB, speed);
	// analogWrite(BENA, speed);
	// analogWrite(BENB, speed);

}
//FLin1,FLin2 (0,1)->F
//FRin3,FRin4 (1,0)->F
//BLin1,BLin2 (1,0)->F
//BRin3,BRin4 (0,1)->F
void turnLeft(){
    digitalWrite(FLin1, HIGH);
    digitalWrite(FLin2, LOW);
    digitalWrite(FRin3, HIGH);
    digitalWrite(FRin4, LOW);

    digitalWrite(BLin3, HIGH);
    digitalWrite(BLin4, LOW);
    digitalWrite(BRin1, HIGH);
    digitalWrite(BRin2, LOW);
}

void turnRight(){
    digitalWrite(FLin1, LOW);
    digitalWrite(FLin2, HIGH);
    digitalWrite(FRin3, LOW);
    digitalWrite(FRin4, HIGH);

    digitalWrite(BLin3,  LOW);
    digitalWrite(BLin4, HIGH);
    digitalWrite(BRin1,  LOW);
    digitalWrite(BRin2, HIGH);
}

void stop(){
    digitalWrite(FLin1, LOW);
    digitalWrite(FLin2, LOW);
    digitalWrite(FRin3, LOW);
    digitalWrite(FRin4, LOW);

    digitalWrite(BLin3, LOW);
    digitalWrite(BLin4, LOW);
    digitalWrite(BRin1, LOW);
    digitalWrite(BRin2, LOW);
}

void loop() {
	delay(100);
	analogWrite(FENA, speed);
	analogWrite(FENB, speed);
	analogWrite(BENA, speed);
	analogWrite(BENB, speed);
 	char strc[MAX_BTCMDLEN];
 	int SIZE;
	SIZE = BTSerial.available();
	if (SIZE > 0){ //讀取藍牙訊息
		Serial.print("input size = ");
		Serial.println(SIZE);
		for (int i = 0; i < SIZE; i++){
			cmd[i] = char(BTSerial.read());
		}

		sprintf(strc,"%s",cmd);
	}
	switch (strc[0]) {
		case 'u':
			forward();
			break;
		case 'd':
			backward();
			break;
		case 'l':
			turnLeft();//forward();
			break;
		case 'r':
			turnRight();//backward();
			break;
		case's':
			stop();
			break;
	}
    delay(5);
	speed = speed > 250 ? 0 : speed;
	speed = speed < 0 ? 0 : speed;
	len = 0;
}
