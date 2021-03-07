#ifndef BEENHERE
#include "SigGen.h"
#endif
#include <Arduino.h>

/*****
  Purpose: Start Freq A sweep
  Parameter list: void
  Return value:  void
*****/
void runSweep() {
  while (sweepFlag == 1 ) {
    for (int i = 0; i <= points; i++) {
      sendFrequency(freqSweepStart + ((freqSweepEnd - freqSweepStart) * i) / 100);
      delay(sweepTime * 1000 / points);

      t_x = 0, t_y = 0; // To store the touch coordinates
      pressed = tft.getTouch(&t_x, &t_y);   // Pressed will be set true is there is a valid touch on the screen

      if (pressed && sweepMenuButtons[2].contains(t_x, t_y)) {
        sweepMenuButtons[2].press(true);  // tell the button it is pressed
        sweepFlag = 0;
        i=points;
      } else {
        sweepMenuButtons[2].press(false);  // tell the button it is NOT pressed
      }
      tft.setFreeFont(FS9);
      if (sweepMenuButtons[2].justReleased()) sweepMenuButtons[2].drawButton();     // draw normal
      if (sweepMenuButtons[2].justPressed()) {
        sweepMenuButtons[2].drawButton(true);  // draw invert
      }
    }
  }
}

/*****
  Purpose: Set Sweep Parameters from keypad input
  Parameter list: void
  Return value:  void
*****/
void setSweep() {   //============================  setFreq_A_B

  tft.setCursor(170, 309, 2);
  tft.setFreeFont(FS12);
  tft.setTextColor(TFT_GREEN, TFT_YELLOW);
  tft.print("Sweep A");
  t_x = 0, t_y = 0; // To store the touch coordinates
  pressed = tft.getTouch(&t_x, &t_y);   // Pressed will be set true is there is a valid touch on the screen
  for (uint8_t b10 = 0; b10 < 4; b10++) {
    if (pressed && sweepButtons[b10].contains(t_x, t_y)) {
      sweepButtons[b10].press(true);  // tell the button it is pressed
    } else {
      sweepButtons[b10].press(false);  // tell the button it is NOT pressed
    }
 tft.setFreeFont(FS9);
    if (sweepButtons[b10].justReleased()) sweepButtons[b10].drawButton();     // draw normal
    if (sweepButtons[b10].justPressed()) {
      tft.setFreeFont(FS9);
      sweepButtons[b10].drawButton(true);  // draw invert
      //tft.setFreeFont(FS12);
      switch (b10) {
        case 0:
          freqSweepStart = displayFreqOut * freqMult;;
          tft.fillRect(79, 190 , 158, 30, TFT_BLACK);
          tft.setFreeFont(FS12);
          tft.setTextColor(TFT_GREEN, TFT_BLACK);
          tft.setCursor(80, 213, 2);
          freqFormat((long)freqSweepStart);
          tft.print(final2);
          sendFrequency(freqSweepStart);
          tft.setFreeFont(FS9);
          break;

        case 1:
          freqSweepEnd = displayFreqOut * freqMult;;
          tft.fillRect(79, 222, 158, 30, TFT_BLACK);
          tft.setFreeFont(FS12);
          tft.setTextColor(TFT_GREEN, TFT_BLACK);
          tft.setCursor(80, 245, 2);
          freqFormat((long)freqSweepEnd);
          tft.print(final2);
          break;

        case 2:  //Time sec
          sweepTime = displayFreqOut;
          if (sweepTime <= 0) sweepTime = 0;
          if (sweepTime >= 10000) sweepTime = 10000;
          tft.fillRect(79, 254, 158, 30, TFT_BLACK);
          tft.setTextColor(TFT_GREEN, TFT_BLACK);
          tft.setFreeFont(FS12);
          tft.setCursor(150, 277, 2);
          tft.print("sec.");
          tft.setCursor(80, 277, 2);
          freqFormat((long)sweepTime);
          tft.print(final2);
          break;

        case 3:  //Points
          points = displayFreqOut;
          if (points <= 0) points = 0;
          if (points >= 1000) points = 1000;
          tft.fillRect(79, 286, 158, 30, TFT_BLACK);
          tft.setTextColor(TFT_GREEN, TFT_BLACK);
          tft.setFreeFont(FS12);
          tft.setCursor(80, 309, 2);
          freqFormat((long)points);
          tft.print(final2);
          break;
      }
    }
  }
}

/*****
  Purpose: Draw Sweep Menu buttons
  Parameter list: void
  Return value:  void
*****/
void sweepMenu() {
  t_x = 0, t_y = 0; // To store the touch coordinates
  pressed = tft.getTouch(&t_x, &t_y);   // Pressed will be set true is there is a valid touch on the screen
  for (uint8_t b5 = 0; b5 <= 3; b5++) {
    if (pressed && sweepMenuButtons[b5].contains(t_x, t_y)) {
      sweepMenuButtons[b5].press(true);  // tell the button it is pressed
    } else {
      sweepMenuButtons[b5].press(false);  // tell the button it is NOT pressed
    }
    tft.setFreeFont(FS9);
    if (sweepMenuButtons[b5].justReleased()) sweepMenuButtons[b5].drawButton();     // draw normal
    if (sweepMenuButtons[b5].justPressed()) {
      sweepMenuButtons[b5].drawButton(true);  // draw invert

      switch (b5) {
        case 0: // Main page
          selectPageFlag = 0;
          sweepFlag = 0;
          drawFreqButtons();
          drawMainMenu();
          break;
        case 1: // Start Sweep
          selectPageFlag = 1;
          sweepFlag = 1;
          runSweep();
          break;
        case 2:  //End Sweep
          sweepFlag = 0;
          break;
      }
    }
  }
}
