
// Define the states
const int REST = 0;
const int FOLLOW_PATH = 1;
const int FIND_PATH = 2;
const int DONUT = 3;

// current state of the car
int state = REST;

void setup() {
}

void loop() {
	switch (state) {
		case REST:
			delay(50);
		break;

		case FOLLOW_PATH:
		break;

		case FIND_PATH:
		break;

		case DONUT:
		break;

		default:
		state = REST;
		break; 
	}
}

// Goes along the path until the car reaches the end section, then drops into donut state
void followPath() {
	// Sample execution
		// check if the path is the end section (if yes --> state = DONUT;)
		// Run control loop for left and right wheels
}

// Searches for the path, then drops into follow_path state when it is found
void findPath() {}
void donut() {
	// Check if this is the ending place or the turn around place (if end, drop into rest state)
	// Check if finished yet
	// drive motors full speed in opposite directions
	// drop into find_path state
}
