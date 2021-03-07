#define TOUCH_CS

#include "Free_Fonts.h" // Include the header file attached to this sketch
#include "FS.h"
#include <TFT_eSPI.h> // https://github.com/Bodmer/TFT_eSPI  Hardware-specific library
#include <SPI.h>
#include <Wire.h>                 // Standard with IDE
#include <Rotary.h>               // https://github.com/brianlow/Rotary

#include <MD_AD9833.h>   //github.com/MajicDesigns/MD_AD9833

#define DDS_CLOCK 180000000 // DDS
extern byte LOAD; 
extern byte CLOCK; 
extern byte DATA;

#define DATA2  17  ///< SPI Data pin number AD9833
#define CLK2   14  ///< SPI Clock pin number
#define FSYNC 16  ///< SPI Load pin number (FSYNC in AD9833 usage)
#define KEY_X 35 // Centre of key Keypad start position, key sizes and spacing
#define KEY_Y 85
#define KEY_W 55 // Width and height
#define KEY_H 30
#define KEY_SPACING_X 10 // X and Y gap
#define KEY_SPACING_Y 6
#define KEY_TEXTSIZE 1   // Font size multiplier
#define LABEL1_FONT &FreeSansOblique12pt7b // Key label font 1 Using two fonts since numbers are nice when bold
#define LABEL2_FONT &FreeSansBold12pt7b    // Key label font 2
#define DISP_X 1// Numeric display box size and location
#define DISP_Y 10
#define DISP_W 198
#define DISP_H 50
#define DISP_TSIZE 3
#define DISP_TCOLOR TFT_CYAN
#define CALIBRATION_FILE "/TouchCalData2"
#define REPEAT_CAL false
#define M_SIZE .9
#define KEYPADXOFFSET 270
#define KEYPADYOFFSET -5
#define YOFFSETMETER  30
#define ENCODERSWITCH1    22     //Encoder1
#define ENCODER1PINA      12     //Encoder1
#define ENCODER1PINB      13     //Encoder1
#define ENCODER2PINA      33     //Encoder2
#define ENCODER2PINB      32     //Encoder2
#define ROTARYDELAY        300      // Milliseconds for display delay
#define NUM_LEN 8 // Number length, buffer for storing it and character index
#define TFT_GREY 0x5AEB
#define SPLASHDELAY       500
#define PIXELWIDTH        480
#define PIXELHEIGHT       320
extern Rotary rotary1;
extern Rotary rotary2;
extern TFT_eSPI tft;
extern TFT_eSPI_Button key[21];
extern TFT_eSPI_Button genFunction[3];
extern TFT_eSPI_Button attnButtons[4];
extern TFT_eSPI_Button freqButtons[5];
extern TFT_eSPI_Button enSelect[2];
extern TFT_eSPI_Button mainMenuButton[4];
extern TFT_eSPI_Button functionButtons[4];
extern TFT_eSPI_Button modulateButtons[4];
extern TFT_eSPI_Button fmFreqButtons[6];
extern TFT_eSPI_Button spanButtons[4];
extern TFT_eSPI_Button sweepButtons[3];
extern TFT_eSPI_Button sweepMenuButtons[3];
extern MD_AD9833  AD_DDS2;
extern MD_AD9833::channel_t chan;
extern MD_AD9833::mode_t mode;
extern TFT_eSPI_Button helpButton;

extern char *enSelectLabels[];
extern char *fmFreqLabels[];
extern char *functionLabel[];
extern char *functionLabels[];
extern char *mainMenuLabels[];
extern char *modulateLabels[];
extern char *setFreqLabels[];
extern char *spanLabels[];
extern char *sweepLabels[];
extern char *sweepMenuLabels[];
extern char buffer[12];
extern char keyLabel[21][7];
extern char* hzLabel[];
extern char* setButtonLabels[];
extern uint16_t keyColor1[21];
extern uint16_t keyColor2[21];

//--------------

extern     uint8_t b1;
extern   boolean pressed;
extern   int placePos;
extern   long freqMult;
extern   uint16_t t_x;
extern   uint16_t t_y;
extern char final2[];
extern char numberBuffer[9];
extern const char *msg;
extern float DDS2FreqOut;
extern float displayFreqOut;
extern float displayFreqOut2;
extern float freqOut;
extern float logAmpOut;
extern float logAmpOutOld;
extern float ltx;    // Saved x coord of bottom of needle
extern float sineWave2[];
extern float voltsConv;
extern int d;
extern int eCounter1Rev;
extern int eCounter2;
extern int eCounter3;
extern int en1State;
extern int en2State;
extern int en3State;
extern int meterFlag1;
extern int encoderFlag2;
extern int final_length;
extern int FM_FlagOff;
extern int fmDelay;
extern int freqFlag;
extern int freqMultExp;
extern int old_analog ; // Value last displayed
extern int old_value[6];
extern int oldeCounter1;
extern int oldeCounter1Rev;
extern int oldkCounter2;
extern int points;
extern int selectPageFlag;
extern int span;
extern int spanFlag;
extern int sweepFlag;
extern int value[6];
extern int yOffset;
extern long eCounter1;
extern long freqOut2;
extern long freqOutA;
extern long freqOutB;
extern long freqSweepEnd;
extern long freqSweepStart;
extern long kCounter2;
extern long sweepTime;
extern long voltRFOut;
extern uint16_t osx, osy; // Saved x & y coords
extern uint32_t updateTime;       // time for next update
extern uint8_t numberIndex;
extern unsigned char result1;
extern unsigned char result2;
extern unsigned char result3;
extern unsigned long freqFM;
extern volatile int encoder2;     // + is CW, - is CCW
extern int en2ChangeFlag;
extern int voltsConv2;
extern int helpFlag;



//----------------
char freqFormat(long freqToFormat);
int encoderRead1();
int encoderRead3();
void analogMeter();
void attnSelect();
void byte_out(unsigned char byte);
void ClrFreqDisplay();
void displayKbdOut();
void drawFreqButtons();
void drawHome();
void drawHelp();
void drawKeypad();
void drawMainMenu();
void encoder1ISR();
void encoder2ISR();
void encoder3ISR();
void encoderSWISR();
void logAmpCalibrate();
void modulationPage();
void MyDelay(unsigned long millisWait);
void outOne();
void outZero();
void plotNeedle(int value1, byte ms_delay);
void readEncoders();
void readKeyPad();
void rotate2();
void runSweep();
void selectDDS2Function();
void selectMeterInput();
void selectMainMenu();
void sendFrequency(unsigned long frequency);
void set_DDS2();
void setFreq_A_B();
void setFreqOut();
void setOutputFreq();
void setPageFlag();
void setSweep();
void sweepMenu();
void touch_calibrate();
void set_DDS2_EN();
