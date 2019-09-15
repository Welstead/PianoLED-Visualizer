#include <bitswap.h>
#include <chipsets.h>
#include <color.h>
#include <colorpalettes.h>
#include <colorutils.h>
#include <controller.h>
#include <cpp_compat.h>
#include <dmx.h>
#include <FastLED.h>
#include <fastled_config.h>
#include <fastled_delay.h>
#include <fastled_progmem.h>
#include <fastpin.h>
#include <fastspi.h>
#include <fastspi_bitbang.h>
#include <fastspi_dma.h>
#include <fastspi_nop.h>
#include <fastspi_ref.h>
#include <fastspi_types.h>
#include <hsv2rgb.h>
#include <led_sysdefs.h>
#include <lib8tion.h>
#include <noise.h>
#include <pixelset.h>
#include <pixeltypes.h>
#include <platforms.h>
#include <power_mgt.h>

#include <LinkedList.h>

#include <MIDI.h>

#include <Adafruit_NeoPixel.h>
#define LED 13
#ifdef __AVR__
#include <avr/power.h>
#endif

#define BRIGHTNESS 15
#define PIN 6
byte curWheelPos = 0;
int pressedNotes[10];
int index = -1;
static const bool HandleNullVelocityNoteOnAsNoteOff = true;
MIDI_CREATE_DEFAULT_INSTANCE();

LinkedList<int> noteList = LinkedList<int>();

Adafruit_NeoPixel strip = Adafruit_NeoPixel(144, PIN, NEO_GRB + NEO_KHZ800);
CRGB leds[144];
int lastNote = 0;

int currentNote = 0;
bool notePressed;
uint32_t green = strip.Color(0, 255, 0); //Colour declarations
uint32_t red = strip.Color(255, 0, 0);
uint32_t blue = strip.Color(0, 0, 255);
uint32_t purple = strip.Color(255, 0, 255);
uint32_t orange = strip.Color(255, 69, 0);
uint32_t color;
int keysPressed = 0;
bool doubleNote;
int doubleNotes;

void setup() {

  Serial.begin(31250);
  while (!Serial);
  pinMode (LED, OUTPUT);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.turnThruOff();
  MIDI.setHandleNoteOn(KeyOn);
  MIDI.setHandleNoteOff(KeyOff);
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  color = blue;
  //showNextNote();

  strip.show();

  
}
void loop() {

  MIDI.read();
  
}

void KeyOff(byte inChannel, byte inNote, byte inVelocity) {//What happens whenever a key is released on the piano
  //inNote is the note being pressed
  
  doubleNotes = 0;
  keysPressed--;
  
  strip.setPixelColor(noteToStripIndex(inNote), 0);
  strip.setPixelColor(noteToStripIndex(inNote)+1, 0);
  strip.show();
  Serial.print(keysPressed);
  //Serial.print(inNote);
  //Serial.print("off");
  //Serial.print("   ");
  
}
void KeyOn(byte inChannel, byte inNote, byte inVelocity) {
  
  
  if (inVelocity == 144){
    return;
  }
  if (inNote <33 || inNote>105){
    keysPressed++;
    return;
  }
  keysPressed++;
  strip.setPixelColor(noteToStripIndex(inNote), color);
  strip.setPixelColor(noteToStripIndex(inNote)+1, color);
  strip.show();
  Serial.print(keysPressed);
  //Serial.print(inNote);
  //Serial.print("on");
  //Serial.print("  ");
  
}

void clearStrip()
{
  for (int i = 0; i < 144; i++) {
    strip.setPixelColor(i, 0);

  }
  strip.show();
}

int noteToStripIndex(byte inNote){
  byte index;
  
  index = (inNote-33) * 2;
  if (inNote>=69){
    index--;
    return index;
  }
  return index;
}
