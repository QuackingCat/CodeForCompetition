/*
 * free pins (if used pls delete from here):
 * 0, 1, 2, 6 (PWM), 7, A1-A3.
 *
 * terminology: (so many complicated acronyms)
 * US - Ultra sonic.
 * USF - Ultra sonic sensor on the front.
 * USB - Ultra sonic sensor on the back.
 */


/*	libraries	*/

#include <Wire.h>
#include <string.h>


/*	constants	*/

// pins for the US sensors
const int pinEchoUSF = 3; // PWM
const int pinTrigUSF = 2;
const int pinEchoUSB = 5; // PWM
const int pinTrigUSB = 4;

// pins for the motors' controllers
// prime axis
const int pinPrimeAxis1 = 12; // goes to in1
const int pinPrimeAxis2 = 13; // goes to in2
// secondary axises
const int pinSecAxis1 = 8; // goes to in1
const int pinSecAxis2 = 9; // goes to in2
const int pinSecAxis3 = 10; // goes to in3
const int pinSecAxis4 = 11; // goes to in4

// pins for the reflected light sensor
const int pinRefLight = A0;

// I2C registers' addresses
const int addressMaster = b1; // The address of the master's register


/*	variables	*/
string lstTrans = ""; // holds the last transmission from the master.


/*	main code	*/

void setup() {
	// US sensor
	pinMode(pinEchoUSF, INPUT);
	pinMode(pinTrigUSF, OUTPUT);
	pinMode(pinEchoUSB, INPUT);
	pinMode(pinEchoUSB, OUTPUT);
	
	// Motors' controllers
	pinMode(pinPrimeAxis1,OUTPUT);
	pinMode(pinPrimeAxis2,OUTPUT);
	pinMode(pinSecAxis1,OUTPUT);
	pinMode(pinSecAxis2,OUTPUT);
	pinMode(pinSecAxis3,OUTPUT);
	pinMode(pinSecAxis4,OUTPUT);
	
	// Reflected light sensor
	pinMode(pinRedLight, INPUT);
	
	// I2C shit
	Wire.begin(b1000101); // turn on the I2C and set the address of this device's register.
	Wire.onReceive(receiveEvent); // when asked to deliver info.
	Wire.onRequest(requestEvent); // when asked to do something.
}


void loop() {
	// keep empty
}


// when the master is sending data this function will start.
void recieveEvent(int input) {
	lstTrans = "";
	while (Wire.available()) // adding the transmission to "lstTrans" character by character.
		lastTrans += (char)Wire.read();
	
	// handling the received data
	if (strcmp(lstTrans, "forward"))
		moveForward();
	else if (strcmp(lstTrans, "backward"))
		moveBackward();
	else if (strcmp(lstTrans, "right"))
		moveveRight();
	else if (strcmp(lstTrans, "left"))
		moveLeft();
	else if (strcmp(lstTrans, "stop"))
}


// when the msater asks for data this function will start.
void requestEvent() {
	if (strcmp(lstTrans, "isWhiteArea"))
		send(checkWhiteArea());
	else if (strcmp(lstTrans, "disFront"))
		send(getFrontDis());
	else if (strcmp(lstTrans, "disBack"))
		send(getBackDis());
}


// move forward
void moveForward() {
	digitalWrite(pinPrimeAxis1, LOW);
	digitalWrite(pinPrimeAxis2, LOW);
	digitalWrite(pinSecAxis1, HIGH);
	digitalWrite(pinSecAxis2, LOW);
	digitalWrite(pinSecAxis3, HIGH);
	digitalWrite(pinSecAxis4, Low);
}


// move backward
void moveBackward() {
	digitalWrite(pinPrimeAxis1, LOW);
	digitalWrite(pinPrimeAxis2, LOW);
	digitalWrite(pinSecAxis1, LOW);
	digitalWrite(pinSecAxis2, HIGH);
	digitalWrite(pinSecAxis3, LOW);
	digitalWrite(pinSecAxis4, HIGH);
}


// move right
void moveRight() {
	digitalWrite(pinPrimeAxis1, HIGH);
	digitalWrite(pinPrimeAxis2, LOW);
	digitalWrite(pinSecAxis1, LOW);
	digitalWrite(pinSecAxis2, LOW);
	digitalWrite(pinSecAxis3, LOW);
	digitalWrite(pinSecAxis4, Low);
}


// move left
void moveLeft() {
	digitalWrite(pinPrimeAxis1, LOW);
	digitalWrite(pinPrimeAxis2, HIGH);
	digitalWrite(pinSecAxis1, LOW);
	digitalWrite(pinSecAxis2, LOW);
	digitalWrite(pinSecAxis3, LOW);
	digitalWrite(pinSecAxis4, Low);
}


// stop the robot
void stopMoving() {
	digitalWrite(pinPrimeAxis1, LOW);
	digitalWrite(pinPrimeAxis2, LOW);
	digitalWrite(pinSecAxis1, LOW);
	digitalWrite(pinSecAxis2, LOW);
	digitalWrite(pinSecAxis3, LOW);
	digitalWrite(pinSecAxis4, Low);
}


// returns the front distance.
int getFrontDis() {
	return getDis(pinTrigUSF, pinEchoUSF);
}

// returns the front distnance.
int getBackDis() {
	return getDis(pinTrigUSB, pinEchoUSB);
}

// return the distance of a specific distance sensor
int getDis(int trig, int echo) {
	digitalWrite(trig, LOW);
	delayMicroseconds(2);
	digitalWrite(trig, HIGH);
	delayNicroseconds(10);
	digitalWrite(trig, LOW);
	
	int duration = pulseIn(echo, HIGH);
	return duration * 0.034 / 2; // return the distance
}


// return true if there is white area under the robot.
boolean checkWhiteArea() {
	int loewerBoundWhite = 100;
	return analogRead(pinRedLight) < lowerBoundWhite;
}
