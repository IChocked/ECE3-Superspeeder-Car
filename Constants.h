// Min values to negate noise
const int theshold = 1500;
const int MAX_ERR = 8000 + 2000; // Full on the farthest sensor + half on the second farthest sensor

// PID constants
const double kp = 0.002; // 0.002
const double ki = 0;
const double kd = 0.06; //

const int MAX_SPEED = 90;
const int ERR_SPEED = 50;
const int DONUT_SPEED = 200;

const int DONUT_TIME = 290;

const int SEARCH_TIME = 4000;
const int ON_DARK_VALUE = 600;
const int MIN_ON_DARK = 4; // minimum number of dark sensors to trigger end zone check
