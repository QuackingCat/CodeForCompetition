/*
	free pins (if used pls delete from here):
	0, 1, 2, 6 (PWM), 7, A1-A3.
	
	terminology: (so many complicated acronyms)
		US - Ultra sonic.
		USF - Ultra sonic sensor on the front.
		USB - Ultra sonic sensor on the back.
*/



/*	libraries	*/

#include <Wire.h>
#include <String.h>


/*	constants	*/

// pins for the US sensors
#define pinTrigUSF 2
#define pinEchoUSF 3 // PWM
#define pinTrigUSB 4
#define pinEchoUSB 5 // PWM

// pins for the motors' controllers
// prime axis
#define pinPrimeAxis1 12 // goes to in1
#define pinPrimeAxis2 13 // goes to in2
// secondary axises
#define pinSecAxis1 8 // goes to in1
#define pinSecAxis2 9 // goes to in2
#define pinSecAxis3 10 // goes to in3
#define pinSecAxis4 11 // goes to in4

// pins for the reflected light sensor
#define pinRefLight A0

// I2C registers' addresses
#define addressMaster 0b1 // The address of the master's register


/*	variables	*/
char *lstTrans = ""; // holds the last transmission from the master.


/*	main code	*/

void setup() {
	// US sensor
	pinMode(pinEchoUSF, INPUT);
	pinMode(pinTrigUSF, OUTPUT);
	pinMode(pinEchoUSB, INPUT);
	pinMode(pinEchoUSB, OUTPUT);
	
	// Motors' controllers
	pinMode(pinPrimeAxis1, OUTPUT);
	pinMode(pinPrimeAxis2, OUTPUT);
	pinMode(pinSecAxis1, OUTPUT);
	pinMode(pinSecAxis2, OUTPUT);
	pinMode(pinSecAxis3, OUTPUT);
	pinMode(pinSecAxis4, OUTPUT);
	
  // Reflected light sensor
  pinMode(pinRefLight, INPUT);

	// I2C shit
	Wire.begin(0b1000101); // turn on the I2C and set the address of this device's register.
	Wire.onReceive(recieveEvent); // when asked to deliver info.
	Wire.onRequest(requestEvent); // when asked to do something.
}


void loop() {
	// keep empty
}


// when the master is sending data this function will start.
void recieveEvent(int input) {
	lstTrans = "";
	while (Wire.available()) // adding the transmission to "lstTrans" character by character.
		lstTrans += (char)Wire.read();
	
	// handling the received data
	if (strcmp(lstTrans, "forward"))
		moveForward();
	else if (strcmp(lstTrans, "backward"))
		moveBackward();
	else if (strcmp(lstTrans, "right"))
		moveRight();
	else if (strcmp(lstTrans, "left"))
		moveLeft();
	else if (strcmp(lstTrans, "stop"))
    stopMoving();
}


// when the msater asks for data this function will start.
void requestEvent() {
	if (strcmp(lstTrans, "isWhiteArea"))
		Wire.write(checkWhiteArea());
	else if (strcmp(lstTrans, "disFront"))
		Wire.write(getFrontDis());
	else if (strcmp(lstTrans, "disBack"))
		Wire.write(getBackDis());
}


// move forward
void moveForward() {
	digitalWrite(pinPrimeAxis1, LOW);
	digitalWrite(pinPrimeAxis2, LOW);
	digitalWrite(pinSecAxis1, HIGH);
	digitalWrite(pinSecAxis2, LOW);
	digitalWrite(pinSecAxis3, HIGH);
	digitalWrite(pinSecAxis4, LOW);
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
	digitalWrite(pinSecAxis4, LOW);
}


// move left
void moveLeft() {
	digitalWrite(pinPrimeAxis1, LOW);
	digitalWrite(pinPrimeAxis2, HIGH);
	digitalWrite(pinSecAxis1, LOW);
	digitalWrite(pinSecAxis2, LOW);
	digitalWrite(pinSecAxis3, LOW);
	digitalWrite(pinSecAxis4, LOW);
}


// stop the robot
void stopMoving() {
	digitalWrite(pinPrimeAxis1, LOW);
	digitalWrite(pinPrimeAxis2, LOW);
	digitalWrite(pinSecAxis1, LOW);
	digitalWrite(pinSecAxis2, LOW);
	digitalWrite(pinSecAxis3, LOW);
	digitalWrite(pinSecAxis4, LOW);
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
	delayMicroseconds(10);
	digitalWrite(trig, LOW);
	
	int duration = pulseIn(echo, HIGH);
	return duration * 0.034 / 2; // return the distance
}


// return true if there is white area under the robot.
boolean checkWhiteArea() {
	int lowerBoundWhite = 100;
	return analogRead(pinRefLight) < lowerBoundWhite;
}
