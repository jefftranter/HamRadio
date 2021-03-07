//Encoder 1 is coarse and encoder 2 is fine
#ifndef BEENHERE
#include "SigGen.h"
#endif
#include <Arduino.h>

/*****
   Purpose: ISR to read rotary encoder1
  Parameter list: void
  Return value:  void
*****/
void encoder1ISR() {
  result1 = rotary1.process();
  if (selectPageFlag == 0) {
    //Serial.print("meterFlag1= ");Serial.println(meterFlag1);
    if (result1 != 0) {
      MyDelay(100);
      eCounter1 = 0;
      if (result1 == DIR_CW) {
        eCounter1 = 1;
        en1State = 1;
      } else if (result1 == DIR_CCW) {
        eCounter1 = -1;
        en1State = 1;
      }
      if (en1State == 1) {
        freqOutA = freqOutA + eCounter1 * freqMult;
        freqFormat(freqOutA);
        tft.fillRect(79, 188, 158, 36, TFT_BLACK);
        tft.setFreeFont(FS18);
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        tft.setCursor(80, 218, 2);
        tft.print(final2);
        sendFrequency(freqOutA);
        en1State = 0;
      }
    }
  }
//}
}

/*****
   Purpose: ISR to read rotary encoder1 switch
  Parameter list: void
  Return value:  void
*****/
void encoderSWISR() {
  FM_FlagOff = 1;
  sweepFlag = 0;
   helpFlag =1;
}

/*****
   Purpose: ISR to read rotary encoder2
  Parameter list: void
  Return value:  void
*****/
void encoder2ISR() {
   
  if (selectPageFlag == 0) {
    result2 = rotary2.process();
    if (result2 != 0) {
      MyDelay(100);
      if (result2 == DIR_CW) {
        encoder2 = 1;
        kCounter2 = 1;
        en2State = 1;

      } else if (result2 == DIR_CCW) {
        encoder2 = -1;
        kCounter2 = -1;
        en2State = 1;
      }
      if (en2State == 1) {
        tft.setFreeFont(FS18);
        tft.setTextColor(TFT_GREEN, TFT_BLACK);

        //switch (meterFlag1) {
        // case 0:
        tft.fillRect(79, 188, 158, 36, TFT_BLACK);
        tft.setFreeFont(FS18);
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        tft.setCursor(80, 218, 2);
        freqOutA = freqOutA +  kCounter2 * freqMult / 10;
        freqFormat(freqOutA);
        tft.print(final2);
        sendFrequency(freqOutA);
        en1State = 0;
      }
    }
  }
}
