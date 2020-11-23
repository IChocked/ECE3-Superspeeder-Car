#include "Control.h"

Control::Control(double kp_l, double ki_l, double kd_l, double kp_r, double ki_r, double kd_r) {
	this->kp_left = kp_l;
	this->ki_left = ki_l;
	this->kd_left = kd_l;
	this->kp_right = kp_r;
	this->ki_right = ki_r;
	this->kd_right = kd_r;

	ECE3_Init();
//	Serial.begin(9600);
//	delay(200);
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
		return Output{255, 255}; // TODO this needs to be scaled based on PID
	}
	else if (weightedSum < 0) { // veered too far right
		return Output{(1.0-percent_left) * 255, percent_right * 255};
	}
	else if (weightedSum >= 0) {
		return Output{percent_left * 255, (1.0-percent_right) * 255};
	}

	return Output{-1, -1};
}

double Control::fuseSensors() {
	ECE3_read_IR(sensorValues);

	sensorValues[0]=abs((sensorValues[0]-630.4)*(1000/1397.8)) * -8;
	sensorValues[1]=abs((sensorValues[1]-540)*(1000/1229.6)) * -4;
	sensorValues[2]=abs((sensorValues[2]-630.4)*(1000/1513.0)) * -2;
	sensorValues[3]=abs((sensorValues[3]-630.8)*(1000/1082.8)) * -1;
	sensorValues[4]=abs((sensorValues[4]-562.2)*(1000/904.2)) * 1;
	sensorValues[5]=abs((sensorValues[5]-675.4)*(1000/1094.2)) * 2;
	sensorValues[6]=abs((sensorValues[6]-653.6)*(1000/1394.8)) * 4;
	sensorValues[7]=abs((sensorValues[7]-721.8)*(1000/581.0)) * 8;

	double weightedSum = 0;
	for (unsigned char i = 0; i < 8; i++) {
		weightedSum += sensorValues[i];

		Serial.print(sensorValues[i]);
		Serial.print("\t");
	}
 Serial.println();

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
