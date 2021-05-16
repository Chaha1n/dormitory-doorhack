#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include<Servo.h>

SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int fingerprintID = 0;
bool isDoorLocked = false;
Servo myservo;

int getFingerprintID() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;
 
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;
 
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
 
  return finger.fingerID;
}

void toggleDoorLock(){
  myservo.attach(9);
  if(isDoorLocked)
    myservo.write(180); 
  else 
    myservo.write(0);
   delay(300);
  isDoorLocked = !isDoorLocked;
  myservo.detach();
}


void setup() {
  // put your setup code here, to run once:
  finger.verifyPassword();
  Serial.begin(9600);
  finger.begin(57600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  fingerprintID = getFingerprintID();
  delayMicroseconds(100);
  if (fingerprintID >0 && fingerprintID<4){
    Serial.println(fingerprintID);
    toggleDoorLock();
  }
}
