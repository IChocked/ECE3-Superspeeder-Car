#include "Control.h"

Control::Control(double kp_l, double ki_l, double kd_l, double kp_r, double ki_r, double kd_r) {
	this->kp_left = kp_l;
	this->ki_left = ki_l;
	this->kd_left = kd_l;
	this->kp_right = kp_r;
	this->ki_right = ki_r;
	this->kd_right = kd_r;
}



Control::Output Control::Update() {
	// Fuse sensor output
	double weightedSum = fuseSensors();

	// Run PID on the out
	double percent_left = PID(weightedSum, kp_left, ki_left, kd_left);
	double percent_right = PID(weightedSum, kp_right, ki_right, kd_right);

	// conditions
		// if the error is less than the threshold
			// output PID-scaled max for both motors (to account for differences in L vs R)
		// if the error is on the right side
			// output PID weighted for right motor
			// output max - PID weighted for left motor
		// if the error is on the left side
			// output PID weighted for left motor
			// output max - PID weighted for right motor

	if (abs(weightedSum) < theshold) {
		return Output{MAX_SPEED, MAX_SPEED}; // TODO this needs to be scaled based on PID
	}
	else if (weightedSum > 0) { // veered too far right
		return Output{(1.0-percent_left) * MAX_SPEED, MAX_SPEED};
	}
	else if (weightedSum <= 0) { // veered too far left
		return Output{MAX_SPEED, (1.0-percent_right) * MAX_SPEED};
	}

	return Output{-1, -1};
}

double Control::fuseSensors() {
	ECE3_read_IR(sensorValues);

//   for (unsigned char i = 0; i < 8; i++) {
//  
//      Serial.print(sensorValues[i]);
//      Serial.print("\t");
//    }
//   Serial.println();

	sensorValues[0]=abs((sensorValues[0]-630.4)*(1000/1397.8));
	sensorValues[1]=abs((sensorValues[1]-540.0)*(1000/1229.6));
	sensorValues[2]=abs((sensorValues[2]-630.4)*(1000/1513.0));
	sensorValues[3]=abs((sensorValues[3]-630.8)*(1000/1082.8));
	sensorValues[4]=abs((sensorValues[4]-562.2)*(1000/904.2));
	sensorValues[5]=abs((sensorValues[5]-675.4)*(1000/1094.2));
	sensorValues[6]=abs((sensorValues[6]-653.6)*(1000/1394.8));
	sensorValues[7]=abs((sensorValues[7]-721.8)*(1000/581.0));

	double weightedSum = 0;
  weightedSum -= sensorValues[0]*8;
  weightedSum -= sensorValues[1]*4;
  weightedSum -= sensorValues[2]*2;
  weightedSum -= sensorValues[3]*1;//adds negative weight to the first 4 sensors -8 ,-4, -2, and -1 from right to center, then adds it to the sum
  weightedSum += sensorValues[4]*1;
  weightedSum += sensorValues[5]*2;
  weightedSum += sensorValues[6]*4;
  weightedSum += sensorValues[7]*8;//adds positive weight to the final four sensors 1, 2, 4, and 8 from center to left, then adds it to the sum
 
	for (unsigned char i = 0; i < 8; i++) {
		Serial.print(sensorValues[i]);
		Serial.print("\t");
	}
 Serial.print(weightedSum);
 Serial.print("\t");
// Serial.println();

	return weightedSum;
}

// TODO if P controller sucks, add D, if D sucks, add I
double Control::PID(double fusionValue, double kp, double ki, double kd) {
	// return abs(fusionValue * kp +  * ki +  * kd) / max_sensor_sum;
	double percent = abs(fusionValue * kp) / max_sensor_sum;
	if (percent > 1) {
		return 1;
	} else if (percent < 0) {
		return 0;
	}
	return percent;
}

uint16_t* Control::getSensorValues() {
	return sensorValues;
}
