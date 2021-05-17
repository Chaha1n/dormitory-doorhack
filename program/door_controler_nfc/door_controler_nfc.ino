
#include <SoftwareSerial.h>
#include<Servo.h>
#include <PN532_SWHSU.h>

#include <PN532.h>
#define SERVO_PIN 9


SoftwareSerial SWSerial( 10, 11 ); // RX, TX

PN532_SWHSU pn532swhsu( SWSerial );

PN532 nfc( pn532swhsu );

bool isDoorLocked = false;
Servo myservo;

boolean compareUID(uint8_t a[],uint8_t b[]){
  for(int i=0;i<7;i++){
      if(a[i] != b[i])
      return false;
    }  
    return true;
}
void toggleDoorLock(){
  myservo.attach(SERVO_PIN);
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
  Serial.begin(115200);

  Serial.println("Hello Maker!");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();

  if (! versiondata) {

    Serial.print("Didn't Find PN53x Module");

    while (1); // 
  }Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX);

  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC);

  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);

  // Configure board to read RFID tags

  nfc.SAMConfig();

  Serial.println("Waiting for an ISO14443A Card ...");

}

void loop() {
  // put your main code here, to run repeatedly:


  boolean success;

  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID

  uint8_t uidLength;                       // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

  if (success) {

    if(compareUID(MY_WATCH,uid) || compareUID(MY_TAG,uid)){
      Serial.println("Verified");
      toggleDoorLock();
      }
  }

}
