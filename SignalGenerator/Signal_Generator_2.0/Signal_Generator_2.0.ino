#ifndef BEENHERE
#include "SigGen.h"
#endif
// Button Instances
TFT_eSPI tft = TFT_eSPI();
TFT_eSPI_Button key[21];
TFT_eSPI_Button genFunction[3];
TFT_eSPI_Button attnButtons[4];
TFT_eSPI_Button freqButtons[5];
TFT_eSPI_Button enSelect[2];
TFT_eSPI_Button mainMenuButton[4];
TFT_eSPI_Button functionButtons[4];
TFT_eSPI_Button modulateButtons[4];
TFT_eSPI_Button fmFreqButtons[6];
TFT_eSPI_Button spanButtons[4];
TFT_eSPI_Button sweepButtons[3];
TFT_eSPI_Button sweepMenuButtons[3];
TFT_eSPI_Button helpButton;

float ltx = 0;    // Saved x coord of bottom of needle
uint16_t osx = M_SIZE * 120, osy = M_SIZE * 120; // Saved x & y coords
uint32_t updateTime = 0;       // time for next update
#define TFT_GREY 0x5AEB
#define DDS_CLOCK 180000000  //set DDS AD9851 clock

//DDS 9388 Instance and parameters
MD_AD9833  AD_DDS2(DATA2, CLK2, FSYNC); // Arbitrary SPI pins
MD_AD9833::channel_t chan;
MD_AD9833::mode_t mode;

int old_analog =  -999; // Value last displayed
int value[6] = {0, 0, 0, 0, 0, 0};
int old_value[6] = { -1, -1, -1, -1, -1, -1};
int d = 0;
const char *msg;
float freqOut;
float logAmpOut = 0;
float logAmpOutOld = 0;

// Rotary encoder instances
Rotary rotary1 = Rotary(ENCODER1PINA, ENCODER1PINB);
Rotary rotary2 = Rotary(ENCODER2PINA, ENCODER2PINB);

byte LOAD = 5;
byte CLOCK = 14;
byte DATA = 17;

char numberBuffer[NUM_LEN + 1] = "";
uint8_t numberIndex = 0;
boolean pressed;
uint16_t t_x = 0, t_y = 0;
long freqMult;
int placePos = 0;
char* hzLabel[] = {"   Hz", "KHz", "MHz"};
uint8_t  b1;
float displayFreqOut;
float displayFreqOut2;
unsigned char result1;
unsigned char result2;
unsigned char result3;
int en1State;
int en2State;
int en3State = 0;
long eCounter1 = 0;
int eCounter2 = 0;
int eCounter3 = 0;
int oldeCounter1 = 0;
long kCounter2 = 0;
int oldkCounter2 = 0;
int eCounter1Rev = 0;
int oldeCounter1Rev = 0;
volatile int encoder2;     // + is CW, - is CCW
long freqOut2;
int final_length;
char final2[12];
char buffer[12] = {0};
int freqMultExp;
int meterFlag1 = 0;
int encoderFlag2;
long freqOutA;
long freqOutB;
long freqOutAOld;
float sineWave2[32];
unsigned long freqFM = 1000000;
int FM_FlagOff = 0;
int fmDelay = 43;
int freqFlag = 0;
float voltsConv;
int selectPageFlag = 0;
int span = 5000;
int spanFlag = 0;
long freqSweepStart;
long freqSweepEnd;
int sweepFlag = 0;
int helpFlag = 0;
long sweepTime = 10;
int points = 100;
int en2ChangeFlag = 0;
int voltsConv2 = 0;

// Create various key labels
char keyLabel[][7] = {"Clr", "SetA", "SetB", "1", "2", "3", "4", "5", "6", "7", "8", "9", ".", "0", "1", "10", "100", "1K", "10K", "100K", "1M"};
char* functionLabel[] = {"Home", "Mod Setup", "Sweep Set"};
char* setButtonLabels[] = {"0", "-10", "-20", "-30"};
char* setFreqLabels[] = {"Freq A", "Freq B", "Start", "Stop", "Time"};
char *enSelectLabels[] = {"A", "B"};
char *mainMenuLabels[] = {"Main", "Sweep", "FM", "Funct"};
char *functionLabels[] = {"Sine", "Square1", "Square2", "Triangle"};
char *modulateLabels[] = {"Main", "FM on", "Freq", "Span"};
char *fmFreqLabels[] = {"400", "600 ", "800", "1000", "1200", "1400"};
char *spanLabels[] = {"2K", "3K", "5K", "10K", "12K"};
char *sweepLabels[] = {"Start", "End", "Time", "Points"};
char *sweepMenuLabels[] = {"Main", "Start", "End"};

uint16_t keyColor1[21] = {TFT_RED, TFT_DARKGREY, TFT_DARKGREEN,
                          TFT_BLUE, TFT_BLUE, TFT_BLUE,
                          TFT_BLUE, TFT_BLUE, TFT_BLUE,
                          TFT_BLUE, TFT_BLUE, TFT_BLUE,
                          TFT_BLUE, TFT_BLUE, TFT_GREEN,
                          TFT_GREEN, TFT_GREEN, TFT_GREEN,
                          TFT_GREEN, TFT_GREEN, TFT_GREEN
                         };
