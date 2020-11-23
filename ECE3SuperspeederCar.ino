#include "Control.h"

// Define the states
const int REST = 0;
const int FOLLOW_PATH = 1;
const int FIND_PATH = 2;
const int DONUT = 3;

// current state of the car
int state = REST;

const int left_nslp_pin=31;
const int right_nslp_pin=11;// nslp ==> awake & ready for PWM
const int left_dir_pin=29;
const int right_dir_pin=30;
const int left_pwm_pin=40;
const int right_pwm_pin=39;

const int LED_RF = 41;

Control control(KP_L, KI_L, KD_L, KP_R, KI_R, KD_R);

void setup() {
	pinMode(left_nslp_pin,OUTPUT);
  pinMode(left_dir_pin,OUTPUT);
  pinMode(left_pwm_pin,OUTPUT);
  pinMode(right_nslp_pin,OUTPUT);
  pinMode(right_dir_pin,OUTPUT);
  pinMode(right_pwm_pin,OUTPUT);
  pinMode(LED_RF, OUTPUT);
  digitalWrite(left_dir_pin,LOW);
  digitalWrite(left_nslp_pin,HIGH);
  digitalWrite(right_dir_pin,LOW);
  digitalWrite(right_nslp_pin,HIGH);
  
  state = FOLLOW_PATH;
  Serial.begin(9600);
}

void loop() {
	switch (state) {
		case REST:
			delay(50);
		break;

		case FOLLOW_PATH:
		followPath();
		break;

		case FIND_PATH:
			findPath();
		break;

		case DONUT:
			donut();
		break;

		default:
			state = REST;
		break;
	}
}

// Goes along the path until the car reaches the end section, then drops into donut state
void followPath() {

	// Sample execution
		// Run control loop for left and right wheels
		// check if the path is the end section (if yes --> state = DONUT;)
		digitalWrite(left_nslp_pin, HIGH);
		digitalWrite(right_nslp_pin, HIGH);

		Control::Output output = control.Update();
		analogWrite(left_pwm_pin, output.left);
		analogWrite(right_pwm_pin, output.right);

		if (isAtEndOfPath()) {
//			state = DONUT;
		}
}

// Searches for the path, then drops into follow_path state when it is found
void findPath() {}
void donut() {
	// Check if this is the ending place or the turn around place (if end, drop into rest state)
	// Check if finished yet
	// drive motors full speed in opposite directions
	// drop into find_path state
}

// Are all of the sensor values above 800?
bool isAtEndOfPath() {
	uint16_t* sensorValues = control.getSensorValues();
	for (int i = 0; i < 8; i++) {
		if (abs(sensorValues[i]) < 800) {
			return false;
		}
	}
	return true;
}
