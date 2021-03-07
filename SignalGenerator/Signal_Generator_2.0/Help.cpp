#ifndef BEENHERE
#include "SigGen.h"
#endif
#include <Arduino.h>

/*****
   Purpose: On-screen Help File
  Parameter list: void
  Return value:  void
*****/
void drawHelp() {
  if (pressed && helpButton.contains(t_x, t_y))
  {
    tft.fillScreen(TFT_BLUE);
    tft.setFreeFont(FSB9);
    tft.setCursor(5, 20, 2);
    tft.setTextColor(TFT_YELLOW, TFT_BLUE);
    tft.print(" Select Frequency:\n");
    tft.setTextColor(TFT_WHITE, TFT_BLUE);
    tft.print("    1. Select Multiplier   2. Enter freq on numeric keypad\n");
    tft.print("    3. Select either Chan A / Chan B or SetA / SetB\n");
        tft.setTextColor(TFT_YELLOW, TFT_BLUE);
    tft.print(" AM Modulation:\n");
    tft.setTextColor(TFT_WHITE, TFT_BLUE);
    tft.print("    1. Chan A = Carrier, Chan B = Modulating Freq\n");
    tft.print("    2. Switch Mod to On   3. Set Mod Level\n");
    tft.print("    4. Suppressed Carrier On/Off  5. Adjust Carrier Null\n");
        tft.setTextColor(TFT_YELLOW, TFT_BLUE);
    tft.print(" FM Modulation:\n");
    tft.setTextColor(TFT_WHITE, TFT_BLUE);
    tft.print("    1. Select FM   2. Set Mod Freq and Span\n");
    tft.print("    3. Select FM-on Button  4.Coarse En SW to End\n");
        tft.setTextColor(TFT_YELLOW, TFT_BLUE);
    tft.print(" Sweep Chan A:\n");
    tft.setTextColor(TFT_WHITE, TFT_BLUE);
    tft.print("    1. Select Sweep   2. Set Start / End Freq,Time & # Points\n");
    tft.setTextColor(TFT_YELLOW, TFT_BLUE);
    tft.print(" Function: ");
    tft.setTextColor(TFT_WHITE, TFT_BLUE); 
        tft.print("    Set Chan B waveform - Sine, SQ, Triangle\n");
        tft.print("        Press Coarse En Sw to Exit Help");    
    while (digitalRead(22) == 1){
    }
    helpButton.press(false);
    tft.fillScreen(TFT_BLACK);
    drawHome();
    drawKeypad();
    logAmpOut = 0;
    analogMeter(); // Draw analogue meter
  }
}
