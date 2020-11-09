#include "Control.h"

Control::Control(double kp, double ki, double kd) {
	this->kp = kp;
	this->ki = ki;
	this->kd = kd;
	init()
}


int Control::Update() {
	// Fuse sensor output
	// Run PID on the out
	// rescale output to [0,255] and other various checks we come up with
	return -1;
}

double Control::fuseSensors() {
	return -1;
}

double Control::PID() {
	return -1;
}
