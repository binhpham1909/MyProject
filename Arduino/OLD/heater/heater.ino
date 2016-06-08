#include <dht.h>
#include <PID_v1.h>
#include <ACPWM.h>

 /* AC POWER CONTROL WITH PWM AND ZERO CROSS DETECTION */
 /* AUTHOR: Dimitris El. Vassis - 2013 */
#define DHT22_PIN 5
#define ZERO_PIN 2 //Input pin from zero cross detector
#define PWM_PIN 9 //Output pin to TRIAC / SSR
#define DHT22_PIN 6 // Input DHT 
int SET_PIN = A0; //Analog pin for setting the dutyCycle value with a pontentiometer
//Define Variables we'll be connecting to (PID)
double Setpoint, Input, Output;
//Specify the links and initial tuning parameters
double Kp=2, Ki=5, Kd=1; 
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
dht DHT;

 void setup()
 {
    Serial.begin(115200);
    Serial.println("dht22_test.ino");
    Serial.print("LIBRARY VERSION: ");
    Serial.println(DHT_LIB_VERSION);
    Serial.println();
    Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)\tTime (us)");
    pinMode(ZERO_PIN, INPUT);
    pinMode(PWM_PIN, OUTPUT);
    //Initialize PWM operation.
    //Mains frequency: 50Hz.
    //Zero crossing point reached whenever pulse to PIN2 changes
    //Duty cycle = 0..255. 0:always off. 255: always on. 150: 59% on.
    ACpwm.initialize(50,ZERO_PIN,CHANGE,PWM_PIN,255);
    //Latching when voltage is positive: 3 microseconds.
    //Latching when voltage is negative: 5 microseconds.
    ACpwm.setLatch(3,5); 

      //initialize the variables we're linked to
//  Input = analogRead(PIN_INPUT); Input is value of tmp
  Setpoint = 100;

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
 }
 
 void loop() {
  //Adjust the power with the potentiometer
  //Set the duty cycle equal to the value of the potentiometer.
  Input = analogRead(PIN_INPUT);
  myPID.Compute();
  analogWrite(PIN_OUTPUT, Output);
    ACpwm.setDutyCycle(analogRead(SET_PIN));
 }
