#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6
#define WAIT 20

Adafruit_NeoPixel strip = Adafruit_NeoPixel(57, PIN, NEO_GRB + NEO_KHZ800);


/*********************
   Animation Utils
*********************/

int brightnessModifier = 1;

int getColor(){
  int base_color = brightnessModifier * 1;
  return base_color;
}

void clearStrip(){
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
}

/*********************
      Animations
*********************/

void blankStrip(){
  clearStrip();
  strip.show();
  delay(WAIT);
}


void whiteLight(){
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    int c = getColor();
    strip.setPixelColor(i, strip.Color(c, c, c));
  }
  strip.show();
}


void redPingPong(){  
  int c = getColor();
  
  for(uint16_t i=0; i<=strip.numPixels()-1; ++i) {
    clearStrip();
    strip.setPixelColor(i, strip.Color(c, 0, 0));
    delay(WAIT*2);
    strip.show();
  }

  for(uint16_t i=strip.numPixels()-1; i>0; i--) {
    clearStrip();
    strip.setPixelColor(i, strip.Color(c, 0, 0));
    delay(WAIT*2);
    strip.show();
  }
  
}


void randomColors() {  
  int randRed = random(255);
  int randGreen = random(255);
  int randBlue = random(255);
  
  for (uint16_t i=0; i<strip.numPixels(); i++) {
    if (i%10 == 0) {
      strip.setPixelColor(i, strip.Color(0, 0, randBlue));
    } else if (i%5 == 0) {
      strip.setPixelColor(i, strip.Color(0, randGreen, 0));
    } else if (i%3 == 0) {
      strip.setPixelColor(i, strip.Color(randRed, 0, 0));
    } else {
      strip.setPixelColor(i, strip.Color(randRed, randGreen, randBlue));  
    }
  }

  strip.show();
  delay(WAIT);
}


/*********************
 * State management
 *********************/

// Event flags
bool justBooted = true;
bool playPingPong = false;


void resetBlink(){
  if (!justBooted ) {
    return;
  }
  justBooted = false;
  randomColors();
}


void serialEvent(){
  Serial.println("serialEvent");
  
  if (Serial.available() > 0) {   
    String cmd = Serial.readStringUntil(':');
    int val = Serial.readStringUntil('\n').toInt();
    
    Serial.print("Command is: ");
    Serial.println(cmd);
    
    Serial.print("Value is: ");
    Serial.println(val);

    if (cmd == "WHITELIGHT") {
      brightnessModifier = val;
      playPingPong = false;
    }
    if (cmd == "PINGPONG") {
      brightnessModifier = val;
      playPingPong = true;
    }
  }
}


void setup() {
  Serial.begin(9600);
  
  // On subsequent executions of a sketch random sequence should differ
  randomSeed(analogRead(0));

  strip.begin();
  strip.show();
}


void loop() {
  resetBlink();

  if(playPingPong){
    redPingPong();
  } else {
    whiteLight();  
  }
  
}

