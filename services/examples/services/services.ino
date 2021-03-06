/**
 * @file
 */
/******************************************************************************
 * Copyright (c) 2013, AllSeen Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for any
 *    purpose with or without fee is hereby granted, provided that the above
 *    copyright notice and this permission notice appear in all copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

#include <Triton_WiFi.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"

#include <alljoyn.h>
#include <services.h>
#include <stdint.h>
#include <Wire.h>
int led = 13;

//your wifi ssid and password
#define WLAN_SSID       "CSU"          // cannot be longer than 32 characters!
#define WLAN_PASS       "12345abcde"
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

#define IDLE_TIMEOUT_MS  3000      // Amount of time to wait (in milliseconds) with no data 
                                   // received before closing the connection.  If you know the server
                                   // you're accessing is quick to respond, you can reduce this value.


/**************************************************************************/
/*!
    @brief  Tries to read the IP address and other connection details
*/
/**************************************************************************/
bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
  
  if(!wifi.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else
  {
    Serial.print(F("\nIP Addr: ")); wifi.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); wifi.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); wifi.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); wifi.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); wifi.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}
void DUE_led_timed(uint32_t msec)
{
    AJ_Printf("DUE_led_timed\n");
    digitalWrite(led, HIGH); // turn the LED on (HIGH is the voltage level)
    delay(msec);             // wait for a second
    digitalWrite(led, LOW);  // turn the LED off by making the voltage LOW
    delay(msec);             // wait for a second
}

void DUE_led(uint8_t on)
{
    AJ_Printf("DUE_led(%u)\n", on);
    digitalWrite(led, on ? HIGH : LOW); // turn the LED on (HIGH is the voltage level)
}

void InitHP()
{
    Wire.begin(); 
    Wire.beginTransmission(byte(0x20));  //0x40 >> 1                    
    Wire.write(byte(0x06));  
    Wire.write(byte(0x00)); 
    Wire.write(byte(0x00));
    Wire.endTransmission();  
}

void WriteHP(byte hp)
{
    int data;
    Wire.beginTransmission(byte(0x20));  //0x40 >> 1  
    data = 0xffff<<hp;  
    Wire.write(byte(0x02));
    Wire.write(data & 0xff);             
    Wire.write((data & 0xff00) >> 8);
    Wire.endTransmission(); 
}


/**************************************************************************/
/*!
    @brief  Sets up the HW and the wifi module (called automatically
            on startup)
*/
/**************************************************************************/

uint32_t ip;

// the setup routine runs once when you press reset:
void setup() {
    // initialize the digital pin as an output.
    pinMode(led, OUTPUT);
    
    InitHP();

    Serial.begin(115200);
    while (!Serial) ;

    digitalWrite(led, LOW);

Serial.println(F("Hello, wifi!\n")); 


  Serial.print("Free RAM: "); Serial.println(getFreeRam(), DEC);
  
  /* Initialise the module */
  Serial.println(F("\nInitializing..."));
  if (!wifi.begin())
  {
    Serial.println(F("Couldn't begin()! Check your wiring?"));
    while(1);
  }
  
  // Optional SSID scan
  // listSSIDResults();
 
  
  Serial.print(F("\nAttempting to connect to ")); Serial.println(WLAN_SSID);
  if (!wifi.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    while(1);
  }
   
  Serial.println(F("Connected!"));
  
  /* Wait for DHCP to complete */
  Serial.println(F("Request DHCP"));
  while (!wifi.checkDHCP())
  {
    delay(100); // ToDo: Insert a DHCP timeout!
  }  

  /* Display the IP address DNS, Gateway, etc. */  
  while (! displayConnectionDetails()) {
    delay(1000);
  }

}

// the loop routine runs over and over again forever:
void loop() {
    AJ_Printf("Hello\n");
    AJ_Main();
}


