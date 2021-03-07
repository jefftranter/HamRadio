
#ifndef BEENHERE
#include "SigGen.h"
#endif
#include <Arduino.h>


// #########################################################################
//  Draw the analogue meter on the screen
// #########################################################################
/*****
  Purpose: Draw on-screen analog meter
  Paramter list: void
  Return value:  void
*****/
void analogMeter()
{
  // Meter outline
  tft.fillRect(0, YOFFSETMETER, M_SIZE * 239, M_SIZE * 126, TFT_GREY);
  tft.fillRect(5, YOFFSETMETER + 3, M_SIZE * 230, M_SIZE * 119, TFT_WHITE);
  tft.setFreeFont(FS12);
  tft.setTextColor(TFT_BLACK);  // Text colour

  // Draw ticks every 5 degrees from -50 to +50 degrees (100 deg. FSD swing)
  for (int i = -50; i < 51; i += 5) {
    // Long scale tick length
    int tl = 15;

    // Coodinates of tick to draw
    float sx = cos((i - 90) * 0.0174532925);
    float sy = sin((i - 90) * 0.0174532925);
    uint16_t x0 = sx * (M_SIZE * 100 + tl) + M_SIZE * 120;
    uint16_t y0 = sy * (M_SIZE * 100 + tl) + M_SIZE * 140;
    uint16_t x1 = sx * M_SIZE * 100 + M_SIZE * 120;
    uint16_t y1 = sy * M_SIZE * 100 + M_SIZE * 140;

    // Coordinates of next tick for zone fill
    float sx2 = cos((i + 5 - 90) * 0.0174532925);
    float sy2 = sin((i + 5 - 90) * 0.0174532925);
    int x2 = sx2 * (M_SIZE * 100 + tl) + M_SIZE * 120;
    int y2 = sy2 * (M_SIZE * 100 + tl) + M_SIZE * 140;
    int x3 = sx2 * M_SIZE * 100 + M_SIZE * 120;
    int y3 = sy2 * M_SIZE * 100 + M_SIZE * 140;

    if (i >= 0 && i < 25) {
      tft.fillTriangle(x0, YOFFSETMETER + y0, x1, YOFFSETMETER + y1, x2, YOFFSETMETER + y2, TFT_GREEN);
      tft.fillTriangle(x1, YOFFSETMETER + y1, x2, YOFFSETMETER + y2, x3, YOFFSETMETER + y3, TFT_GREEN);
    }

    // Orange zone limits
    if (i >= 25 && i < 50) {
      tft.fillTriangle(x0, YOFFSETMETER + y0, x1, YOFFSETMETER + y1, x2, YOFFSETMETER + y2, TFT_ORANGE);
      tft.fillTriangle(x1, YOFFSETMETER + y1, x2, YOFFSETMETER + y2, x3, YOFFSETMETER + y3, TFT_ORANGE);
    }

    // Short scale tick length
    if (i % 25 != 0) tl = 8;

    // Recalculate coords incase tick lenght changed
    x0 = sx * (M_SIZE * 100 + tl) + M_SIZE * 120;
    y0 = sy * (M_SIZE * 100 + tl) + M_SIZE * 140;
    x1 = sx * M_SIZE * 100 + M_SIZE * 120;
    y1 = sy * M_SIZE * 100 + M_SIZE * 140;

    // Draw tick
    tft.drawLine(x0, YOFFSETMETER + y0, x1, YOFFSETMETER + y1, TFT_BLACK); //ok

    // Check if labels should be drawn, with position tweaks
    tft.setFreeFont(FS9);
    if (i % 25 == 0) {
      // Calculate label positions
      x0 = sx * (M_SIZE * 100 + tl + 10) + M_SIZE * 120;
      y0 = sy * (M_SIZE * 100 + tl + 10) + M_SIZE * 140;
      switch (i / 25) {
        case -2: tft.drawCentreString("0", x0, YOFFSETMETER + y0 - 12, 1); break;
        case -1: tft.drawCentreString(".5", x0, YOFFSETMETER + y0 - 9, 1); break;
        case 0: tft.drawCentreString("1.0", x0, YOFFSETMETER + y0 - 7, 1); break;
        case 1: tft.drawCentreString("1.5", x0, YOFFSETMETER + y0 - 9, 1); break;
        case 2: tft.drawCentreString("2", x0, YOFFSETMETER + y0 - 12, 1); break;
      }
    }

    // Now draw the arc of the scale
    sx = cos((i + 5 - 90) * 0.0174532925);
    sy = sin((i + 5 - 90) * 0.0174532925);
    x0 = sx * M_SIZE * 100 + M_SIZE * 120;
    y0 = sy * M_SIZE * 100 + M_SIZE * 140;
    // Draw scale arc, don't draw the last part
    if (i < 50) tft.drawLine(x0, YOFFSETMETER + y0, x1, YOFFSETMETER + y1, TFT_BLACK);
  }

  //tft.drawString("%RH", M_SIZE*(5 + 230 - 40), M_SIZE*(119 - 20), 2); // Units at bottom right
  //tft.drawCentreString("%RH", M_SIZE*120, M_SIZE*70, 4); // Comment out to avoid font 4
  tft.drawRect(5, YOFFSETMETER + 3, M_SIZE * 230, M_SIZE * 119, TFT_BLACK); // Draw bezel line

  plotNeedle(0, 0); // Put meter needle at 0
}

