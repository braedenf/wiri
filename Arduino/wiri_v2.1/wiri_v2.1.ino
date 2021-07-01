
// ---------------------------AUDIO SETUP---------------------------

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>


// GUItool: begin automatically generated code
AudioSynthWaveformModulated waveformMod1;   //xy=366,366
AudioEffectEnvelope      envelope1;      //xy=578,325
AudioOutputI2S           i2s1;           //xy=819,380
AudioConnection          patchCord1(waveformMod1, envelope1);
AudioConnection          patchCord2(envelope1, 0, i2s1, 0);
AudioConnection          patchCord3(envelope1, 0, i2s1, 1);
AudioControlSGTL5000     audioShield;     //xy=353,534
// GUItool: end automatically generated code



// ---------------------------FAST LED---------------------------

#include <WS2812Serial.h>
#define USE_WS2812SERIAL
#include <FastLED.h>

// ---------------------------STRIP SETUP---------------------------

#define STRIP_COUNT 960
#define STRIP_1_PIN 17

#define STRIP_1_START 0
#define STRIP_1_END 239

#define STRIP_2_START 240
#define STRIP_2_END 479

#define STRIP_3_START 480
#define STRIP_3_END 719

#define STRIP_4_START 720
#define STRIP_4_END 959

CRGBArray<STRIP_COUNT> leds;

// ---------------------------LIGHT/SOUND PROPS---------------------------

bool noteIsOn;

uint8_t sinWave;
uint8_t sawWave;
uint8_t squareWave;
uint8_t triangleWave;

float sinFreq = 0.2;
uint8_t sawFreq = 30;
float squareFreq = 0.2;
float triFreq = 0.2;


//---------------------------SERIAL---------------------------
bool startReceiving;
const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars]; // temporary array for use when parsing

//Parsed data (received as -> <toggle, mouseX, mouseY, waveType>)

bool toggle = 0;
int mouseX = 0;
int mouseY = 0;
int waveType = 0;

int current_waveform = 0; //Stores the type of waveform (sine, sawtooth, square, triangle)
float waveVol = 0.0;
int waveFrequency = 220;

bool newData = false;

//---------------------------SETUP---------------------------
void setup() {
  delay(3000); // sanity delay

  // Set up audio shield
  AudioMemory(12);
  audioShield.enable();
  audioShield.volume(0.8);


  // Open Serial communication
  Serial.begin(115200);

  /* Use Fastled with serial communication, this is necessary
     with the Teensy 4.0 to meet the timing requirements of the WS2812B LED strip
  */  
  FastLED.addLeds<WS2812SERIAL, STRIP_1_PIN, BRG>(leds, leds.size()).setCorrection( TypicalLEDStrip );

  //Initialise waveform
  noteIsOn = true;
  current_waveform = WAVEFORM_TRIANGLE_VARIABLE;
  waveformMod1.begin(current_waveform);
  waveformMod1.amplitude(1.0);

  startReceiving = true; //Start receiving serial data
}

//---------------------------LOOP---------------------------
void loop() {

  //LIGHT PATTERNS - Setup Waveforms

  /*
   * Map a waveform over the brightness value of each set of LED's
   * The waveform is modulated over time, where the time is multiplied
   * by the frequency variables.
   */
  
  sinWave = map(sin8(millis() * sinFreq), 0, 255, 150, 255);
  sawWave = beat8(sawFreq, 0);
  squareWave= map(squarewave8(millis() * squareFreq), 0, 255, 150, 255);
  triangleWave = map(triwave8(millis() * triFreq), 0, 255, 150, 255);

  //START SERIAL DATA
  /*
   * Receive serial data when avaliable and then parse the data into variables
   * that control the frequency and amplitude of the sound wave, as well as
   * the modulation rate of the lights.
   */

  if(startReceiving) {
    Serial.write(1);
  }
  
  recvWithStartEndMarkers();
    if (newData == true) {
        startReceiving = false; //stop receiving until serial data has been processed
        strcpy(tempChars, receivedChars); // this temporary copy is necessary to protect the original data
        parseData(); //Parse the data into the variables given the correct syntax
        //showParsedData(); //Comment in production only for debug
        newData = false; //Only parse data when new data is avaliable
        
        /*
         * Toggle the volume on and off only when the screem is pressed in openframeworks application
         */
        if(toggle) {
          if(noteIsOn) {
            envelope1.noteOn();
            noteIsOn = false; //Turn off to avoid turning on every loop cycle
          }
          
          waveVol = mapfloat(mouseY, 0, 1080, 0.2, 0.6); // Map the Y coordinate to the amplitude of waveform
          waveFrequency = map(mouseX, 0, 1920, 160, 520); // Map the X coordinate to frequency of the waveform
        } else {
           envelope1.noteOff();
           noteIsOn = true; //ready for when screen is touched again
        }

        /*
         * Based on the wavetype declared in the serial data in range -> [0..3]
         * switch between the waveform and map the X coordinate of mouse to the modulation
         * rate of each pillar
         */
        
        switch (waveType) {
          case 0 :
            current_waveform = WAVEFORM_SINE;
            sinFreq = map(mouseX, 0, 1920, 0.2, 0.6);
            break;
          case 1 :

            current_waveform = WAVEFORM_SAWTOOTH;
            sawWave = map(mouseX, 0, 1920, 0.2, 0.6);
            break;
          case 2 :
            current_waveform = WAVEFORM_SQUARE;
            squareWave = map(mouseX, 0, 1920, 0.2, 0.6);
            break;
          case 3 :
            current_waveform = WAVEFORM_TRIANGLE;
            triangleWave = map(mouseX, 0, 1920, 0.2, 0.6);
            break;
          default:
            Serial.println("Invalid number: Choose a number from 0 to 3");
            current_waveform = 0;
        }
        
        waveformMod1.begin(current_waveform); //Restart Waveform on Wave Change

        /*
         * Set the amplitude and frequency for the
         * waveform to be equal to incomming serial data
         */
        waveformMod1.amplitude(waveVol);
        waveformMod1.frequency(waveFrequency);

        startReceiving = true; // Only start receiving new data once current data has been processed
        
    } // END SERIAL
  
  /*
   * Loop over each light pillar and assign the colour and modulate the brightness
   */

  //SINE WAVE

  for(int i = STRIP_1_START; i < STRIP_1_END; i++) { 
    leds[i] = CHSV(236, 160, sinWave);
  }

  //SAWTOOTH WAVE
  
  for(int i = STRIP_2_START; i < STRIP_2_END; i++) { 
    leds[i] = CHSV(192, 180, sawWave);
  }

  //SQUARE WAVE
  
  for(int i = STRIP_3_START; i < STRIP_3_END; i++) { 
      leds[i] = CHSV(136, 180, squareWave);
  }

  //TRIANGLE WAVE
  
  for(int i = STRIP_4_START; i < STRIP_4_END; i++) { 
    leds[i] = CHSV(12, 180, triangleWave);
  }

  // Display the LEDS last
  FastLED.show();
  
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
