#include <Arduino.h>
#include <ECE3.h>
class Control {
	private:
		// PID constants
		double kp = -1;
		double ki = -1;
		double kd = -1;

		double fuseSensors(); // returns sensor fusion value
		double PID(double fusionValue); // returns %output for a motor


	public:
		Control(double kp, double ki, double kd);
		int Update(); // returns # [0,255] as % output for the motor, cleaned up and scaled
		
};
