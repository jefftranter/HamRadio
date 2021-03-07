#ifndef BEENHERE
#include "SigGen.h"
#endif
#include <Arduino.h>

/*****
  Purpose: Set Page Flag
  Parameter list: void
  Return value:  void
*****/
void setPageFlag(){
  switch (selectPageFlag) {
    case 0: //Main Menu
      t_x = 0, t_y = 0; // To store the touch coordinates
      pressed = tft.getTouch(&t_x, &t_y);   // Pressed will be set true is there is a valid touch on the screen
      setFreq_A_B();      //   in MainMenu.cpp
      selectMainMenu();   //   in MainMenu.cpp
      break;
    case 1: //Sweep
      t_x = 0, t_y = 0; // To store the touch coordinates
      pressed = tft.getTouch(&t_x, &t_y);   // Pressed will be set true is there is a valid touch on the screen
      setSweep();      // in Sweep.cpp
      sweepMenu();
      break;
    case 2: // FM
      t_x = 0, t_y = 0; // To store the touch coordinates
      pressed = tft.getTouch(&t_x, &t_y);   // Pressed will be set true is there is a valid touch on the screen
      modulationPage();
      setFreq_A_B();      //   in MainMenu.cpp
      break;
    case 3: //DDS2 Functions
    t_x = 0, t_y = 0; // To store the touch coordinates
      pressed = tft.getTouch(&t_x, &t_y);   // Pressed will be set true is there is a valid touch on the screen
      selectDDS2Function();
      setFreq_A_B();      //   in MainMenu.cpp
      break;
  }
}
/*****
  Purpose: Draw Main Menu Buttons
  Parameter list: void
  Return value:  void
*****/
void drawMainMenu() {
  //---------------Main Menu - init and draw buttons
  tft.setFreeFont(FS9);
  tft.fillRect(2, 145 , 258, 40, TFT_BLACK);  //erase main menu
  for (int b = 0; b <= 3; b++) {
    mainMenuButton[b].initButton(&tft, 33 + 65 * b , 165, 60, 33, TFT_WHITE,  TFT_NAVY, TFT_WHITE, mainMenuLabels[b], KEY_TEXTSIZE);
    mainMenuButton[b].drawButton();
  }
}

//--------------------------------  Draw Freq A & B buttons

/*****
  Purpose: Draw Freq A & B Buttons
  Parameter list: void
  Return value:  void
*****/
void drawFreqButtons() {
  tft.fillRect(2, 186 , 258, 130, TFT_BLACK);
 tft.setFreeFont(FS9);
  for (int b = 0; b < 2; b++) {
    freqButtons[b].initButton(&tft, 38, 205 + 40 * b, 65, 31, TFT_WHITE,  TFT_BLUE, TFT_WHITE, setFreqLabels[b], KEY_TEXTSIZE);
    freqButtons[b].drawButton();
  }
}

