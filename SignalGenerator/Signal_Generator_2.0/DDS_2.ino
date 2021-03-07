#ifndef BEENHERE
#include "SigGen.h"
#endif
#include <Arduino.h>
/*****
   Purpose: Set Frequency for AD9833 DDS
  Paramter list: void
  Return value:  void
*****/
void set_DDS2() {
  DDS2FreqOut = atof(numberBuffer);
  //Serial.print("in setDDS2  ");
  //Serial.print(DDS2FreqOut);
  AD_DDS2.setFrequency(chan, DDS2FreqOut);
}

/*****
   Purpose: Select Function Mode for AD9833 DDS
  Paramter list: void
  Return value:  void
*****/
void selectDDS2Function() { //===== DDS Function Selection =======
  t_x = 0, t_y = 0; // To store the touch coordinates
  pressed = tft.getTouch(&t_x, &t_y);   // Pressed will be set true is there is a valid touch on the screen
  for (uint8_t b6 = 0; b6 < 4; b6++) {
    if (pressed && functionButtons[b6].contains(t_x, t_y)) {
      functionButtons[b6].press(true);  // tell the button it is pressed
    } else {
      functionButtons[b6].press(false);  // tell the button it is NOT pressed
    }
    tft.setFreeFont(FS9);
    if (functionButtons[b6].justReleased()) functionButtons[b6].drawButton();     // draw normal
    if (functionButtons[b6].justPressed()) {
      functionButtons[b6].drawButton(true);  // draw invert
      switch (b6) {
        case 0:
          mode = MD_AD9833::MODE_SINE;
          AD_DDS2.setMode(mode);
          drawMainMenu();
          selectPageFlag = 0;
          break;
        case 1:
          mode = MD_AD9833::MODE_SQUARE1;
          AD_DDS2.setMode(mode);
          drawMainMenu();
          selectPageFlag = 0;
          break;
        case 2:
          mode = MD_AD9833::MODE_SQUARE2;
          AD_DDS2.setMode(mode);
          drawMainMenu();
          selectPageFlag = 0;
          break;
        case 3:
          mode = MD_AD9833::MODE_TRIANGLE;
          AD_DDS2.setMode(mode);
          drawMainMenu();
          break;
      }
    }
  }
}
