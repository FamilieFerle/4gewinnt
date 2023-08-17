#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define LED_PIN 1
// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 42
#define ROW_COUNT 6
#define COL_COUNT 7

#define BUTTON_1 2
#define BUTTON_2 3
#define BUTTON_3 4
#define BUTTON_4 5
#define BUTTON_5 6
#define BUTTON_6 7
#define BUTTON_7 8

#define LED_R 9
#define LED_G 10
#define LED_B 11

// NeoPixel brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 50  // Set BRIGHTNESS to about 1/5 (max = 255)

int columns[7][6] = { { 35, 34, 21, 20, 7, 6 },
                      { 36, 33, 22, 19, 8, 5 },
                      { 37, 32, 23, 18, 9, 4 },
                      { 38, 31, 24, 17, 10, 3 },
                      { 39, 30, 25, 16, 11, 2 },
                      { 40, 29, 26, 15, 12, 1 },
                      { 41, 28, 27, 14, 13, 0 } };

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

bool isPlayerRed = true;
uint32_t red = strip.Color(255, 0, 0);
uint32_t yellow = strip.Color(255, 255, 0);
uint32_t background = strip.Color(20, 20, 20);

void setup() {
  strip.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();   // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);

  colorWipe(background, 0);  // All light grey
  setLEDPlayerRed();
}

void loop() {
  int buttonPressed = -1;
  if (isButtonPressed(buttonPressed)) {
    //Starte in oberer Zeile und lasse den Farbpunkt nach unten fallen
    int i = 0;
    while (i < ROW_COUNT) {
      int idx = columns[buttonPressed - 1][i];
      auto currPixCol = strip.getPixelColor(idx);
      if (currPixCol == background) {
        strip.setPixelColor(idx, isPlayerRed ? red : yellow);
        delay(200);
        i++;
      } else {
        break;
      }
    }
    checkForVictory();
    changePlayer();
  }
}

void checkForVictory() {
  //vertical
  for (int col = 0; col < COL_COUNT; col++) {
    int beforeCol = strip.getPixelColor(columns[col][0]);
    int counter = 1;
    for (int row = 1; row < ROW_COUNT; row++) {
      int curCol = strip.getPixelColor(columns[col][row]);
      if (curCol != background) {
        if (beforeCol == curCol) {
          counter++;
        } else {
          counter = 1;
        }
        beforeCol = curCol;
      }
      if (counter == 4) {
        gameWon();
      }
    }
  }
}

void gameWon() {
  while (true) {
    colorWipe(isPlayerRed ? red : yellow, 200);
    colorWipe(0, 200);
  }
}

void changePlayer() {
  isPlayerRed = !isPlayerRed;
  isPlayerRed ? setLEDPlayerRed() : setLEDPlayerYel();
}

bool isButtonPressed(int& f_buttonPressed) {
  if (digitalRead(BUTTON_1) == HIGH) {
    f_buttonPressed = 1;
    return true;
  }
  if (digitalRead(BUTTON_2) == HIGH) {
    f_buttonPressed = 2;
    return true;
  }
  if (digitalRead(BUTTON_3) == HIGH) {
    f_buttonPressed = 3;
    return true;
  }
  if (digitalRead(BUTTON_4) == HIGH) {
    f_buttonPressed = 4;
    return true;
  }
  if (digitalRead(BUTTON_5) == HIGH) {
    f_buttonPressed = 5;
    return true;
  }
  if (digitalRead(BUTTON_6) == HIGH) {
    f_buttonPressed = 6;
    return true;
  }
  if (digitalRead(BUTTON_7) == HIGH) {
    f_buttonPressed = 7;
    return true;
  }
  return false;
}

void setLEDPlayerRed() {
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, LOW);
}

void setLEDPlayerYel() {
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_B, LOW);
}

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < strip.numPixels(); i++) {  // For each pixel in strip...
    strip.setPixelColor(i, color);               //  Set pixel's color (in RAM)
    strip.show();                                //  Update strip to match
    delay(wait);                                 //  Pause for a moment
  }
}
