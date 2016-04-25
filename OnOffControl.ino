#include "TM1637.h" 

// Macro Define
#define CLK_PIN         P4_2            // 7-segment display clock pin
#define DIO_PIN         P4_1            // 7-segment display data pin
#define TRIG_PIN        P5_6            // trig pin of the ultrasonic sensor
#define ECHO_PIN        P5_7            // echo pin of the ultrasonic sensor
#define PWM_PIN         P1_2            // motor PWM
#define DIR_PIN         P1_3            // motor direction

// Global Variables
TM1637 tm1637(CLK_PIN, DIO_PIN);            // 7-segment display object
int8_t bits[4] = {0};       // array of digits of the distance, for the 7-segment display

int setpoint = 50;                      // desired distance to maintain, in cm
int distance = 0;                       // measured distance to obstacles in front
int error = 0;

int getDistanceInCentimeters() {
    int duration, rangeInCentimeters;
    
    // Send pulse from ultrasonic sensor
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
	digitalWrite(TRIG_PIN, HIGH);
	delayMicroseconds(5);
	digitalWrite(TRIG_PIN, LOW);

    // Calculate the distance
	duration = pulseIn(ECHO_PIN, HIGH);
	rangeInCentimeters = duration/29/2;
	return rangeInCentimeters;
}   

void setup() {
    // Initialize 7-segment display
    tm1637.init();
    tm1637.set(BRIGHT_TYPICAL);

    pinMode(PWM_PIN, OUTPUT);  // P1_2 = PWM
    pinMode(DIR_PIN, OUTPUT);  // P1_3 = direction
}

void loop() {
    // Read the value from the sensor
    distance = getDistanceInCentimeters();
    
    // Calculate the error
    error = distance - setpoint;
            
    // Determine direction for car to move
    if (error > 0) {
        digitalWrite(DIR_PIN, HIGH);    // move forward
    } else {
        digitalWrite(DIR_PIN, LOW);     // move backward
    }
    
    // Drive the motor
    if (error != 0) {
        digitalWrite(PWM_PIN, HIGH);
    } else {
        digitalWrite(PWM_PIN, LOW);
    }
        
    // Convert the value to individual decimal digits for display
    for (int i = 3; i >= 0; i--) {
        bits[i] = distance % 10;
        distance = distance / 10;  
        tm1637.display(i, bits[i]);
    }
}