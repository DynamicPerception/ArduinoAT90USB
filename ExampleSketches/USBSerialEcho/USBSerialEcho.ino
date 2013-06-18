/* 

 Simple USBSerial Echo
 
 C. A. Church
 
 This sketch is shared in the Public Domain
 */
 

void setup() {
  // put your setup code here, to run once:
  USBSerial.begin(9600);
  
  while( !USBSerial ) {
    ; // do nothing until computer connects
  }
}

void loop() {
  
  
    // Do you hear an echo in here?
  
  if( USBSerial.available() ) {
    char f = USBSerial.read();
    USBSerial.write(f);
  }
    
}
