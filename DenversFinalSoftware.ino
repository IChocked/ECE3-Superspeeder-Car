#include <ECE3.h>
uint16_t sensorValues[8]; // right -> left, 0 -> 7
const int left_nslp_pin=31;
const int right_nslp_pin=11;// nslp ==> awake & ready for PWM
const int left_dir_pin=29;
const int right_dir_pin=30;
const int left_pwm_pin=40;
const int right_pwm_pin=39;
const int LED_RF = 41;
const int negativeThreshold = -1000;
const int positiveThreshold = 1000;
const int highSpd = 90;//drop this if car is not responsive enough
const int uTurnSpd = 255;
const int turnSpd = 0;
const int searchSpd = 70;
int timeInSearch = 0;
bool firstPass = true;

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
  ECE3_Init();
  Serial.begin(9600); 
  delay(2000);
}
void loop() {
  ECE3_read_IR(sensorValues);
  sensorValues[0]=abs((sensorValues[0]-630.4)*(1000/1397.8));
  sensorValues[1]=abs((sensorValues[1]-540)*(1000/1229.6));
  sensorValues[2]=abs((sensorValues[2]-630.4)*(1000/1513.0));
  sensorValues[3]=abs((sensorValues[3]-630.8)*(1000/1082.8));
  sensorValues[4]=abs((sensorValues[4]-562.2)*(1000/904.2));
  sensorValues[5]=abs((sensorValues[5]-675.4)*(1000/1094.2));
  sensorValues[6]=abs((sensorValues[6]-653.6)*(1000/1394.8));
  sensorValues[7]=abs((sensorValues[7]-721.8));
  int rawSum = 0;
  int weightedSum = 0;
  weightedSum += sensorValues[4];
  weightedSum += sensorValues[5]*2;
  weightedSum += sensorValues[6]*4;
  weightedSum += sensorValues[7]*8;
  weightedSum -= sensorValues[0]*8;
  weightedSum -= sensorValues[1]*4;
  weightedSum -= sensorValues[2]*2;
  weightedSum -= sensorValues[3];
  rawSum += sensorValues[0];
  rawSum += sensorValues[1];
  rawSum += sensorValues[2];
  rawSum += sensorValues[3];
  rawSum += sensorValues[4];
  rawSum += sensorValues[5];
  rawSum += sensorValues[6];
  rawSum += sensorValues[7];
  
  if(rawSum > 8000){//First check for finish line ;)
    if (!firstPass) {
      stop();
    }
    analogWrite(left_pwm_pin, uTurnSpd);
    analogWrite(right_pwm_pin, uTurnSpd);
    digitalWrite(left_dir_pin, HIGH);
    delay(190);//alter this if u turn is too wide or shallow
    digitalWrite(left_dir_pin, LOW);
    delay(300);
    analogWrite(left_pwm_pin, highSpd);
    analogWrite(right_pwm_pin, highSpd);
    firstPass = false;
  }
  else if(rawSum < 800){//next check for empty space
    while(rawSum < 800){
      if(timeInSearch < 450){
        analogWrite(right_pwm_pin, searchSpd );
        analogWrite(left_pwm_pin, turnSpd );
      }        
      else{
        analogWrite(right_pwm_pin, turnSpd );
        analogWrite(left_pwm_pin, searchSpd );        
      }
      ECE3_read_IR(sensorValues);
      sensorValues[0]=abs((sensorValues[0]-745.0)*(1000/1755.0));
      sensorValues[1]=abs((sensorValues[1]-683.3)*(1000/1816.6));
      sensorValues[2]=abs((sensorValues[2]-607.3)*(1000/1814.0)); // 
      sensorValues[3]=abs((sensorValues[3]-563.3)*(1000/1276.0)); // 
      sensorValues[4]=abs((sensorValues[4]-608.0)*(1000/1394.6)); // 
      sensorValues[5]=abs((sensorValues[5]-615.3)*(1000/1884.6));
      sensorValues[6]=abs((sensorValues[6]-525.6)*(1000/1547.6)); //
      sensorValues[7]=abs((sensorValues[7]-631.0)*(1000/1869.0));
      rawSum = 0;
      rawSum += sensorValues[0];
      rawSum += sensorValues[1];
      rawSum += sensorValues[2];
      rawSum += sensorValues[3];//assess raw sum for end track detection
      rawSum += sensorValues[4];
      rawSum += sensorValues[5];
      rawSum += sensorValues[6];
      rawSum += sensorValues[7];
      timeInSearch = (timeInSearch + 1) % 900;
     }
  }
  else if(weightedSum <= negativeThreshold){//Check if we need to make a right
    analogWrite(right_pwm_pin, turnSpd);
    analogWrite(left_pwm_pin, highSpd);
  }
  else if(weightedSum >= positiveThreshold){//Check if we need to make a left
    analogWrite(right_pwm_pin, highSpd);
    analogWrite(left_pwm_pin, turnSpd);
  }
  else{//Open Road is the only thing left ;)
    analogWrite(right_pwm_pin, highSpd);
    analogWrite(left_pwm_pin, highSpd);
  } 
  }

  void stop() {
    analogWrite(left_pwm_pin, 0);
    analogWrite(right_pwm_pin, 0);
    digitalWrite(left_nslp_pin,LOW);
    digitalWrite(right_nslp_pin,LOW);
    while (true) {
      delay(1000); 
    }
  }
