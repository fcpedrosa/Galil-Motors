#pragma once

#include <cstdint>
#include <memory>
#include "gclib.h"
#include "gclibo.h"

class Galil
{
public:
	// disallawing a default constructor
	Galil() = delete;

	// defining an overloaded constructor
	Galil(char* add, char* type, uint32_t sp, uint32_t acc, uint32_t decc);

	// set motor configurations
	void setMotorConfigurations();

	// setter method for adjusting the speed
	void setSpeed(uint32_t sp);

	// setter method for adjusting the acceleration
	void setAccelerations(uint32_t acc, uint32_t decc);

	// method for tracking an absolute position
	GReturn trackPosition(int position);

	// error checking method
	GReturn checkError(GReturn rc);

	// method that tries to reconnect with the motor
	void reconnectMotor();


private:
	uint32_t speed;
	uint32_t acceleration;
	uint32_t deceleration;
	char* motionType;
	char* address;
	char trafficBUFFER[G_SMALL_BUFFER];
	GCon connection;
};

