// Base code.
// 
// *  NOTE: this code will do only three things:
// *    --rotate one wheel, and 
// *    --blink the right front mainboard LED.
// *    
// *  You will need to add more code to
// *  make the car do anything useful. 
// 

//#include <ECE3_LCD7.h>
#include <ECE3.h>
uint16_t sensorValues[8]; // right -> left, 0 -> 7

const int left_nslp_pin=31;
const int right_nslp_pin=11;// nslp ==> awake & ready for PWM
const int left_dir_pin=29;
const int right_dir_pin=30;
const int left_pwm_pin=40;
const int right_pwm_pin=39;

const int LED_RF = 41;

///////////////////////////////////
void setup() {
// put your setup code here, to run once:
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


  
//  ECE3_Init();

// set the data rate in bits/second for serial data transmission
  ECE3_Init();
  Serial.begin(9600); 
  delay(2000); //Wait 2 seconds before starting 
  
}

void loop() {
  // put your main code here, to run repeatedly: 

  ECE3_read_IR(sensorValues);
    for (unsigned char i = 0; i < 8; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t'); // tab to format the raw data into columns in the Serial monitor
  }
  Serial.println();


  sensorValues[0]=abs((sensorValues[0]-630.4)*(1000/1397.8));
  sensorValues[1]=abs((sensorValues[1]-540)*(1000/1229.6));
  sensorValues[2]=abs((sensorValues[2]-630.4)*(1000/1513.0));
  sensorValues[3]=abs((sensorValues[3]-630.8)*(1000/1082.8));
  sensorValues[4]=abs((sensorValues[4]-562.2)*(1000/904.2));
  sensorValues[5]=abs((sensorValues[5]-675.4)*(1000/1094.2));
  sensorValues[6]=abs((sensorValues[6]-653.6)*(1000/1394.8));
  sensorValues[7]=abs((sensorValues[7]-721.8)*(1000/581.0));
                
    for (unsigned char i = 0; i < 8; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t'); // tab to format the raw data into columns in the Serial monitor
  }
  Serial.println();                

int weightedSum = 0;

    weightedSum -= sensorValues[0]*8;
    weightedSum -= sensorValues[1]*4;
    weightedSum -= sensorValues[2]*2;
    weightedSum -= sensorValues[3]*1;//adds negative weight to the first 4 sensors -8 ,-4, -2, and -1 from right to center, then adds it to the sum
    weightedSum += sensorValues[4]*1;
    weightedSum += sensorValues[5]*2;
    weightedSum += sensorValues[6]*4;
    weightedSum += sensorValues[7]*8;//adds positive weight to the final four sensors 1, 2, 4, and 8 from center to left, then adds it to the sum
    
  Serial.print(weightedSum);
  Serial.println();
  int negativeThreshold = -1000;
  int positiveThreshold = 1000;

  if(weightedSum < negativeThreshold){
    digitalWrite(right_nslp_pin, LOW);
    digitalWrite(left_nslp_pin, HIGH);
  }
  else if(weightedSum > positiveThreshold){
    digitalWrite(left_nslp_pin, LOW);
    digitalWrite(right_nslp_pin, HIGH);
  }
  else if(weightedSum >= negativeThreshold && weightedSum <= positiveThreshold && (sensorValues[3] + sensorValues[4])>200){
    digitalWrite(right_nslp_pin, HIGH);
    digitalWrite(left_nslp_pin, HIGH);
  }
  else{
    //find line protocol goes here
  }
  int leftSpd = 30;
  int rightSpd = 30;
  analogWrite(left_pwm_pin,leftSpd);
  analogWrite(right_pwm_pin,rightSpd);
//  ECE3_read_IR(sensorValues);
  digitalWrite(LED_RF, HIGH);

  delay(100);
  digitalWrite(LED_RF, LOW);
 // digitalWrite(left_dir_pin, HIGH);
 // delay(500);
 // digitalWrite(left_dir_pin, LOW);
  
//  ECE3_read_IR(sensorValues);
 // digitalWrite(left_dir_pin, HIGH);
  //digitalWrite(right_dir_pin, HIGH);

  
  }
