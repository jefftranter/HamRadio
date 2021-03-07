#ifndef BEENHERE
#include "SigGen.h"
#endif
#include <Arduino.h>

/*****
   Purpose: Send freq value to AD9851
  Paramter list: void
  Return value:  void
*****/
void sendFrequency(unsigned long frequency)
{
  unsigned long tuning_word = (frequency * pow(2, 32)) / DDS_CLOCK;
  digitalWrite (LOAD, LOW); // take load pin low
  for(int i = 0; i < 32; i++)
  {
    if ((tuning_word & 1) == 1)
      outOne();
    else
      outZero();
    tuning_word = tuning_word >> 1;
  }
  byte_out(0x09);
  digitalWrite (LOAD, HIGH); // Take load pin high again
}
/*****
   Purpose: Utility for AD9851
  Paramter list: void
  Return value:  void
*****/
void byte_out(unsigned char byte)
{
  int i;
  for (i = 0; i < 8; i++)
  {
    if ((byte & 1) == 1)
      outOne();
    else
      outZero();
    byte = byte >> 1;
  }
}
/*****
   Purpose: Utility for AD9851
  Paramter list: void
  Return value:  void
*****/
void outOne()
{
  digitalWrite(CLOCK, LOW);
  digitalWrite(DATA, HIGH);
  digitalWrite(CLOCK, HIGH);
  digitalWrite(DATA, LOW);
}
/*****
   Purpose: Utility for AD9851
  Paramter list: void
  Return value:  void
*****/
void outZero()
{
  digitalWrite(CLOCK, LOW);
  digitalWrite(DATA, LOW);
  digitalWrite(CLOCK, HIGH);
}
