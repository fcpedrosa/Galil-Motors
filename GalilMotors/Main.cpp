#include "Galil.h"
#include <iostream>

int main() {
	// actuation speed, acceleration, and deceleration of the motor
	uint32_t speed = 30000;
	uint32_t acceleration = 20000;
	uint32_t deceleration = 5000;
	int position = 0;
	char* address = new char[100];
	strcpy_s(address, 100, "COM6 --baud 115200 --subscribe ALL");
	char* motion = new char[100];
	strcpy_s(motion, 100, "linear");

	Galil Motor_01(address, motion, speed, acceleration, deceleration);

	while (true) {
		std::cout << "Enter new motor position:" << std::endl;
		std::cin >> position;

		// leave while-loop when a nonnumber value is entered
		if (std::cin.fail())
			break;

		Motor_01.trackPosition(position);
		
	}

	return 0;
}