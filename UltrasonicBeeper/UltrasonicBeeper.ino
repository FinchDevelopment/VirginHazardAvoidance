/*
Finch

A simple ultrasonic rangefinder with audio alerts 
for close objects. 

*/

const int pingPin = 7;
const int recvPin = 8;
const int speakerPin = 6;

unsigned long prevTime = millis();
int isBuzzing = 0;
int dist = 0;
int counter = 0;
int buzzBool = 0;
int maxDist = 150;
int buzzOn = 200;
int buzzOff = 0;

void setup() {
  Serial.begin(9600);
  pinMode(pingPin, OUTPUT);
  pinMode(recvPin, INPUT);
  pinMode(speakerPin, OUTPUT);
}

void loop()
{
  int cm = getDistanceFromUltrasonic(pingPin, recvPin);
  
  if(cm) counter++;
  else counter = 0;
  
  //Turn on buzz
  if(!isBuzzing && cm < 150 && counter > 8){
    isBuzzing = 1;
    buzzBool = 1;
    dist = dist*0.1 + cm*0.9;
    buzzOff = 1000 * (double(min(maxDist, dist-30)) / maxDist) + 20;
    analogWrite(speakerPin, 128);
    prevTime = millis();
  //Turn off buzz
  } else if(buzzBool && prevTime + buzzOn < millis()){
    buzzBool = 0;
    analogWrite(speakerPin, 0);
    prevTime = millis();
  //Reset for next approach
  } else if(!buzzBool && prevTime + buzzOff < millis()){
    isBuzzing = 0;
    prevTime = millis();
  }
  
  //if(!counter){
  //  isBuzzing = 0;
  //  buzzBool = 0;
  //  prevTime = millis();
  //  analogWrite(speakerPin, 0);
  //}
  

  delay(10);
}

long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}

int getDistanceFromUltrasonic(int pingPin, int recvPin){
  long duration, cm;
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(100);
  digitalWrite(pingPin, LOW);
  duration = pulseIn(recvPin, HIGH, 20000);
  digitalWrite(recvPin, HIGH);
  cm = microsecondsToCentimeters(duration);
  return cm;
}
