#include <Arduino.h>

// Motor Pins
const int motorRightForward = 2, motorRightBackward = 4;
const int motorLeftForward = 5, motorLeftBackward = 6;
const int enableRight = 3, enableLeft = 10;

// Ultrasonic Sensor Pins
const int trigPinFront = 7, echoPinFront = 8;
const int trigPinRight = 9, echoPinRight = 11;
const int trigPinLeft = 12, echoPinLeft = 13;

// Track previous turns (1 = Left, 2 = Right, 3 = U-Turn)
int lastMoves[5] = {0, 0, 0, 0, 0}; // Stores last 5 movements

// Function to Measure Distance Using Ultrasonic Sensors
long getDistance(int trigPin, int echoPin) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    return pulseIn(echoPin, HIGH) * 0.034 / 2;
}

// Move Forward with Speed Control
void moveForward(int speedLeft, int speedRight) {
    digitalWrite(motorLeftForward, HIGH);
    digitalWrite(motorRightForward, HIGH);
    analogWrite(enableLeft, speedLeft);
    analogWrite(enableRight, speedRight);
}

// Stop the Robot
void stopRobot() {
    digitalWrite(motorLeftForward, LOW);
    digitalWrite(motorRightForward, LOW);
    digitalWrite(motorLeftBackward, LOW);
    digitalWrite(motorRightBackward, LOW);
}

// Turn Right While Avoiding Walls
void turnRight() {
    digitalWrite(motorLeftForward, HIGH);
    digitalWrite(motorRightBackward, HIGH);
    analogWrite(enableLeft, 140);
    analogWrite(enableRight, 120);
    delay(400);
    stopRobot();
    
    // Store move in memory
    trackMovement(2);
}

// Turn Left While Avoiding Walls
void turnLeft() {
    digitalWrite(motorRightForward, HIGH);
    digitalWrite(motorLeftBackward, HIGH);
    analogWrite(enableLeft, 120);
    analogWrite(enableRight, 140);
    delay(400);
    stopRobot();
    
    // Store move in memory
    trackMovement(1);
}

// Rotate 180 Degrees Quickly if Trapped
void turnAroundFast() {
    digitalWrite(motorLeftForward, HIGH);
    digitalWrite(motorRightBackward, HIGH);
    analogWrite(enableLeft, 200);
    analogWrite(enableRight, 200);
    delay(500);
    stopRobot();
    
    // Store move in memory
    trackMovement(3);
}

// Function to Store Last Moves
void trackMovement(int move) {
    for (int i = 4; i > 0; i--) {
        lastMoves[i] = lastMoves[i - 1];
    }
    lastMoves[0] = move;
}

// Check if a move has been repeated recently
bool isRepeating() {
    return (lastMoves[0] == lastMoves[2] && lastMoves[1] == lastMoves[3]);
}

// Navigate the Maze Based on Sensor Data
void navigateMaze() {
    long frontDistance = getDistance(trigPinFront, echoPinFront);
    long rightDistance = getDistance(trigPinRight, echoPinRight);
    long leftDistance = getDistance(trigPinLeft, echoPinLeft);

    Serial.print("Front: "); Serial.print(frontDistance);
    Serial.print(" Right: "); Serial.print(rightDistance);
    Serial.print(" Left: "); Serial.println(leftDistance);

    if (frontDistance > 35) {  
        if (rightDistance < 15 && leftDistance < 15) {
            moveForward(100, 120);
        } else if (leftDistance < 15) {
            moveForward(120, 100);
        } else {
            moveForward(120, 120);
        }
    } 
    else if (frontDistance <= 35 && frontDistance > 25) {  
        moveForward(90, 90);
    } 
    else if (frontDistance <= 25 && frontDistance > 15) {  
        moveForward(60, 60);
    }
    else {  
        stopRobot();
        delay(500);

        if (frontDistance < 15 && rightDistance < 15 && leftDistance < 15) {
            if (isRepeating()) {
                turnLeft(); // If we are looping, change direction
            } else {
                turnAroundFast();
            }
        }
        else if (rightDistance > 20) {  
            turnRight();
        } 
        else if (leftDistance > 20) {  
            turnLeft();
        }
        else {  
            turnAroundFast();
        }
    }
}

void setup() {
    Serial.begin(9600);
    
    pinMode(motorRightForward, OUTPUT);
    pinMode(motorRightBackward, OUTPUT);
    pinMode(motorLeftForward, OUTPUT);
    pinMode(motorLeftBackward, OUTPUT);
    pinMode(enableRight, OUTPUT);
    pinMode(enableLeft, OUTPUT);
    
    pinMode(trigPinFront, OUTPUT);
    pinMode(echoPinFront, INPUT);
    pinMode(trigPinRight, OUTPUT);
    pinMode(echoPinRight, INPUT);
    pinMode(trigPinLeft, OUTPUT);
    pinMode(echoPinLeft, INPUT);
}

void loop() {
    navigateMaze();
    delay(100);
}
