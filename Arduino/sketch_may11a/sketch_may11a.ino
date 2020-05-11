void setup() {
  Serial.begin(9600);     // baud rate of 9600
   pinMode(LED_BUILTIN, OUTPUT);
}


void loop() {  
  Serial.write('t');              // transmit 't' command 

  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on 
  delay(500);                      
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off
  delay(500); 

}
