#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Message.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>

#include <Servo.h>

Servo myServoX;
Servo myServoY;

int myX = 90;
int myY = 60;


MIDI_CREATE_DEFAULT_INSTANCE();

void HandleNoteOn(byte channel, byte pitch, byte velocity)
{  

  if(channel == 1)
  {
    digitalWrite(12, LOW);
    myServoX.write(90);
    myServoY.write(60);  
  }
  if(channel > 1)
  {
    digitalWrite(12, HIGH);
    myX = pitch + 50;
    myY = velocity + 20;
  }
}

void setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleNoteOn(HandleNoteOn);

  myServoX.attach(10);
  myServoY.attach(9);

  myServoX.write(90);
  myServoY.write(60);

  pinMode(12,OUTPUT);

  digitalWrite(12,LOW);
}

void loop() {
  //unsigned long currentMillis = millis();
  MIDI.read();

  myServoX.write(myX);
  myServoY.write(myY);
  
}
