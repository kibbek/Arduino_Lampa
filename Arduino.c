
#define MODE_OFF    0 
#define MODE_ON     1  
int incomingByte = 0;

void setup() {
   Serial.begin(115200);
   pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
 
    // send data only when you receive data:
        if (Serial.available() > 0) {
           incomingByte = Serial.read();
           Serial.println(incomingByte, DEC);
        }
  
  
  switch (incomingByte) {
     case MODE_OFF:
        digitalWrite(LED_BUILTIN, LOW);
      break;
     case MODE_ON:
        digitalWrite(LED_BUILTIN, HIGH);
      break;
               
  }
}

