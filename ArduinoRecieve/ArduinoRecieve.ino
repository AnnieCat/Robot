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
  if(channel == 1)
  {
    myX = pitch;
    myServoX.write(myX);
  }
  if(channel == 2)
  {
    myY = pitch;
    myServoY.write(myY);
  }
}

void setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleNoteOn(HandleNoteOn);
  
  myServoX.attach(3);
  myServoY.attach(4);

  //for the LED
  pinMode(12, OUTPUT);
}

void loop() {
  //unsigned long currentMillis = millis();
  MIDI.read();
}
