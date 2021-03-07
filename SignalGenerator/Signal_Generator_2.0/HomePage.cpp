#ifndef BEENHERE
#include "SigGen.h"
#endif
#include <Arduino.h>

/*****
  ================== drawHome Input Page =========================
   Purpose: To draw the Manual Input page page
   Parameters: none
   Return: None
*****/
void    drawHome() {
  tft.fillScreen(TFT_BLACK);
  // Draw keypad background
  tft.fillRect(KEYPADXOFFSET - 4, KEYPADYOFFSET + 5, 210, 320, TFT_DARKGREY);

  // Draw keypad number display area and frame
  tft.fillRect(KEYPADXOFFSET + DISP_X + 1, KEYPADYOFFSET + DISP_Y , DISP_W - 1, DISP_H - 8, TFT_BLACK);
  tft.drawRect(KEYPADXOFFSET + DISP_X + 1, KEYPADYOFFSET + DISP_Y , DISP_W - 1, DISP_H - 8, TFT_WHITE);
  //tft.fillScreen(TFT_BLACK);
  int outputValueKeypad;
  tft.setFreeFont(FSB12);
  tft.setCursor(5, 20, 2);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.print("Signal Generator V07");
  tft.setFreeFont(FS12);
  tft.drawRect(1, 185 , 260, 132, TFT_CYAN);
  tft.setCursor(438, 30, 2);
  tft.print("Hz");
  drawMainMenu();
  drawFreqButtons();
  helpButton.initButton(&tft, 240 , 40, 40, 31, TFT_BLACK,  TFT_CYAN, TFT_BLUE, "?", KEY_TEXTSIZE);
  helpButton.drawButton();

  //---------------Attenuator - init and draw attn buttons

  tft.setFreeFont(FS9);
  for (int b = 0; b <= 3; b++) {
    attnButtons[b].initButton(&tft, 296 + 50 * b , 299, 46, 26, TFT_WHITE,  TFT_RED, TFT_WHITE, setButtonLabels[b], KEY_TEXTSIZE);
    attnButtons[b].drawButton();
  }
  tft.setCursor(340, 313, 2);
  tft.setTextFont(1);
  tft.print("Attn. (dB)");

  //--------------------------------  Draw encoder  Freq select buttons

  tft.setTextFont(1);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  //tft.setCursor(222, 30, 1);
  // tft.print("Encoder");
  //tft.setCursor(222, 45, 1);
  //tft.print("Freq to");
  tft.setCursor(222, 65, 1);
  tft.print("Meter:");
  tft.setFreeFont(FS9);
  enSelect[0].initButton(&tft, 240, 90, 40, 31, TFT_WHITE,  TFT_GREEN, TFT_WHITE, "A", KEY_TEXTSIZE);
  enSelect[1].initButton(&tft, 240, 124, 40, 31, TFT_WHITE,  TFT_RED, TFT_WHITE, "B", KEY_TEXTSIZE);
  enSelect[0].drawButton();
  enSelect[1].drawButton();
}
