#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Message.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>

MIDI_CREATE_DEFAULT_INSTANCE();

void HandleNoteOn(byte channel, byte pitch, byte velocity)
{     
  if(pitch == 0)
    digitalWrite(12, LOW);
  if(pitch == 1)
    digitalWrite(12, HIGH);
}

void setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleNoteOn(HandleNoteOn);

  pinMode(12, OUTPUT);

  digitalWrite(12, HIGH);
  delay(500);
  digitalWrite(12, LOW);
}

void loop() {
  //unsigned long currentMillis = millis();
  MIDI.read();
}