// #########################################################################
// Update needle position
// This function is blocking while needle moves, time depends on ms_delay
// 10ms minimises needle flicker if text is drawn within needle sweep area
// Smaller values OK if text not in sweep area, zero for instant movement but
// does not look realistic... (note: 100 increments for full scale deflection)
// #########################################################################
/*****
  Purpose: Plot on-screen analog meter needle
  Paramter list: void
  Return value:  void
*****/
void plotNeedle(int value1, byte ms_delay)
{
  value1 = value1 / 10;
  //tft.setFreeFont(FS12);
  //tft.setTextColor(TFT_BLACK, TFT_WHITE);
  //char buf[8]; dtostrf(value1, 4, 0, buf);
  //tft.drawRightString(buf, 10 + M_SIZE * 40, 10 + M_SIZE * (119 ), 1);
  tft.fillRect(8, 116, 46, 20, TFT_WHITE);
  tft.setFreeFont(FS12);
  tft.setTextColor(TFT_BLACK, TFT_BLACK);
  tft.setCursor(10, 134);
  float valuePrint= (float)value1/100;
  tft.print(valuePrint);

  if (value1 < 0) value1 = 0; // Limit value1 to emulate needle end stops
  if (value1 > 200) value1 = 200;

  // Move the needle until new value1 reached
  while (!(value1 == old_analog)) {
    if (old_analog < value1) old_analog++;
    else old_analog--;

    if (ms_delay == 0) old_analog = value1; // Update immediately if delay is 0

    //float sdeg = map(old_analog, -10, 110, -150, -30); // Map value1 to angle
    float sdeg = map(old_analog, 0, 200, -140, -40); // Map value1 to angle
    // Calcualte tip of needle coords
    float sx = cos(sdeg * 0.0174532925);
    float sy = sin(sdeg * 0.0174532925);

    // Calculate x delta of needle start (does not start at pivot point)
    float tx = tan((sdeg + 90) * 0.0174532925);

    // Erase old needle image
    tft.drawLine(M_SIZE * (120 + 20 * ltx - 1), YOFFSETMETER + M_SIZE * (140 - 20), osx - 1, YOFFSETMETER + osy, TFT_WHITE);
    tft.drawLine(M_SIZE * (120 + 20 * ltx), YOFFSETMETER + M_SIZE * (140 - 20), osx, YOFFSETMETER + osy, TFT_WHITE);
    tft.drawLine(M_SIZE * (120 + 20 * ltx + 1), YOFFSETMETER + M_SIZE * (140 - 20), osx + 1, YOFFSETMETER + osy, TFT_WHITE);

    // Re-plot text under needle
    //tft.setTextColor(TFT_BLACK);
    //tft.drawCentreString("%RH", M_SIZE*120, M_SIZE*70, 4); // // Comment out to avoid font 4

    // Store new needle end coords for next erase
    ltx = tx;
    osx = M_SIZE * (sx * 98 + 120);
    osy = M_SIZE * (sy * 98 + 140);

    // Draw the needle in the new postion, magenta makes needle a bit bolder
    // draws 3 lines to thicken needle
    tft.drawLine(M_SIZE * (120 + 20 * ltx - 1), YOFFSETMETER + M_SIZE * (140 - 20), osx - 1, YOFFSETMETER + osy, TFT_RED);
    tft.drawLine(M_SIZE * (120 + 20 * ltx), YOFFSETMETER + M_SIZE * (140 - 20), osx, YOFFSETMETER + osy, TFT_MAGENTA);
    tft.drawLine(M_SIZE * (120 + 20 * ltx + 1), YOFFSETMETER + M_SIZE * (140 - 20), osx + 1, YOFFSETMETER + osy, TFT_RED);

    // Slow needle down slightly as it approaches new postion
    if (abs(old_analog - value1) < 10) ms_delay += ms_delay / 5;

    // Wait before next update
    delay(ms_delay);
  }
}
/*****
  Purpose: Calibrate Log Amp 1 Freq A
  Paramter list: void
  Return value:  void
*****/
void logAmpCalibrate() {
  float voltdB100;
  float voltdB1;
  if (meterFlag1 == 0)  //Chan A
  {
    logAmpOut = (.06 * analogRead(35) + .94 * logAmpOutOld);
    logAmpOutOld = logAmpOut;
    //Serial.print("logAmpOutOld=  "); Serial.println(logAmpOutOld);
    voltdB100 = logAmpOut * 10;
     voltdB1 =  (map(voltdB100, 37660, 28850, 60, -60));
  }
  else {
    if (meterFlag1 == 1) {
      logAmpOut = (.06 * analogRead(34) + .94 * logAmpOutOld);
      logAmpOutOld = logAmpOut;
      //Serial.print("logAmpOutOld=  "); Serial.println(logAmpOutOld);
      voltdB100 = logAmpOut * 10;
      voltdB1 =  (map(voltdB100, 38480, 28850, 50, -60));
    }
  }
  voltdB1 = voltdB1 / 10; // Set values to calibrate output reading cal:  map(reading from A0, lower cal#x100, upper cal#x100, lower dBVx100, upper dBVx100
  voltsConv = pow(10, voltdB1 / 20);
  //Serial.print("voltsConv=  "); Serial.println(voltsConv); Serial.println();
}
/*****
   Purpose: Select which output Encoder will change: A or B
  Paramter list: void
  Return value:  void
*****/
void selectMeterInput() {
  //=========================  Select DDS for encoder input  ============
  for (uint8_t b4 = 0; b4 < 2; b4++) {
    if (pressed && enSelect[b4].contains(t_x, t_y)) {
      enSelect[b4].press(true);  // tell the button it is pressed
    } else {
      enSelect[b4].press(false);  // tell the button it is NOT pressed
    }
    tft.setFreeFont(FS9);
    if (enSelect[b4].justReleased()) enSelect[b4].drawButton();     // draw normal
    if (enSelect[b4].justPressed()) {
      enSelect[b4].drawButton(true);  // draw invert

      switch (b4) {
        case 0:
          enSelect[0].initButton(&tft, 240, 90, 40, 31, TFT_WHITE,  TFT_GREEN, TFT_WHITE, "A", KEY_TEXTSIZE);
          enSelect[1].initButton(&tft, 240, 124, 40, 31, TFT_WHITE,  TFT_RED, TFT_WHITE, "B", KEY_TEXTSIZE);
          enSelect[0].drawButton();
          enSelect[1].drawButton();
          meterFlag1 = 0;
          break;
        case 1:
          enSelect[1].initButton(&tft, 240, 124, 40, 31, TFT_WHITE,  TFT_GREEN, TFT_WHITE, "B", KEY_TEXTSIZE);
          enSelect[0].initButton(&tft, 240, 90, 40, 31, TFT_WHITE,  TFT_RED, TFT_WHITE, "A", KEY_TEXTSIZE);
          enSelect[0].drawButton();
          enSelect[1].drawButton();
          meterFlag1 = 1;
          break;
      }
    }
  }
}
