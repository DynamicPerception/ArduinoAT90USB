/*

 Example Sketch for the ArduinoLUFA KeyboardHost library.
 
 Reads keys from a USB-attached keyboard and prints them to an
 attached LCD.
 
 Requires the AT90USB Hardware Definition for Arduino 1.5.x
 
 Shared under the MIT License
 
 Copyright (c) C. A. Church / Dynamic Perception LLC
 
*/
 

#include "ArduinoLUFA.h"
#include "KeyboardHost.h"
#include <LiquidCrystal.h>

    // lcd pins
const byte   LCD_RS = 21;
const byte   LCD_EN = 20;
const byte   LCD_D4 = 16;
const byte   LCD_D5 = 17;
const byte   LCD_D6 = 18;
const byte   LCD_D7 = 19;
const byte  LCD_BKL = 22;
const byte LCD_ROWS = 2;
const byte LCD_COLS = 16;

 // initialize LCD object
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

KeysPressed keys;


void setup() {

    // initialize the LCD
  lcd.begin(LCD_COLS, LCD_ROWS);
  
    // initialize the USB host
  ArduinoLUFA::init();

}



void loop() {
  
    // if the keyboard is connected, see what key (if any) has been pressed
  
  if( KeyHost.status() == STAT_CONNECTED )
    keys = KeyHost.getKeys();

    // update the LCD
    
  lcdDrawStat();
}





void lcdDrawStat() {

 static int wasStat = -50;
 static char wasKey = 0;

 bool forceUpdate = false;
 
  // keyboard status has changed?
 if(wasStat != keys.status ) {
   lcd.clear();
   forceUpdate = true;
   
   lcd.setCursor(0, 0);   
   
     // print the correct keyboard status
   switch(keys.status) {
      case STAT_NOTCONNECTED :
       lcd.print("Not Connected");
       break;
      case STAT_CONNECTED :
       lcd.print("Connected");
       break;
      case STAT_ERROR :
       lcd.print("Error");
       break;
   }

  wasStat = keys.status;
 }

  // key changed, or we had a keyboard status change?
 if( wasKey != keys.keys[0].raw || forceUpdate ) {

    
       // was no key actually pressed? (we won't want to clear out the display in this
       // case )
    if( keys.keys[0].raw == 0 ) {

      
     if( forceUpdate ) {

      lcdKeyRefresh();
      lcd.print("None");
      return;
     }
     
    } // end if( keys...
    else {
      
      lcdKeyRefresh();
    
        // print out each key pressed
      for(byte i =0; i < 6; i++) {
        if( keys.keys[i].raw != 0 )
          lcd.write( keys.keys[i].value );
      }

    }
    
    wasKey = keys.keys[0].raw;
 }
   
}


void lcdKeyRefresh() {
   lcd.setCursor(0,1);
   lcd.write("                ");
   lcd.setCursor(0,1);
   lcd.write("Last Key: ");
}


