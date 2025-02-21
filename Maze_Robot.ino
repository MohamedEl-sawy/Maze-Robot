void setup() {// Motor A connections (Right Motors)
int enA = 3;
int MOTOR_RIGHT_PIN1 = 2;
int MOTOR_RIGHT_PIN2 = 4;

// Motor B connections (Left Motors)
int enB = 10;
int MOTOR_LEFT_PIN1 = 5;
int MOTOR_LEFT_PIN2 = 6; 

#define trigPin 7
#define echoPin 8
#define trigPinRight 9  
#define echoPinRight 11
#define trigPinLeft 12
#define echoPinLeft 13   

const int SPEED_MIN = 80;
const int SPEED_MAX = 120;
const int TURN_SPEED = 110;
const int TURN_SLOW_SPEED = 60;
const int DISTANCE_THRESHOLD = 25;

enum Direction { FORWARD, BACKWARD, LEFT, RIGHT, STOP };
int cm = 0;
int cmRight = 0;
int cmLeft = 0;

// Function prototypes
float readUltrasonicDistance();
float readUltrasonicDistanceRight();
float readUltrasonicDistanceLeft();
void directionControl(Direction direction);
void setSpeed(int speed, int speedB = -1);

void setup() {
    Serial.begin(9600);
    
    // Set motor pins as OUTPUT
    pinMode(enA, OUTPUT);
    pinMode(enB, OUTPUT);
    pinMode(MOTOR_RIGHT_PIN1, OUTPUT);
    pinMode(MOTOR_RIGHT_PIN2, OUTPUT);
    pinMode(MOTOR_LEFT_PIN1, OUTPUT);
    pinMode(MOTOR_LEFT_PIN2, OUTPUT);
    
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    
    pinMode(trigPinRight, OUTPUT);
    pinMode(echoPinRight, INPUT);
    
    pinMode(trigPinLeft, OUTPUT);
    pinMode(echoPinLeft, INPUT);

    // Stop all motors before setting new direction
    directionControl(STOP);
} 

void loop() {
    // Measure distances using ultrasonic sensors
    cm = readUltrasonicDistance();
    cmRight = readUltrasonicDistanceRight();
    cmLeft = readUltrasonicDistanceLeft();

    // Display results
    Serial.print("Front: "); Serial.print(cm == -1 ? "______Error" : String(cm) + " cm");
    Serial.print(" | Right: "); Serial.print(cmRight == -1 ? "______Error" : String(cmRight) + " cm");
    Serial.print(" | Left: "); Serial.println(cmLeft == -1 ? "______Error" : String(cmLeft) + " cm");

    // Stop immediately if all paths are blocked (dead-end)
//    if (cm < 10 && cmRight < 18 && cmLeft < 18) {
//        directionControl(STOP);
//        Serial.println("Dead-end detected! Reversing...");
//      
//        return;
//    }
    int speed = map(cm, 10, 50, SPEED_MIN, SPEED_MAX);
    speed = constrain(speed, SPEED_MIN, SPEED_MAX);
    setSpeed(speed);
   
    if (cm > 15) {
        directionControl(FORWARD);
    }
    // If the path ahead is clear, move forward
    if (cm < 18) {
        if (cmRight > cmLeft) {
            directionControl(RIGHT);
        } else {
            directionControl(LEFT);
        }
    } else if (cmRight > 18 && cmRight > cmLeft) {
        directionControl(RIGHT);
    } else if (cmLeft > 18) {
        directionControl(LEFT);
    } else {
        directionControl(FORWARD);
    }
    
}

void directionControl(Direction direction) {
    switch (direction) {
        case FORWARD:
            digitalWrite(MOTOR_RIGHT_PIN1, HIGH);
            digitalWrite(MOTOR_RIGHT_PIN2, LOW);
            digitalWrite(MOTOR_LEFT_PIN1, HIGH);
            digitalWrite(MOTOR_LEFT_PIN2, LOW);
            Serial.println("Moving FORWARD");
            break;
        case BACKWARD:
            digitalWrite(MOTOR_RIGHT_PIN1, LOW);
            digitalWrite(MOTOR_RIGHT_PIN2, HIGH);
            digitalWrite(MOTOR_LEFT_PIN1, LOW);
            digitalWrite(MOTOR_LEFT_PIN2, HIGH);
            Serial.println("Moving BACKWARD");
            break;
        case LEFT:
            digitalWrite(MOTOR_RIGHT_PIN1, HIGH);
            digitalWrite(MOTOR_RIGHT_PIN2, LOW);
            digitalWrite(MOTOR_LEFT_PIN1, LOW);
            digitalWrite(MOTOR_LEFT_PIN2, HIGH);
            Serial.println("Turning LEFT");
            break;
        case RIGHT:
            digitalWrite(MOTOR_RIGHT_PIN1, LOW);
            digitalWrite(MOTOR_RIGHT_PIN2, HIGH);
            digitalWrite(MOTOR_LEFT_PIN1, HIGH);
            digitalWrite(MOTOR_LEFT_PIN2, LOW);
            Serial.println("Turning RIGHT");
            break;
        case STOP:
            digitalWrite(MOTOR_RIGHT_PIN1, LOW);
            digitalWrite(MOTOR_RIGHT_PIN2, LOW);
            digitalWrite(MOTOR_LEFT_PIN1, LOW);
            digitalWrite(MOTOR_LEFT_PIN2, LOW);
            Serial.println("STOPPED");
            break;
    }
}

void setSpeed(int speedA, int speedB) {
    if (speedB == -1) speedB = speedA;
    analogWrite(enA, speedA);
    analogWrite(enB, speedB);
}

// Function to read front sensor distance
float readUltrasonicDistance() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH, 30000); // Timeout after 30ms
    float distance = (duration * 0.0343) / 2;

    return (distance >= 2 && distance <= 400) ? distance : -1;
}



// Function to read right sensor distance
float readUltrasonicDistanceRight() {
  float distance = -1;
  for (int i = 0; i < 3; i++) {  // Retry up to 3 times for a valid reading
    digitalWrite(trigPinRight, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinRight, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinRight, LOW);

    long duration = pulseIn(echoPinRight, HIGH, 30000); // Timeout after 30ms
    float tempDistance = (duration * 0.0343) / 2;
    if (tempDistance >= 2 && tempDistance <= 400) {
            distance = tempDistance;
            break; // Valid reading, break loop
        }
    }
    return distance;
}


// Function to read left sensor distance
float readUltrasonicDistanceLeft() {
    digitalWrite(trigPinLeft, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinLeft, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinLeft, LOW);

    long duration = pulseIn(echoPinLeft, HIGH, 30000); // Timeout after 30ms
    float distance = (duration * 0.0343) / 2;

    return (distance >= 2 && distance <= 400) ? distance : -1;
}
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
