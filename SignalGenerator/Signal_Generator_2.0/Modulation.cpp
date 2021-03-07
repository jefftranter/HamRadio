#ifndef BEENHERE
#include "SigGen.h"
#endif
#include <Arduino.h>

/*****
  Purpose: Select Modulation page options
      Return to Min
      Turn on FM Modulation
      Select optional Digi Mode
      Set FM modulation Freq Hz
      Set FM deviation Hz
  Paramter list: void
  Return value:  void
*****/
void modulationPage() {
  spanFlag = 0;
  for (uint8_t b7 = 0; b7 < 4; b7++) {
    t_x = 0, t_y = 0; // To store the touch coordinates
    pressed = tft.getTouch(&t_x, &t_y);   // Pressed will be set true is there is a valid touch on the screen
    if (pressed && modulateButtons[b7].contains(t_x, t_y)) {
      modulateButtons[b7].press(true);  // tell the button it is pressed
    } else {
      modulateButtons[b7].press(false);  // tell the button it is NOT pressed
    }
    tft.setFreeFont(FS9);
    if (modulateButtons[b7].justReleased()) modulateButtons[b7].drawButton();     // draw normal
    if (modulateButtons[b7].justPressed()) {
      modulateButtons[b7].drawButton(true);  // draw invert
      FM_FlagOff = 0;
      switch (b7) {
        case 0: // Main Menu
          drawMainMenu();
          selectPageFlag = 0;
          selectMainMenu();
          break;
        case 1: //Start FM
          tft.setFreeFont(FSB9);
          tft.setCursor(80, 278, 2);
          tft.setTextColor(TFT_YELLOW, TFT_BLACK);
          tft.print("Coarse SW - Exit FM");
          for (int j = 0; j < 32; j++) {
            float sinArgument = (6.28 / 32) * j;
            sineWave2[j] = sin(sinArgument);
          }
          //Serial.print("span=  "); Serial.println(span);
          //Serial.print("fmDelay=  "); Serial.println(fmDelay);
          while (FM_FlagOff != 1) {
            //Serial.print("FM_FlagOff= "); Serial.println(FM_FlagOff);
            for (int i = 0; i <= 31; i++) {
              freqFM = freqOutA + span * (sineWave2[i]);
              sendFrequency(freqFM);
              delayMicroseconds(fmDelay);
            }
              if (FM_FlagOff == 1)
              {
              sendFrequency(freqOutA);
                tft.fillRect(98, 238 , 150, 20, TFT_BLACK);
                tft.setFreeFont(FS9);
                for (uint8_t b7 = 0; b7 < 4; b7++) {
                  modulateButtons[b7].drawButton();
                }
                FM_FlagOff = 0;
                 //modulationPage();
                break;
              }
          }
          //FM_FlagOff=0;
          break;

        case 2: // frequency
          freqFlag = 0;
          tft.fillRect(2, 280 , 258, 40, TFT_BLACK);
          for (int b = 0; b < 6; b++) {
            fmFreqButtons[b].initButton(&tft, 22 + 43 * b , 300, 39, 33, TFT_WHITE,  TFT_NAVY, TFT_WHITE, fmFreqLabels[b], KEY_TEXTSIZE);
            fmFreqButtons[b].drawButton();
          }
          t_x = 0, t_y = 0; // To store the touch coordinates
          pressed = tft.getTouch(&t_x, &t_y);   // Pressed will be set true is there is a valid touch on the screen
          while (freqFlag != 1) {
            for (uint8_t b8 = 0; b8 < 6; b8++) {
              pressed = tft.getTouch(&t_x, &t_y);
              if (pressed && fmFreqButtons[b8].contains(t_x, t_y)) {
                fmFreqButtons[b8].press(true);  // tell the button it is pressed
              } else {
                fmFreqButtons[b8].press(false);  // tell the button it is NOT pressed
              }
              tft.setFreeFont(FS9);
              if (fmFreqButtons[b8].justReleased()) fmFreqButtons[b8].drawButton();     // draw normal
              if (fmFreqButtons[b8].justPressed()) {
                fmFreqButtons[b8].drawButton(true);  // draw invert
                //FM_FlagOff = 0;
                switch (b8) {
                  case 0:
                    fmDelay = 70;
                    break;
                  case 1:
                    fmDelay = 43;
                    break;
                  case 2:
                    fmDelay = 24;
                    break;
                  case 3:
                    fmDelay = 13;
                    break;
                  case 4:
                    fmDelay = 6;
                    break;
                  case 5:
                    fmDelay = 1;
                    break;
                }
                freqFlag = 1;
                pressed = 0;
              }
            }
          }
          selectPageFlag = 2;
          break;
        case 3:  //span
          spanFlag = 0;
          tft.fillRect(2, 280 , 258, 40, TFT_BLACK);
          for (int b = 0; b < 5; b++) {
            spanButtons[b].initButton(&tft, 22 + 43 * b , 300, 39, 33, TFT_WHITE,  TFT_NAVY, TFT_WHITE, spanLabels[b], KEY_TEXTSIZE);
            spanButtons[b].drawButton();
          }
          t_x = 0, t_y = 0; // To store the touch coordinates
          pressed = tft.getTouch(&t_x, &t_y);   // Pressed will be set true is there is a valid touch on the screen
          while (spanFlag != 1) {
            //Serial.print("spanFlag=  "); Serial.println(spanFlag);
            for (uint8_t b9 = 0; b9 < 6; b9++) {
              pressed = tft.getTouch(&t_x, &t_y);
              if (pressed && spanButtons[b9].contains(t_x, t_y)) {
                spanButtons[b9].press(true);  // tell the button it is pressed
              } else {
                spanButtons[b9].press(false);  // tell the button it is NOT pressed
              }
              tft.setFreeFont(FS9);
              if (spanButtons[b9].justReleased()) spanButtons[b9].drawButton();     // draw normal
              if (spanButtons[b9].justPressed()) {
                spanButtons[b9].drawButton(true);  // draw invert

                switch (b9) {
                  case 0:
                    span = 2000;
                    break;
                  case 1:
                    span = 3000;
                    break;
                  case 2:
                    span = 5000;
                    break;
                  case 3:
                    span = 10000;
                    break;
                  case 4:
                    span = 12000;
                    break;
                  default:
                    spanFlag = 0;
                    break;
                }
                spanFlag = 1;
              }
            }
          } //while
          pressed = 0;
          selectPageFlag = 2;
          break;
      }  //switch (b7) {
    }
  }
}
