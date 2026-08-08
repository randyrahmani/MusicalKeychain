#include <Arduino.h>

// Can be moved in header file i.e notes.h
#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))
#define Gb5 831
#define F5 698
#define Ab5 932
#define C6 1047
#define Cb6 1109
#define Db5 622
#define E5 659
#define BUZZER_PIN 13

const int midi1[111][3] = {
 {Gb5, 88, 88},
 {Gb5, 88, 88},
 {F5, 88, 88},
 {Gb5, 176, 176},
 {Gb5, 88, 88},
 {Ab5, 265, 88},
 {Gb5, 176, 353},
 {Ab5, 176, 176},
 {Ab5, 88, 88},
 {Ab5, 176, 176},
 {C6, 353, 0},
 {Cb6, 529, 353},
 {C6, 88, 88},
 {C6, 88, 88},
 {C6, 88, 88},
 {C6, 88, 88},
 {C6, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Gb5, 88, 88},
 {Gb5, 88, 88},
 {Gb5, 88, 88},
 {Ab5, 88, 88},
 {Cb6, 88, 88},
 {Db5, 265, 88},
 {C6, 88, 88},
 {C6, 88, 88},
 {C6, 88, 88},
 {C6, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Gb5, 88, 88},
 {Ab5, 88, 88},
 {C6, 88, 88},
 {Db5, 265, 265},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {C6, 265, 88},
 {Gb5, 265, 88},
 {F5, 265, 88},
 {Db5, 265, 88},
 {C6, 353, 176},
 {C6, 265, 88},
 {Db5, 88, 88},
 {Db5, 88, 88},
 {Db5, 88, 88},
 {E5, 265, 88},
 {E5, 265, 88},
 {Gb5, 265, 88},
 {Gb5, 265, 88},
 {Ab5, 265, 88},
 {Ab5, 265, 88},
 {C6, 88, 88},
 {C6, 88, 88},
 {C6, 88, 88},
 {C6, 88, 88},
 {C6, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Gb5, 88, 88},
 {Gb5, 88, 88},
 {Gb5, 88, 88},
 {Ab5, 88, 88},
 {C6, 88, 88},
 {Db5, 265, 88},
 {C6, 88, 88},
 {C6, 88, 88},
 {C6, 88, 88},
 {C6, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Gb5, 88, 88},
 {Gb5, 88, 88},
 {Gb5, 88, 88},
 {Ab5, 88, 88},
 {C6, 88, 88},
 {Db5, 265, 265},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {Ab5, 88, 88},
 {C6, 265, 88},
 {Gb5, 265, 88},
 {F5, 265, 88},
 {Db5, 265, 88},
 {C6, 353, 176},
 {C6, 265, 88},
 {Db5, 88, 88},
 {Db5, 88, 88},
 {Db5, 88, 88},
 {E5, 265, 88},
 {E5, 265, 88},
 {Gb5, 265, 88},
 {Gb5, 265, 88},
 {Ab5, 265, 88},
 {Ab5, 265, 88},
 {Ab5, 265, 88},
 {Gb5, 265, 88},
 {Gb5, 706, 0},
};

void playMidi(int pin, const int notes[][3], size_t len){
 for (int i = 0; i < len; i++) {
    tone(pin, notes[i][0]);
    delay(notes[i][1]);
    noTone(pin);
    delay(notes[i][2]);
  }
}
// Generated using https://github.com/ShivamJoker/MIDI-to-Arduino

// main.ino or main.cpp
void setup() {
  // put your setup code here, to run once:
  // play midi by passing pin no., midi, midi len
  playMidi(BUZZER_PIN, midi1, ARRAY_LEN(midi1));
}

void loop() {
  // put your main code here, to run repeatedly:
}
