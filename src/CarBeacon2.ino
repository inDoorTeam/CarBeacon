#include <SoftwareSerial.h>
#include <Wire.h>
#include <Arduino.h>
#define MAX_BTCMDLEN 128
SoftwareSerial BTSerial(A14,A15); // Arduino TX/RX
byte cmd[MAX_BTCMDLEN];
int len = 0;

const int  trig  = A1; // 超音波避障 
const int  echo  = A2; // 超音波避障
const byte FENB  =  2;
const byte FRin4 =  3; //FR=右前輪
const byte FRin3 =  4;
const byte FLin2 =  5; //FL=左前輪
const byte FLin1 =  6;
const byte FENA  =  7;

const byte BENB  =  8;
const byte BLin4 =  9; //BL=左後輪
const byte BLin3 = 10;
const byte BRin2 = 11;//BR=右後輪
const byte BRin1 = 12;
const byte BENA  = 13;
byte speed;
int stopf = 0;
void setup() {
	Serial.begin(9600);
    BTSerial.begin(9600);
	pinMode(FLin1, OUTPUT);
	pinMode(FLin2, OUTPUT);
	pinMode(FENA,  OUTPUT);
	//pinMode(BLUE, OUTPUT);
	//pinMode(RED, OUTPUT);
	pinMode(FRin3, OUTPUT);
	pinMode(FRin4, OUTPUT);
	pinMode(FENB,  OUTPUT);

	pinMode(BRin1, OUTPUT);
	pinMode(BRin2, OUTPUT);
	pinMode(BENA,  OUTPUT);

	pinMode(BLin3, OUTPUT);
	pinMode(BLin4, OUTPUT);
	pinMode(BENB,  OUTPUT);
	speed = 60; //初始化速度

    pinMode (trig, OUTPUT);
    pinMode (echo, INPUT);
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
	// digitalWrite(BLUE,HIGH);
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
	delay(70);
	// digitalWrite(BLUE,HIGH);
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
void checkDistance(){
	float duration, distance;
	digitalWrite(trig, HIGH);
	delayMicroseconds(1000);
	digitalWrite(trig, LOW);
	duration = pulseIn (echo, HIGH);
	distance = (duration/2)/29;
	if(distance <= 30){
		stop();
		stopf = 1;
	}
	else
		stopf = 0;
}
void loop() {
	delay(100);
	analogWrite(FENA, speed);
	analogWrite(FENB, speed);
	analogWrite(BENA, speed);
	analogWrite(BENB, speed);
	speed = 60;
	checkDistance();
	char strc[MAX_BTCMDLEN];
 	int SIZE = BTSerial.available();
	if (SIZE > 0){ //讀取藍牙訊息
		Serial.print("input size = ");
		Serial.println(SIZE);
		for (int i = 0; i < SIZE; i++){
			cmd[i] = char(BTSerial.read());
		}
		sprintf(strc,"%s",cmd);
	}
	Serial.println(strc);
	switch (strc[0]) {
		case 'u':
			if(!stopf)
				forward();
			break;
		case 'd':
			backward();
			break;
		case 'l':
			speed = 120;
			turnLeft();//forward();
			break;
		case 'r':
			speed = 120;
			turnRight();//backward();
			break;
		case 's':
			stop();
			break;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			int dtime = atoi(strc);
			Serial.println(dtime);
			for(int i = 0; i < dtime/4; i++){
				checkDistance();
				if(!stopf){
					analogWrite(FENA, speed);
					analogWrite(FENB, speed);
					analogWrite(BENA, speed);
					analogWrite(BENB, speed);
					forward();
				}
				else stop();
			}
			stop();
			memset(strc,'\0',sizeof(strc));
			memset(cmd,'\0',sizeof(cmd));
	}
    delay(5);
	len = 0;
}