uint16_t keyColor2[21] = {TFT_WHITE, TFT_WHITE, TFT_WHITE,
                          TFT_WHITE, TFT_WHITE, TFT_WHITE,
                          TFT_WHITE, TFT_WHITE, TFT_WHITE,
                          TFT_WHITE, TFT_WHITE, TFT_WHITE,
                          TFT_WHITE, TFT_WHITE, TFT_BLACK,
                          TFT_BLACK, TFT_BLACK, TFT_BLACK,
                          TFT_BLACK, TFT_BLACK, TFT_BLACK
                         };
float DDS2FreqOut;
long voltRFOut;
unsigned long frequency;


//========================
/*****
  Purpose: Logon Page

  Argument list:
    void

  Return value:
    void
*****/
void Splash()
{
  tft.fillScreen(TFT_BLUE);
  tft.setFreeFont(FSB18);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setCursor(PIXELWIDTH / 2 - 160, PIXELHEIGHT / 4);
  tft.print("Signal Generator V07");
  //tft.setCursor(40, 45);
  // tft.print("for Amateur Radio");
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setCursor(PIXELWIDTH / 2 - 10 , PIXELHEIGHT / 4 + 60);
  tft.print("by");
  tft.setCursor(PIXELWIDTH / 2 - 170, PIXELHEIGHT / 2 + 100);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.print("Jack Purdum  W8TEE");
  tft.setCursor(PIXELWIDTH / 2 - 130, PIXELHEIGHT / 2 + 50);
  tft.println("Al Peter  AC8GY");
  // tft.setTextSize(2);

  MyDelay(SPLASHDELAY);
  tft.fillScreen(TFT_BLACK);
}

//===========================================
void setup()
{
  delay(1000);

  Serial.begin(115200);
  tft.begin();
  tft.init();  // Initialise the TFT screen


  tft.setRotation(3);  // Set the rotation before we calibrate
  delay(100);
  touch_calibrate();
  rotary1.begin();
  rotary2.begin();

  pinMode(0,   OUTPUT);
  pinMode(ENCODER1PINA,   INPUT_PULLUP);
  pinMode(ENCODER1PINB,   INPUT_PULLUP);
  pinMode(ENCODERSWITCH1,   INPUT_PULLUP);
  pinMode(ENCODER2PINA,   INPUT_PULLUP);
  pinMode(ENCODER2PINB,   INPUT_PULLUP);
  pinMode(35, INPUT);

  analogSetPinAttenuation(34, ADC_6db);
  analogSetPinAttenuation(35, ADC_6db);
  pinMode (27, OUTPUT); // sets pin 27 as OUTPUT

  pinMode (DATA, OUTPUT); // sets pin 10 as OUPUT
  pinMode (CLOCK, OUTPUT); // sets pin 9 as OUTPUT
  pinMode (LOAD, OUTPUT); // sets pin 8 as OUTPUT

  unsigned long tuning_word = (frequency * pow(2, 32)) / DDS_CLOCK;

  attachInterrupt(ENCODER1PINA, encoder1ISR, CHANGE);
  attachInterrupt(ENCODER1PINB, encoder1ISR, CHANGE);
  attachInterrupt(ENCODERSWITCH1, encoderSWISR, CHANGE);

  attachInterrupt(ENCODER2PINA, encoder2ISR, CHANGE);
  attachInterrupt(ENCODER2PINB, encoder2ISR, CHANGE);
Serial.println("In Setup");
  AD_DDS2.begin();  //AD9833 init
  chan = MD_AD9833::CHAN_0;
  AD_DDS2.setFrequency(chan, 5000);   //set_DDS2();
  sendFrequency(2000UL);
  Splash();
  updateTime = millis(); // Next update time
  drawHome();
  drawKeypad();   // Draw keypad
  logAmpOut = 0;
  analogMeter(); // Draw analogue meter
  mode = MD_AD9833::MODE_SINE;
  AD_DDS2.setMode(mode);
  meterFlag1 = 0;
  freqMult = 100;
  selectPageFlag = 0; //start with home (Main) page
  span = 10000;
  sendFrequency(2000000UL);
}

void loop()
{
  touch_calibrate();
  en2ChangeFlag = 0;
  setPageFlag();  //in file MainMenu
  selectMeterInput();
  selectDDS2Function();
  attnSelect();
  logAmpCalibrate();
  voltsConv2 = voltsConv * 1000;
  plotNeedle(voltsConv2, 10); //
  //Serial.print("voltsConv*100 loop"); Serial.println(voltsConv * 100);
  t_x = 0, t_y = 0; // To store the touch coordinates
  // Pressed will be set true is there is a valid touch on the screen
  pressed = tft.getTouch(&t_x, &t_y);
  //Serial.print(t_x); Serial.print("  "); Serial.println(t_y);
  drawHelp();
  readKeyPad();
  if (en2ChangeFlag == 1) {
    AD_DDS2.setFrequency(chan, freqOutA);
    en2ChangeFlag = 0;
  }
}
