#ifndef BEENHERE
#include "SigGen.h"
#endif
#include <Arduino.h>

int freqIndex;
char *multLabel[] = {"   Hz", "  10Hz", "  100Hz", "  KHz", " 10KHz", "100KHz", "  MHz"};

/*****
   Purpose: Read Number Buffer charaters and change to Long
  Paramter list: void
  Return value:  void
*****/
void setOutputFreq() {
  freqOut = atol(numberBuffer);
}

/*****
   Purpose: Read Keypad values and set char array NumbeBuffer
  Paramter list: void
  Return value:  void
*****/
void readKeyPad() {
  // / Check if any key coordinate boxes contain the touch coordinates
  for (uint8_t b = 0; b < 21; b++) {
    if (pressed && key[b].contains(t_x, t_y)) {
      key[b].press(true);  // tell the button it is pressed
    } else {
      key[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // Check if any key has changed state
  for (uint8_t b = 0; b < 21; b++) {

    if (b < 3 and b < 14) tft.setFreeFont(LABEL1_FONT);
    else if (b > 3 and b < 14)tft.setFreeFont(LABEL2_FONT);
    else if (b >= 14)tft.setFreeFont(FSB9);
    if (key[b].justReleased()) key[b].drawButton();     // draw normal
    if (key[b].justPressed()) {
      key[b].drawButton(true);  // draw invert
      switch (b) {
        case 0:  //clr
          ClrFreqDisplay();
          break;
        case 1:
          freqOutA = freqOut2;
          tft.fillRect(79, 188, 158, 36, TFT_BLACK);
          tft.setFreeFont(FS18);
          tft.setTextColor(TFT_GREEN, TFT_BLACK);
          tft.setCursor(80, 218, 2);
          freqFormat((long)freqOutA);
          tft.print(final2);
          sendFrequency(freqOutA);
          break;
        case 2:
          freqOutB = freqOut2;
          tft.fillRect(79, 229, 158, 37, TFT_BLACK);
          tft.setFreeFont(FS18);
          tft.setTextColor(TFT_GREEN, TFT_BLACK);
          tft.setCursor(80, 258, 2);
          freqFormat((long)freqOutB);
          tft.print(final2);
          AD_DDS2.setFrequency(chan, freqOutB);
          break;
        case 14:
          freqMult = 1;
          ClrFreqDisplay();
          break;
        case 15:
          freqMult = 10;
          ClrFreqDisplay();
          break;
        case 16:
          freqMult = 100;
          ClrFreqDisplay();
          break;
        case 17:
          freqMult = 1000;
          ClrFreqDisplay();
          break;
        case 18:
          freqMult = 10000;
          ClrFreqDisplay();
          break;
        case 19:
          freqMult = 100000;
          ClrFreqDisplay();
          break;
        case 20:
          freqMult = 1000000;
          ClrFreqDisplay();
          break;
        default:
          if (b >= 3 and b <= 14) {
            if (numberIndex < NUM_LEN) {
              numberBuffer[numberIndex] = keyLabel[b][0];
              numberIndex++;
              numberBuffer[numberIndex] = 0; // zero terminate
            }
            displayKbdOut();
          }
          break;
      }
    }
  }
  //delay(10); // UI debouncing
}

/*****
  Purpose: Draw on-screen numeric keypad
  Paramter list: void
  Return value:  void
*****/
void drawKeypad()
{
  // Draw the keys
  for (uint8_t row = 0; row < 7; row++) {
    for (uint8_t col = 0; col < 3; col++) {
      uint8_t b = col + row * 3;
      if (b < 3 and b < 14) tft.setFreeFont(LABEL1_FONT);
      else if (b > 3 and b < 14)tft.setFreeFont(LABEL2_FONT);
      else if (b >= 14)tft.setFreeFont(FSB9);
      key[b].initButton(&tft, KEYPADXOFFSET - 3 + KEY_X + col * (KEY_W + KEY_SPACING_X + 3),
                        KEYPADYOFFSET + KEY_Y + row * (KEY_H + KEY_SPACING_Y - 3) - 12,
                        KEY_W + 6, KEY_H, TFT_WHITE, keyColor1[b], keyColor2[b],
                        keyLabel[b], KEY_TEXTSIZE);// x, y, w, h, outline, fill, text
      key[b].drawButton();
    }
  }
  freqMult = 1;
  tft.setTextColor(TFT_BLACK, TFT_GREEN);
    tft.setCursor(425, 185, 2);
  tft.setTextFont(1);
  tft.print("Mult.");
  tft.setFreeFont(FS9);

}

//------------------------------------------------------------------------------------------

/*****
  Purpose: Display Keypad selection in Keypad display
  Paramter list: void
  Return value:  void
*****/
void displayKbdOut() {
  displayFreqOut = atof(numberBuffer);
  freqOut2 = displayFreqOut * freqMult;
  if (freqOut2 >= 50000000) {
    freqOut2 = 50000000;
  }
  freqFormat((long)freqOut2);
  tft.fillRect(KEYPADXOFFSET + DISP_X + 3, KEYPADYOFFSET + DISP_Y + 4 , DISP_W - 36, DISP_H - 15, TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setFreeFont(FS18);
  tft.setCursor(KEYPADXOFFSET + 5, KEYPADYOFFSET + DISP_Y + 32);
  tft.print(final2);
}

/*****
  Purpose: Format input Long to comma separated Char
  Paramter list: long freqToFormat
  Return value:  final2[final_length];
*****/
char freqFormat(long freqToFormat) {
  char buffer[12] = {0};
  ltoa(freqToFormat, buffer, 10);
  unsigned int buff_length = strlen(buffer);    // Calculate how many commas there will be
  unsigned int num_commas = buff_length / 3;
  unsigned int digits_left = buff_length % 3;
  if (digits_left == 0) {
    num_commas--;
  }
  final_length = buff_length + num_commas + 1;     // Allocate space for final string representation
  int final_pos = final_length - 2;    // Parse strings from last to first to count positions
  int buff_pos = buff_length - 1;
  memset(final2, 0, final_length);
  int i = 0;
  while (final_pos >= 0) {
    final2[final_pos--] = buffer[buff_pos--];
    i++;
    if (i % 3 == 0) {
      final2[final_pos--] = ',';
    }
  }
  return final2[final_length];
} // Enf Fn

/*****
  Purpose: Clear Freq Display
  Paramter list:
  Return value:
*****/
void ClrFreqDisplay()
{
  numberIndex = 0; // Reset index to 0
  numberBuffer[numberIndex] = 0; // Place null in buffer
  freqOut2 = 0;
  displayKbdOut();
}
