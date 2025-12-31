#include <Servo.h>

#define trigPin 7
#define echoPin 6

#define in1 5
#define in2 4
#define in3 3
#define in4 2

#define servoPin 8

Servo myServo;

//For MOTOR FUNCTIONS

void moveForward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void moveBackward() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void turnLeft() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void turnRight() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void stopMotor() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

//For ULTRASONIC FUNCTION

long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) return 200; 

  long distance = duration * 0.034 / 2;
  return distance;
}



void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  myServo.attach(servoPin);
  myServo.write(90);

  delay(2000);
}


void loop() {

  myServo.write(90);
  delay(200);

  long frontDistance = getDistance();

  if (frontDistance > 20) {
    moveForward();
  } 
  else {

    //STOP
    stopMotor();
    delay(150);

    //GO BACK (~2 inch)
    moveBackward();
    delay(400);   
    stopMotor();
    delay(150);

    //SCAN LEFT
    myServo.write(150);
    delay(400);
    long leftDistance = getDistance();

    //SCAN CENTER
    myServo.write(90);
    delay(300);
    long centerDistance = getDistance();

    //SCAN RIGHT
    myServo.write(30);
    delay(400);
    long rightDistance = getDistance();

    //DECISION
    if (leftDistance > centerDistance && leftDistance > rightDistance) {
      turnLeft();
      delay(450);
    } 
    else if (rightDistance > centerDistance && rightDistance > leftDistance) {
      turnRight();
      delay(450);
    } 
    else {
      moveForward();  
      delay(300);
    }

    stopMotor();
  }
}
