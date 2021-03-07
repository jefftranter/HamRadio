#ifndef BEENHERE
#include "SigGen.h"
#endif
#include <Arduino.h>
/*****
   Purpose: set 50 ohm attenuator: 0,-10dB ,-20dB, -30dB
  Paramter list: void
  Return value:  void
*****/
void attnSelect(){
  //=========================  Select Attn
  for (uint8_t b5 = 0; b5 < 4; b5++) {
    if (pressed && attnButtons[b5].contains(t_x, t_y)) {
      attnButtons[b5].press(true);  // tell the button it is pressed
    } else {
      attnButtons[b5].press(false);  // tell the button it is NOT pressed
    }
    tft.setFreeFont(FS9);
    if (attnButtons[b5].justReleased()) attnButtons[b5].drawButton();     // draw normal
    if (attnButtons[b5].justPressed()) {
      attnButtons[b5].drawButton(true);  // draw invert
      switch (b5) {
        case 0:
          digitalWrite(0, LOW);
          digitalWrite(27, LOW);
          break;
        case 1:
          digitalWrite(0,HIGH );
          digitalWrite(27, LOW);
          break;
        case 2:
          digitalWrite(0, LOW);
          digitalWrite(27, HIGH);
          break;
        case 3:
          digitalWrite(27, HIGH);
          digitalWrite(0, HIGH);
          break;
      }
    }
  }
}
