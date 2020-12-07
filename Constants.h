// Min values to negate noise
const int theshold = 1500;
const int MAX_ERR = 8000 + 3000; // Full on the farthest sensor + half on the second farthest sensor

// PID constants
const double kp = 0.00085; // 0.0001 understeer, 0.001 oversteer, 0.0008 worked really well alone, after tried increasing speed to 120 but needed more correction and couldn't find the right values (increaed p and d), changed back to 0.00085 and worked well
const double ki = 0;
const double kd = 0.008; // started 0.01 worked really well, 0.008 almost flawless! Smooth, well done, etc., as battery dropped had to decrease to 0.005 this because of understeer though, 0.007 still understeer, oversteer leading up to donut and off decrease to 0.006, changed back to 0.008 and worked well not sure why didn't have understeering issues anymore

const int MAX_SPEED = 90; // 90 105 120
const int ERR_SPEED = 50;
const int DONUT_SPEED = 200;

const int DONUT_TIME = 290;

const int SEARCH_TIME = 4000;
const int ON_DARK_VALUE = 600;
const int MIN_ON_DARK = 4; // minimum number of dark sensors to trigger end zone check
