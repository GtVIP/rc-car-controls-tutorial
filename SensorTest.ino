#include "TM1637.h" 

// Macro Define
#define CLK_PIN         P4_2            // 7-segment display clock pin
#define DIO_PIN         P4_1            // 7-segment display data pin
#define TRIG_PIN        P5_6            // trig pin of the ultrasonic sensor
#define ECHO_PIN        P5_7            // echo pin of the ultrasonic sensor

// Global Variables
TM1637 tm1637(CLK_PIN, DIO_PIN);            // 7-segment display object
int8_t bits[4] = {0};       // array of digits of the distance, for the 7-segment display

int distance = 0;                           // measured distance to obstacles in front

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
}

void loop() {
    // Read the value from the sensor
    distance = getDistanceInCentimeters();
    
    // Convert the value to individual decimal digits for display
    for (int i = 3; i >= 0; i--) {
        bits[i] = distance % 10;
        distance = distance / 10;  
        tm1637.display(i, bits[i]);
    }
}