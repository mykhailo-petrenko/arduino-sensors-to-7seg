#include <Servo.h>

#define servoPin 9 // Servo

#define echoPin 2 // Echo Ultrasonic
#define trigPin 4 // Trigger Ultrasonic

#define ledPin 13 // Default Led

Servo servoDrive;
int servoPosition = 90;
int servoTask = 90;

int servoPrevPosition = 90;

#define servoStep 15

#define DistanceBreakpoint 30

int Distance = 100;
int PrevDistance = 100;

int Seek = 0;
int SeekDirection = 1;
int SeekForward = 6;
int SeekBackward = 10;

void blink( int n = 1 ) {
  for (int j = 0; j < n; j++) {
    digitalWrite(ledPin, HIGH);
    delay(400);
    digitalWrite(ledPin, LOW);
  }
}

void servoController() {
  if ( servoTask > 180 ) {
    servoTask = 180;
  } else if ( servoTask < 0 ) {
    servoTask = 0;
  }

  if ( servoPosition == servoTask ) {
    return;
  }

  servoPrevPosition = servoPosition;

//  Serial.print( servoPosition );
//  Serial.print( " - " );
//  Serial.print( servoTask );
//  Serial.print( " servoController " );

  if ( servoPosition < servoTask ) {
    servoPosition = servoPosition + servoStep;
  } else if ( servoPosition > servoTask ) {
    servoPosition = servoPosition - servoStep;
  }

//  Serial.println( servoPosition );

  servoDrive.write( servoPosition );
}

int readDistance() {
  long duration;
  
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);

  return (int)(duration/58.2);
}

void setup() {
  // Ultrasonic Pins
  pinMode( trigPin, OUTPUT );
  pinMode( echoPin, INPUT );

  // Led Pin
  pinMode( ledPin, OUTPUT );

  // Start Serial
  Serial.begin(9600);
  
  blink();

  // Attach the Servo
  servoDrive.attach( servoPin );
  
  blink();

  // Move to Center
  servoDrive.write( servoPosition );
}

void loop() {
  
  servoController();
  PrevDistance = Distance;
  Distance = readDistance();
  
  // Start Search
  if ( Seek == 0 && Distance > DistanceBreakpoint && PrevDistance < DistanceBreakpoint ) {
    Seek = SeekBackward + SeekForward + 1;
    servoTask = servoPosition + ( SeekForward * servoStep * SeekDirection );

    digitalWrite(ledPin, LOW);
//    Serial.print( Seek );
//    Serial.println( " Start Search" );
  } else
  // Stop when Found
  if ( Distance < DistanceBreakpoint ) {
    Seek = 0;
    servoTask = servoPosition;

    digitalWrite(ledPin, HIGH);
    
//    Serial.print( Seek );
//    Serial.println( " Stop when Found" );
  } else
  // Stop search
  if ( Seek == 0 ) {
    servoTask = servoPosition;
//    Serial.print( Seek );
//    Serial.println( " Stop search" );
  }

  // Change search direction
  if( Seek == SeekBackward && Distance > DistanceBreakpoint ) {
    SeekDirection = SeekDirection * -1;
    
    servoTask = servoPosition + ( SeekBackward * servoStep * SeekDirection );

//    Serial.print( Seek );
//    Serial.println( " Change search direction" );
  }

  if ( Seek > 0 && servoPosition == 180 ) {
    Seek = SeekBackward;
    SeekDirection = -1;
    servoTask = servoPosition + ( Seek * servoStep * SeekDirection );
  }

  if ( Seek > 0 && servoPosition == 0 ) {
    Seek = SeekBackward;
    SeekDirection = 1;
    servoTask = servoPosition + ( Seek * servoStep * SeekDirection );
  }

  // Seek decrement
  if ( Seek > 0 ) {
    Seek = Seek - 1;
//    Serial.print( Seek );
//    Serial.println( " Seek decrement" );
  }

//  Serial.println( Distance );
  delay( 60 );
}




