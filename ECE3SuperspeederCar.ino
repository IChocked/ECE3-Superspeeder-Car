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

Control control(kp, ki, kd);

bool goingOut = true;
unsigned long start_time;

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
  
  state = FIND_PATH;
  start_time = millis();
  
  Serial.begin(9600);
  Serial.println("Started");
  ECE3_Init();
  delay(2000);
}

void loop() {
	switch (state) {
		case REST:
      driveOut(LOW, LOW, 0, 0);
			delay(1000);
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
 delay(20);
 Serial.println();
}

// Goes along the path until the car reaches the end section, then drops into donut state
void followPath() {
  
	// Sample execution
		// Run control loop for left and right wheels
		// check if the path is the end section (if yes --> state = DONUT;)

		Control::Output output = control.Update();
    Serial.print(output.left);
    Serial.print("\t");
    Serial.print(output.right);

		if (isAtEndOfPath()) {
      driveOut(LOW, LOW, 0, 0);
			state = DONUT;
		}

   driveOut(LOW, LOW, output.left, output.right);
}

// Searches for the path, then drops into follow_path state when it is found
void findPath() {
  // sine wave of increasing amplitude with time
    control.Update();
    uint16_t* sensorValues = control.getSensorValues();
    for (int i = 0; i < 8; i++) {
      if (sensorValues[i] > ON_DARK_VALUE) { // found it
        state = FOLLOW_PATH;
        return;
      }
    }
    if (millis() - start_time < SEARCH_TIME) { // left curve
      driveOut(LOW, LOW, MAX_SPEED / 2, MAX_SPEED);
    } else { //right curve
      driveOut(LOW, LOW, MAX_SPEED, MAX_SPEED / 2);
    }
    
}

void donut() {
  if (!goingOut) {
    state = REST;
    return;
  }
  driveOut(LOW, HIGH, DONUT_SPEED, DONUT_SPEED);
  delay(DONUT_TIME);
  driveOut(LOW, LOW, 0, 0);
  goingOut = false;
  state = FOLLOW_PATH;
}

bool isAtEndOfPath() {
	uint16_t* sensorValues = control.getSensorValues();
	for (int i = 0; i < 8; i++) {
		if (abs(sensorValues[i]) < ON_DARK_VALUE) {
			return false;
		}
	}
	return true;
}

// Actually output drive commands. Makes it easy to comment out output and collect test data
void driveOut(int left_dir, int right_dir, int left_PWM, int right_PWM) {
  digitalWrite(left_dir_pin, left_dir);
  digitalWrite(right_dir_pin,right_dir);
  analogWrite(left_pwm_pin, left_PWM);
  analogWrite(right_pwm_pin, right_PWM);
}
