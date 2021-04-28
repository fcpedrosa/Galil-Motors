#include "Galil.h"
#include <iostream>

Galil::Galil(char* add, char* type, uint32_t sp, uint32_t acc, uint32_t decc) :
	address(add), motionType(type), speed(sp), acceleration(acc), deceleration(decc) {
	// initialize the connection variable to currently disconnected
	this->connection = 0;
	// empty the traffic buffer; insert null terminator at the end
	memset(this->trafficBUFFER, ' ', G_SMALL_BUFFER - 2);
	this->trafficBUFFER[G_SMALL_BUFFER - 1] = '\0';
	// tries to establish a connection with the given address
	this->checkError(GOpen(this->address, &this->connection));
	// configures motor in case the connection has been established
	this->setMotorConfigurations();
};

// class destructor
Galil::~Galil() {
	// nothing to be done
}

// set motor configurations
void Galil::setMotorConfigurations() {
	// motor physical properties
	this->checkError(GCmd(this->connection, "BA A"));				// brushless A axis: sets all axes that require sinusoidal communication
	
	int type = strcmp(this->motionType, "linear");
	if ( type == 0 )
		this->checkError(GCmd(this->connection, "BM 2000"));			// Defines the length of the magnetic cycle in encoder counts
	else
		this->checkError(GCmd(this->connection, "BM 1600"));			// Defines the length of the magnetic cycle in encoder counts

	this->checkError(GCmd(this->connection, "BZ<1000>1500"));		// brushless zero
	this->checkError(GCmd(this->connection, "BZ 3"));				// set axes for sinusoidal communication
	// this->checkError(GCmd(this->connection, "SH A"));				// use the current motor position as the command position and enable position control here	

	// setting speed, acceleration, and decelerattion for position tracking
	this->checkError(GCmd(this->connection, "STA"));				// stop motor
	this->checkError(GMotionComplete(this->connection, "A"));		// wait for motion to complete
	this->checkError(GCmd(this->connection, "SHA"));				// set servo here
	this->checkError(GCmd(this->connection, "DPA=0"));				// start position at absolute zero
	this->checkError(GCmd(this->connection, "PTA=1"));				// start tracking mode on A axis

	// configuring motion speed
	sprintf_s(this->trafficBUFFER, G_SMALL_BUFFER, "SPA=%d", this->speed);
	this->checkError(GCmd(this->connection, this->trafficBUFFER));
	// configuring motion acceleration
	sprintf_s(this->trafficBUFFER, G_SMALL_BUFFER, "ACA=%d", this->speed);
	this->checkError(GCmd(this->connection, this->trafficBUFFER));
	// configuring motion deceleration
	sprintf_s(this->trafficBUFFER, G_SMALL_BUFFER, "DCA=%d", this->speed);
	this->checkError(GCmd(this->connection, this->trafficBUFFER));

	std::cerr << "The motor address <" << this->address << " configurations have been updated! \n" << std::endl;
}

// setter method for adjusting the speed
void Galil::setSpeed(uint32_t sp) {
	this->speed = sp;
	this->setMotorConfigurations();
}

// setter method for adjusting the acceleration
void Galil::setAccelerations(uint32_t acc, uint32_t decc) {
	this->acceleration = acc;
	this->deceleration = decc;
	this->setMotorConfigurations();
}

// method for tracking an absolute position
GReturn Galil::trackPosition(int position) {
	// first, check whether a connection has been established
	if (this->connection == 0) { // a valid connection is always nonzero
		std::cerr << "There was an error with the connection." << std::endl;
		return G_CONNECTION_NOT_ESTABLISHED;
	}

	// in case a valid connection has been found:
	sprintf_s(this->trafficBUFFER, G_SMALL_BUFFER, "PAA=%d", position);
	return this->checkError(GCmd(this->connection, this->trafficBUFFER));
}

// error checking method
GReturn Galil::checkError(GReturn rc) {
	// prints out the error message on the console
	if (rc != G_NO_ERROR) {
		// get error information
		GError(rc, trafficBUFFER, G_SMALL_BUFFER);
		std::cerr << trafficBUFFER << std::endl;
	}

	return rc;
}

// method that tries to reconnect with the motor
void Galil::reconnectMotor() {
	// check if the connection has indeed been lost
	if (connection != 0) {
		std::cerr << "A connection has already been established!" << std::endl;
		return;
	}
	else {
		// tries to establish a connection with the given address
		this->checkError(GOpen(this->address, &this->connection));
		// configures motor in case the connection has been established
		this->setMotorConfigurations();
	}
}