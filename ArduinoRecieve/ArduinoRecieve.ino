#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Message.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>

#include <Servo.h>

Servo myServoX;
Servo myServoY;

int myX;
int myY;


MIDI_CREATE_DEFAULT_INSTANCE();

void HandleNoteOn(byte channel, byte pitch, byte velocity)
{  
  if(velocity>15)
  {
    digitalWrite(12,HIGH);
    delay(100);
    digitalWrite(12, LOW);  
  }
  if(velocity<15)
  {
    digitalWrite(12,HIGH);
    delay(100);
    digitalWrite(12, LOW);
    delay(100);
    digitalWrite(12, HIGH);  
    delay(100);
    digitalWrite(12, LOW);
  }
}

void setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleNoteOn(HandleNoteOn);

  delay(500);
  digitalWrite(12,HIGH);
    delay(500);
    digitalWrite(12, LOW);
    delay(500);
    digitalWrite(12, HIGH);  
    delay(500);
    digitalWrite(12, LOW);

  //for the LED
  pinMode(12, OUTPUT);
}

void loop() {
  //unsigned long currentMillis = millis();
  MIDI.read();
}
