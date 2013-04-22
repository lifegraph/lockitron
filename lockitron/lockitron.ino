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
 
// Pin our LED is connected to.
int light = 13;
 
 
/**
 * Setup
 */
 
void setup()
{
  // Setup ports.
  Serial.begin(9600);
  wifiSerial.begin(9600);
  pinMode(light, OUTPUT);
 
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
  
  myservo.attach(9); 
  
  // Initialize access tokens.
  Lifegraph.configure(app_namespace, app_key, app_secret);
  Lifegraph.readIdentity(rfid, &wifiSerial, access_token);
}

void loop()
{ 
  // Request if there are unread notifications on Facebook.
  int unread_count;
  int status_code = Facebook.unreadNotifications ( access_token, &unread_count );
  
  // If the request is successful (HTTP OK), update the light accordingly.
  if (status_code == 200) {
    digitalWrite(light, unread_count > 0 ? HIGH : LOW);
  }

  // Notify terminal of our status.
  Serial.print("HTTP Status Code: ");
  Serial.print(status_code);
  Serial.print(" Unread notifications: ");
  Serial.println(unread_count);
}

void openDoor() {
  servo.write(180); 
}

void closeDoor() {
 servo.write(0); 
}
