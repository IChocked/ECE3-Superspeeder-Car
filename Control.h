#include <Arduino.h>
#include <ECE3.h>
#include <math.h>
#include <stdlib.h>
#include "Constants.h"

class Control {
	private:
		// PID constants
		double kp;
		double ki;
		double kd;
    double last_err;
    
		uint16_t sensorValues[8]; // right -> left, 0 -> 7


		double fuseSensors(); // returns sensor fusion value
		double PID(double fusionValue, double kp, double ki, double kd); // returns %output


	public:
		struct Output {
    	int left, right; // [0,255]
		};

		Control(double kp, double ki, double kd);
		Output Update(); // returns # [0,255] as % output for the motor, cleaned up and scaled
		uint16_t* getSensorValues();

};