/*****
  Purpose: Select Menu options by button press
  Parameter list: void
  Return value:  void
*****/
void selectMainMenu() {
  for (uint8_t b5 = 0; b5 <= 3; b5++) {
    if (pressed && mainMenuButton[b5].contains(t_x, t_y)) {
      mainMenuButton[b5].press(true);  // tell the button it is pressed
    } else {
      mainMenuButton[b5].press(false);  // tell the button it is NOT pressed
    }
    tft.setFreeFont(FS9);
    if (mainMenuButton[b5].justReleased()) mainMenuButton[b5].drawButton();     // draw normal
    if (mainMenuButton[b5].justPressed()) {
      mainMenuButton[b5].drawButton(true);  // draw invert

      switch (b5) {
        case 0: // Main page
          selectPageFlag = 0;
          drawFreqButtons();
          break;
        case 1: //Sweep
          selectPageFlag = 1;
          tft.fillRect(2, 186 , 258, 130, TFT_BLACK);
          tft.fillRect(2, 145 , 258, 40, TFT_BLACK);
          tft.setTextFont(2);
          tft.setFreeFont(FS9);
          for (int b = 0; b < 4; b++) {
            sweepButtons[b].initButton(&tft, 38, 203 + 32 * b, 65, 28, TFT_WHITE,  TFT_BLUE, TFT_WHITE, sweepLabels[b], KEY_TEXTSIZE);
            sweepButtons[b].drawButton();
          }
          for (int b = 0; b < 3; b++) {
            sweepMenuButtons[b].initButton(&tft, 33 + 65 * b , 165, 60, 33, TFT_GREEN,  TFT_BLUE, TFT_WHITE, sweepMenuLabels[b], KEY_TEXTSIZE);
            sweepMenuButtons[b].drawButton();
          }
          break;
        case 2:  //FM Modulation Mode
          selectPageFlag = 2;
          tft.fillRect(2, 145 , 258, 40, TFT_BLACK);
          tft.setFreeFont(FS9);
          modulateButtons[0].initButton(&tft, 33  , 165, 60, 33, TFT_WHITE,  TFT_NAVY, TFT_WHITE, modulateLabels[0], KEY_TEXTSIZE);
            modulateButtons[0].drawButton();
          for (int b = 1; b < 4; b++) {
            modulateButtons[b].initButton(&tft, 33 + 65 * b , 165, 60, 33, TFT_WHITE,  TFT_BLUE, TFT_WHITE, modulateLabels[b], KEY_TEXTSIZE);
            modulateButtons[b].drawButton();
          }
          break;
        case 3:  //DDS2 Functions
          selectPageFlag = 3;
          tft.setFreeFont(FS9);
          for (int b = 0; b < 4; b++) {
            functionButtons[b].initButton(&tft, 33 + 65 * b , 290, 60, 33, TFT_WHITE,  TFT_NAVY, TFT_WHITE, functionLabels[b], KEY_TEXTSIZE);
            functionButtons[b].drawButton();
          }
          break;
      }
    }
  }
}
/*****
  Purpose: Press Freq A or FreqB to set output value to DDS or DDS2 from Keypad display
  Parameter list: void
  Return value:  void
*****/
void setFreq_A_B() {
  //============================  setFreq_A_B
  for (uint8_t b3 = 0; b3 < 4; b3++) {
    if (pressed && freqButtons[b3].contains(t_x, t_y)) {
      freqButtons[b3].press(true);  // tell the button it is pressed
    } else {
      freqButtons[b3].press(false);  // tell the button it is NOT pressed
    }
    tft.setFreeFont(FS9);
    if (freqButtons[b3].justReleased()) freqButtons[b3].drawButton();     // draw normal
    if (freqButtons[b3].justPressed()) {
      freqButtons[b3].drawButton(true);  // draw invert

      switch (b3) {
        case 0:
          freqOutA = freqOut2;
          tft.fillRect(79, 188, 175, 36, TFT_BLACK);
          tft.setFreeFont(FS18);
          tft.setTextColor(TFT_GREEN, TFT_BLACK);
          tft.setCursor(80, 218, 2);
          freqFormat((long)freqOutA);
          tft.print(final2);
          sendFrequency(freqOutA);
          break;

        case 1:
          freqOutB = freqOut2;
          tft.fillRect(79, 229, 175, 37, TFT_BLACK);
          tft.setFreeFont(FS18);
          tft.setTextColor(TFT_GREEN, TFT_BLACK);
          tft.setCursor(80, 258, 2);
          freqFormat((long)freqOutB);
          tft.print(final2);
          AD_DDS2.setFrequency(chan, freqOutB);
          break;
      }
    }
  }
}

/*****
  Purpose: to cause a delay in program execution
  Parameter list:
  unsigned long millisWait    // the number of millseconds to wait
  Return value:
  void
*****/
void MyDelay(unsigned long millisWait)
{
  unsigned long now = millis();

  while (millis() - now < millisWait)
    ;     // Twiddle thumbs...
}
