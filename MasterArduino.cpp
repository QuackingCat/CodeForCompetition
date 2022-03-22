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
#include <arduinoFFT.h>


void room1();
void room2();
void room3();
void room4();
void checkFire();
int getDegrees();
void moveForward();
void moveBackward();
void moveRight();
void moveLeft();
void stopMoving();
int getFrontDis();
int getBackDis();
int getRightDis();
int getLeftDis();
int askSlave(String);
void sendToSlave(String);


/*	constants	*/

// pins for the US sensors
#define pinTrigUSR 2
#define pinEchoUSR 3 // PWM
#define pinTrigUSL 4
#define pinEchoUSL 5 // PWM

// pins for the sound sensor
#define pinSS A0

// pins for the turret (servo)
#define pinTurret 6 // PWM

// pins for the fire detection system
#define pinFDS1 A1
#define pinFDS2 A2
#define pinFDS3 A3

// pins for the fire extinguishing system
// see "ConfigurationsEFS.png" to know how to control it.
#define pinFES1 7 // goes to INA.
#define pinFES2 8 // goes to INB.

// I2C registers' addresses
#define addressGyro 0x68 // The address of the gyro's register (I2C shit).
#define addressSlave 0x45 // The address of the slave's register (lol 69).

// arduinoFFT setup
#define SAMPLES 128 // number of checks.
#define SAMPLING_FREQUENCY 7200 // twice the highest frequency expected.
#define WANTED_FREQUENCY 3600



/*	variables	*/

// some shit for the sound sensor.
arduinoFFT FFT = arduinoFFT();
unsigned int samplingPeriod;



/*	main code	*/

void setup() {
	// US sensors
	pinMode(pinEchoUSR, INPUT);
	pinMode(pinTrigUSR, OUTPUT);
	pinMode(pinEchoUSL, INPUT);
	pinMode(pinEchoUSL, OUTPUT);
	
	// Sound sensor
	pinMode(pinSS, INPUT);
	samplingPeriod = round(1000000<(1.0/SAMPLING_FREQUENCY));
	
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
	Wire.begin(1); // turn on the I2C and set the address of this device's register.
	
	Wire.beginTransmission(addressGyro); // start the transmission to the gyro.
	Wire.write(0x6B); // PWR_MGMT_1 register.
	Wire.write(0); // wakes up the sensor.
	Wire.endTransmission(true); // releasing the I2C bus.
}


