#include <Servo.h>

// --- PIN DEFINITIONS ---
#define SERVO_PIN 3
#define TRIGGER_PIN 12
#define ECHO_PIN 11

// --- Motor Pins ---
#define LEFT_ENABLE 10
#define RIGHT_ENABLE 9

#define LEFT_MOTOR_PIN1 7
#define LEFT_MOTOR_PIN2 6
#define RIGHT_MOTOR_PIN1 5
#define RIGHT_MOTOR_PIN2 4

// --- Distance limit ---
#define DISTANCE_LIMIT 20 

// --- Turning time ---
#define TURN_TIME 400

// --- Servo Motor object ---
Servo servo_motor;

// --- Setup function ----------
// This function is used to
// initialize input/output pins,
// attach the servo motor and
// do general configurations.
// -----------------------------
void setup() 
{
  // Open the serial output (Console)
  Serial.begin(9600);
  
  // Setup Left Motors
	pinMode(LEFT_ENABLE, OUTPUT);
  pinMode(LEFT_MOTOR_PIN1, OUTPUT);
  pinMode(LEFT_MOTOR_PIN2, OUTPUT);

  digitalWrite(LEFT_ENABLE, HIGH);

  // Setup Right Motors
	pinMode(RIGHT_ENABLE, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN1, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN2, OUTPUT);

  digitalWrite(RIGHT_ENABLE, HIGH);

  // Setup Ultrasonic Sensor
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Setup Servo Motor
  servo_motor.attach(SERVO_PIN);
  servo_motor.write(90);
  
  // Delay before starting loop
  delay(500);
}

// --- Loop Function ----------
// This function has the main
// loop of the program. It
// implements the basic logic.
// -----------------------------
void loop() 
{
  // Read the distance from Ultrasonic sensor
  int distance = getDistance();
  
  // Print distance to console
  Serial.print("Distance: ");
  Serial.println(distance);

  // Check if there is an object
  if (distance < DISTANCE_LIMIT) 
  {
    // Stop the UGV
  	stopCar();
      
    delay(500);

    // Look Right
    Serial.println("Looking right...");
    servo_motor.write(0); 
      
    delay(1000);
      
    int rightDist = getDistance();

    // Print distance to console
    Serial.print("Distance: ");
    Serial.println(rightDist);

    // Look Left
    Serial.println("Looking left...");
    servo_motor.write(180);
      
    delay(1000);
    
    int leftDist = getDistance();

    // Print distance to console
    Serial.print("Distance: ");
    Serial.println(leftDist);    

    // Return to Center
    Serial.println("Centralizing...");
    servo_motor.write(90);
      
    delay(500);

    // Decide where to turn
    if (rightDist > leftDist) 
    {
      Serial.print("Turning right...");
      turnRight(); 
    } 
    else 
    {
      Serial.print("Turning left...");
      turnLeft();
    }

    // Explicity stop the UGV and move it forward
    delay(TURN_TIME); 
    stopCar();
    delay(100);
    moveForward();
  } 
  else 
  {
    // No obstacle, keep moving forward
    moveForward();
  }
  
  delay(100); 
}

// --- Move Forward Function ---
void moveForward() 
{
  digitalWrite(LEFT_MOTOR_PIN1, HIGH);
  digitalWrite(LEFT_MOTOR_PIN2, LOW);
  
  digitalWrite(RIGHT_MOTOR_PIN1, LOW);
  digitalWrite(RIGHT_MOTOR_PIN2, HIGH);
}

// --- Stop Car Function ---
void stopCar() 
{
 	digitalWrite(LEFT_MOTOR_PIN1, LOW);
 	digitalWrite(LEFT_MOTOR_PIN2, LOW);
  
	digitalWrite(RIGHT_MOTOR_PIN1, LOW);
	digitalWrite(RIGHT_MOTOR_PIN2, LOW);
}

// --- Turn Left Function ---
void turnLeft() 
{
  digitalWrite(LEFT_MOTOR_PIN1, LOW);
  digitalWrite(LEFT_MOTOR_PIN2, HIGH); // Left motor back
  
  digitalWrite(RIGHT_MOTOR_PIN1, LOW); // Right motor forward
  digitalWrite(RIGHT_MOTOR_PIN2, HIGH);
}

// --- Turn Right Function ---
void turnRight() 
{
	digitalWrite(LEFT_MOTOR_PIN1, HIGH); // Left motor forward
  digitalWrite(LEFT_MOTOR_PIN2, LOW);
  
  digitalWrite(RIGHT_MOTOR_PIN1, HIGH); // Right motor back
  digitalWrite(RIGHT_MOTOR_PIN2, LOW);
}

// --- Get Distance Function ---
int getDistance() 
{
 	digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) return 999;

  int distance = duration * 0.034 / 2; // Convert to cm
  	
  return distance;
}