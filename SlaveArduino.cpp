/*
 * free pins (if used pls delete from here):
 * 0, 1, 2, 8, 9 (PWM), 10 (PWM), 11 (PWM).
 *
 * terminology: (so many complicated acronyms)
 * US - Ultra sonic.
 * USF - Ultra sonic sensor on the front.
 * USB - Ultra sonic sensor on the back.
 */


/*	libraries	*/

#include <Wire.h>


/*	constants	*/

// pins for the US sensors
const int pinEchoUSF = A0;
const int pinTrigUSF = A1;
const int pinEchoUSB = A2;
const int pinTrigUSB = A3;

// pins for the motors' controllers
// prime axis
const int pinPrimeAxis1 = 12; // goes to in1
const int pinPrimeAxis2 = 13; // goes to in2
// secondary axises
const int pinSecAxis1 = 4; // goes to in1
const int pinSecAxis2 = 5; // goes to in2
const int pinSecAxis3 = 6; // goes to in3
const int pinSecAxis4 = 7; // goes to in4

// pins for the reflected light sensor (PWM)
const int pinRefLight = 3;

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
	wire.begin(b1000101); // turn on the I2C and set the address of this device's register.
	Wire.onReceive(receiveEvent); // register event
	
}


void loop() {
	// keep empty
}


// when the master is sending data this function will start
void recieveEvent() {
	lstTrans = "";
	while (0 < Wire.available()) // adding the transmission to "lstTrans" character by character.
		lastTrans += (char)Wire.read();
	
	// handling the received data
	
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
