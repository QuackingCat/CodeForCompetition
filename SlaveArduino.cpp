/*	libraries	*/

#include <Wire.h>


/*	constants	*/

// I2C registers' addresses
const int addressMaster = b1; // The address of the master's register.



/*	main code	*/

void setup() {
	// I2C shit
	wire.begin(b1000101); // turn on the I2C and set the address of this device's register.
}
