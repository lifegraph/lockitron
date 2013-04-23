/*
 * Lifegraph Facebook Demo
 * This sketch is released to the public domain.
 */

#include <SoftwareSerial.h>
#include <sm130.h>
#include <WiFlyHQ.h>
#include <Lifegraph.h>
#include <Servo.h>

SoftwareSerial wifiSerial(9, 10);

NFCReader rfid(7, 8);

Servo servo;


/**
 * Configuration
 */
 
// Wifi configuration for a WPA network.
const char mySSID[] = "...";
const char myPassword[] = "...";

// Lifegraph Connect manages access tokens for us.
// Create an empty access token we can populate once an RFID card is tagged in.
char access_token[128] = { 0 };

// We need an application's credentials (namespace, key, and secret)
// to request a user's access tokens from Facebook.
const char app_namespace[] = "...";
const char app_key[] = "...";
const char app_secret[] = "...";
 
boolean doorLocked = true;
/**
 * Setup
 */
 
void setup()
{
  // Setup ports.
  Serial.begin(9600);
  wifiSerial.begin(9600);
 
  // Setup network connection.
  Serial.println(F("Connecting to Wifi..."));
  if (!connectWifi(&wifiSerial, mySSID, myPassword)) {
    Serial.println(F("Failed to join network."));
    while (true) {
      // Hang forever.
    }
  } else {
    Serial.println(F("Joined wifi network."));
  }
  
  servo.attach(9); 
  
  // Initialize access tokens.
  Lifegraph.configure(app_namespace, app_key, app_secret);
  
}

void loop()
{ 
  // read rfid
  Lifegraph.readIdentity(rfid, &wifiSerial, access_token);

  Serial.print(F("Found Acccess token for RFID"));
  Serial.println(access_token);
  // Grab the fbid of the corresponding rfid
  int numFound = 0;
  
  // Check if fbid is from a friend
  int status_code = Facebook.findString ( access_token, "me/friends?fields=id", "769075675" ,&numFound );
  
  Serial.print("Found Friends: ");
  Serial.println(numFound, DEC);
  
  // If it is, toggle the lock
  if (numFound) {
    toggleLock(!doorLocked);
    Serial.println("Toggling Lock!");
  }
}

/*
Toggles the state of the lock
*/
void toggleLock(boolean lockState) {
  
  // If the door is locked, unlock it
  if (lockState) servo.write(180); 
  
  // Else, lock it
  else servo.write(0);
  
  doorlocked = !doorLocked;
}


