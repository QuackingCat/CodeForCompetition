/*
	free pins (if used pls delete from here):
	0, 1, 9-11 (PWM), 13.

	terminology: (so much complicated acronyms)
		US - Ultra sonic.
		USR - Ultra sonic sensor on the right.
		USL - Ultra sonic sensor on the left.
		SS - sound sensor.
		slave - the second arduino.
		FDS - fire detection system.
		FES - fire extinguishing system.
*/


/*	libraries	*/

#include <Wire.h>


/*	constants	*/

// pins for the US sensors
const int pinTrigUSR = 2;
const int pinEchoUSR = 3; // PWM
const int pinTrigUSL = 4;
const int pinEchoUSL = 5; // PWM

// pins for the sound sensor
const int pinSS = A0;

// pins for the turret (servo)
const int pinTurret = 6; // PWM

// pins for the fire detection system
const int pinFDS1 = A1;
const int pinFDS2 = A2;
const int pinFDS3 = A3;

// pins for the fire extinguishing system
// see "ConfigurationsEFS.png" to know how to control it.
const int pinFES1 = 7; // goes to INA.
const int pinFES2 = 8; // goes to INB.

// I2C registers' addresses.
const int addressGyro = 0x68; // The address of the gyro's register (I2C shit).
const int addressSlave = 0x45; // The address of the slave's register (lol 69).


/*	main code	*/

void setup() {
	// US sensors
	pinMode(pinEchoUSR, INPUT);
	pinMode(pinTrigUSR, OUTPUT);
	pinMode(pinEchoUSL, INPUT);
	pinMode(pinEchoUSL, OUTPUT);
	
	// Sound sensor
	pinMode(pinSS, INPUT);
	
	// turret
	pinMode(pinTurret, OUTPUT);
	
	// FDS
	pinMode(pinFDS1, INPUT);
	pinMode(pinFDS2, INPUT);
	pinMode(pinFDS3, INPUT);
	
	// FES
	pinMode(pinFES1, INPUT);
	pinMode(pinFES2, INPUT);
	
	// I2C shit
	Wire.begin(b1); // turn on the I2C and set the address of this device's register.
	
	Wire.beginTransmission(addressGyro); // start the transmission to the gyro.
	Wire.write(0x6B); // PWR_MGMT_1 register.
	Wire.write(0); // wakes up the sensor.
	Wire.endTransmission(true); // releasing the I2C bus.
}


void loop() {
	while (!startConditions()) { // waits for the required conditions to be true so we can start the run.
		continue; 
	}
	
	
}


// Return true if the robot is above white area and a sound of 3600Hz is being played for atleast 2 seconds.
boolean startConditions()
{
	for (int i = 0; i < 4; i++){
		delay(500);
		if (!(checkWhiteArea() && checkSound())) {
			return false;
		}
	}
	
	return true;
}


// returns the robot's rotation degree with respect to the initial angle.
int getDegrees(){
	Wire.beginTransmission(addressGyro); // start the transmission to the gyro.
	Wire.write(0x47); // start reading from this register.
	Wire.endTransmission(false); // commit without releasing the I2C bus.
	
	Wire.requestFrom(addressGyro, 2, true); // asking for two registers starting at "0x47".
	return (Wire.read() << 8) | Wire.read(); // reading registers "0x47" (GYRO_ZOUT_H) and "0x48" (GYRO_ZOUT_L). 
}


boolean checkSound()
{
	int sensorValue = analogRead(3);
	while (sensorValue < 3600)
	{
		sensorValue = analogRead(3);
	}
}


// checks if there is white area under the robot.
boolean checkWhiteArea() {
	return askSlave("isWhiteArea");
}


// move forward
void moveForward() {
	sendToSlave("forward");
}

// move backward
void moveBackward() {
	sendToSlave("backward");
}

// move right
void moveRight() {
	sendToSlave("right");
}

// move left
void moveLeft() {
	sendToSlave("left")
}

// stop moving
void stopMoving() {
	sendToSlave("stop");
}


// returns the front distnance.
int getFrontDis() {
	return askSlave("disFront");
}

// returns the back distnance.
int getBackDis() {
	return askSlave("disBack");
}

// returns the right distnance.
int getRightDis() {
	return getDis(pinTrigUSR, pinEchoUSR);
}

// returns the left distnance.
int getLeftDis() {
	return getDis(pinTrigUSL, pinEchoUSL);
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


// sends string to the slave.
void sendToSlave(string str) {
	Wire.beginTransmission(addressSlave); // start the transmission to the gyro.
	Wire.write(str); // start reading from this register.
	Wire.endTransmission(true); // commit and releasing the I2C bus.
}


// ask for info fron the slave based on string.
int askSlave(string str) {
	sendToSlave(str);
	Wire.requestFrom(addressSlave, 1, true); // asking for one register.
	return Wire.read();
}
