#include "Galil.h"
#include <iostream>

int main() {
	// actuation speed, acceleration, and deceleration of the motor
	uint32_t speed = 30000;
	uint32_t acceleration = 25000;
	uint32_t deceleration = 25000;
	int position = 0;
	char* address = new char[100];
	char* motion = new char[10];

	// strcpy_s(address, 100, "COM6 --baud 115200 --subscribe ALL");
	
	strcpy_s(address, 100, "192.168.42.100 --subscribe ALL");
	strcpy_s(motion, 10, "angular");
	Galil MotorAngular(address, motion, speed, acceleration, deceleration);

	strcpy_s(address, 100, "192.168.42.200 --subscribe ALL");
	strcpy_s(motion, 10, "linear");
	Galil MotorLinear(address, motion, speed, acceleration, deceleration);

	while (true) {
		std::cout << "Enter new motor position:" << std::endl;
		std::cin >> position;

		// leave while-loop when a nonnumber value is entered
		if (std::cin.fail())
			break;

		MotorAngular.trackPosition(position);
		MotorLinear.trackPosition(position);
		
	}

	// freeing heap allocated memory
	delete[] address;
	delete[] motion;
	address = motion = nullptr;
	return 0;
}