void loop() {
	while (!startConditions()); // waits for the required conditions to be true so we can start the run.
	
	room1();
	room2();
	room3();
  room4();
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


//flag if dog is in pos A
boolean dogA = false;

//flag if dog is in pos C
boolean dogC = false;

//runs all the way to the 1st room and till the main intersection
void room1()
{
	if (getLeftDis() > 60) //finds if the dog is in A
	{
		dogA = true;
	}
	moveForward();
	while (getBackDis() > 110); //till the main intersection
	stopMoving(); //to stop in the intersection
	
	if (getLeftDis() > 50) //finds if dog is in C
	{
		dogC = true;
	}
	
	moveRight();
	while (getRightDis() > 15); //till the wall
	stopMoving(); //stop infront of the room
	
	moveBackward();
	while (getFrontDis() < 70); // move inside the room
	stopMoving();
	
	checkFire(); //needs to be done will check for the the fire
	
	moveForward();
	while (getFrontDis() > 15); //gets out of the room
	stopMoving(); //stops in front of the room
	
	moveLeft();
	while (getRightDis() < 90) //gets back to the intersection
	stopMoving(); //stops in the intersection
}

//runs from the main intersection to the 2nd room and gets infront of the next
void room2()
{
	moveForward();
	while (getFrontDis() > 20);
	stopMoving();
	
	moveRight();
	delay (1200);
	stopMoving();
	
	checkFire();
	
	moveLeft();
	delay (1200);
	stopMoving();
}

//runs all the way to the 3rd room and to the intersection where the dog is
void room3()
{
	if (getLeftDis() <20) //checks where the door is (is it on top or bottom)
	{
		moveBackward();
		while (getFrontDis() < 70);
		stopMoving();
	}
	
	moveLeft(); //enters the room
	delay(1200);
	stopMoving();
	
	checkFire();
	
	moveLeft(); //leavs to the exsit
	while(getLeftDis() > 10);
	stopMoving();
	
	moveBackward(); //goes to the exsit of the room and to the intersection with the dog
	while(getFrontDis() < 110);
	stopMoving();
}

//runs from the dog intersection all the way home
void room4()
{
	if (dogC) //if the dog is in pos C
	{
		moveBackward(); //gets to the interection next to the room and across from the home
		while (getBackDis() > 10);
		stopMoving();
		
		moveRight(); //gets inffront of the final room
		while (getLeftDis() < 60);
		stopMoving();
		
		moveForward(); //enters the room
		delay(1200);
		stopMoving();
		
		checkFire();
		
		moveBackward(); //esits the room
		while (getBackDis() > 10);
		stopMoving();
		
		moveRight(); //arives home
		while (getLeftDis() < 140);
		stopMoving();
		//turn LED to high
	}
	else if(dogA) //if dog is in A
	{
		moveRight(); //gets in front of the room from under
		while (getLeftDis() < 90);
		stopMoving();
		
		if (getBackDis() > 20) //checks if you can go in from under
		{
			moveBackward(); //enters the room
			delay(1200);
			stopMoving();
			
			checkFire();
			
			moveForward(); //leavs the room
			while (getFrontDis() > 10);
			stopMoving();
		}
		moveRight(); //gets to the main interesection
		while (getLeftDis() < 138);
		stopMoving();
		
		moveBackward(); //gets to the end
		while (getBackDis() > 20);
		stopMoving();
		//turn led to high
	}
	else //dog is in pos B
	{
		moveRight(); //gets in front of the room from under
		while (getLeftDis() < 90);
		stopMoving();
		
		if (getBackDis() > 20) //checks if you can go in from under and gets home
		{
			moveBackward(); //enters the room
			delay(1200);
			stopMoving();
			
			checkFire();
			
			moveForward(); //leavs the room
			while (getFrontDis() > 10);
			stopMoving();
			
			moveRight(); //gets to the main interesection
			while (getLeftDis() < 138);
			stopMoving();

			moveBackward(); //gets to home
			while (getBackDis() > 20);
			stopMoving();
			//turn led to high
		}
		else
		{
			moveRight(); //gets to the main interesection
			while (getLeftDis() < 138);
			stopMoving();

			moveBackward(); //gets to home
			while (getBackDis() > 20);
			stopMoving();
			
			moveLeft(); //gets in front of the room from the top side
			while (getLeftDis() > 70);
			stopMoving();
			
			moveForward(); //enters the room
			delay(1200);
			stopMoving();
			
			checkFire();
			
			moveBackward(); //leavs the room
			while (getBackDis() > 10);
			stopMoving();
			
			moveRight(); //arives home
			while (getLeftDis() < 140);
			stopMoving();
			//turn LED to high
		}
		
	}
	
	
		
}

// returns the robot's rotation degree with respect to the initial angle.
int getDegrees() {
	Wire.beginTransmission(addressGyro); // start the transmission to the gyro.
	Wire.write(0x47); // start reading from this register.
	Wire.endTransmission(false); // commit without releasing the I2C bus.
	
	Wire.requestFrom(addressGyro, 2, true); // asking for two registers starting at "0x47".
	return (Wire.read() << 8) | Wire.read(); // reading registers "0x47" (GYRO_ZOUT_H) and "0x48" (GYRO_ZOUT_L). 
}


// return true if sound of 3600Hz is being played.
boolean checkSound() {
	unsigned long microSeconds;
	double vReal[SAMPLES];
	double vImag[SAMPLES] = {0}; // creates and sets all the array to zeros
	
	for (int i = 0; i < SAMPLES; i++) {
		microSeconds = micros();
		vReal[i] = analogRead(pinSS); // puts the current value from the sensor in the array
		
		while (micros() < (microSeconds + samplingPeriod)) { // waits some time.
			continue;
		}
	}
	
	// perform fast fourier transform on the samples.
	FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
	FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
	FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
	
	int frequency = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY); // the frequency.
	
	return ( (WANTED_FREQUENCY - 40) < frequency ) && ( frequency < (WANTED_FREQUENCY + 40) );
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
	sendToSlave("left");
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
	delayMicroseconds(10);
	digitalWrite(trig, LOW);
	
	int duration = pulseIn(echo, HIGH);
	return duration * 0.034 / 2; // return the distance
}


// sends string to the slave.
void sendToSlave(char* str) {
	Wire.beginTransmission(addressSlave); // start the transmission to the gyro.
	Wire.write(str); // start reading from this register.
	Wire.endTransmission(true); // commit and releasing the I2C bus.
}


// ask for info fron the slave based on string.
int askSlave(char* str) {
	sendToSlave(str);
	Wire.requestFrom(addressSlave, 1, true); // asking for one register.
	return Wire.read();
}
