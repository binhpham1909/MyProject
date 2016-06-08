/* DHT library

MIT license
written by Adafruit Industries
*/

#include "DHT.h"

#define MIN_INTERVAL 2000

void DHT::init(uint8_t pin, uint8_t type, uint8_t count, boolean DEBUG) {
  DHT_PIN = pin;
  DHT_TYPE = type;
  #ifdef __AVR
    _bit = digitalPinToBitMask(pin);
    _port = digitalPinToPort(pin);
  #endif
  DHT_MAX_CYCLES = microsecondsToClockCycles(1000);  // 1 millisecond timeout for
                                                 // reading pulses from DHT sensor.
  // Note that count is now ignored as the DHT reading algorithm adjusts itself
  // basd on the speed of the processor.
  pinMode(DHT_PIN, INPUT_PULLUP);
  DHT_LASTREAD_TIME = -MIN_INTERVAL;
  DEBUG_HBI("Max clock cycles: "); 
  DEBUGln_HBI(DHT_MAX_CYCLES, DEC);
}

//boolean S == Scale.  True == Fahrenheit; False == Celcius
float DHT::readTemperature(bool S, bool force) {
  float f = 0;

  if (read(force)) {
    switch (DHT_TYPE) {
    case DHT11:
      f = data[2];
      if(S) {
        f = convertCtoF(f);
      }
      break;
    case DHT22:
    case DHT21:
      f = data[2] & 0x7F;
      f *= 256;
      f += data[3];
      f *= 0.1;
      if (data[2] & 0x80) {
        f *= -1;
      }
      if(S) {
        f = convertCtoF(f);
      }
      break;
    }
  }
  return f;
}

float DHT::convertCtoF(float c) {
  return c * 1.8 + 32;
}

float DHT::convertFtoC(float f) {
  return (f - 32) * 0.55555;
}

float DHT::readHumidity(bool force) {
  float f = 0;
  if (read()) {
    switch (DHT_TYPE) {
    case DHT11:
      f = data[0];
      break;
    case DHT22:
    case DHT21:
      f = data[0];
      f *= 256;
      f += data[1];
      f *= 0.1;
      break;
    }
  }
  return f;
}

//boolean isFahrenheit: True == Fahrenheit; False == Celcius
float DHT::computeHeatIndex(float temperature, float percentHumidity, bool isFahrenheit) {
  // Using both Rothfusz and Steadman's equations
  // http://www.wpc.ncep.noaa.gov/html/heatindex_equation.shtml
  float hi;

  if (!isFahrenheit)
    temperature = convertCtoF(temperature);

  hi = 0.5 * (temperature + 61.0 + ((temperature - 68.0) * 1.2) + (percentHumidity * 0.094));

  if (hi > 79) {
    hi = -42.379 +
             2.04901523 * temperature +
            10.14333127 * percentHumidity +
            -0.22475541 * temperature*percentHumidity +
            -0.00683783 * pow(temperature, 2) +
            -0.05481717 * pow(percentHumidity, 2) +
             0.00122874 * pow(temperature, 2) * percentHumidity +
             0.00085282 * temperature*pow(percentHumidity, 2) +
            -0.00000199 * pow(temperature, 2) * pow(percentHumidity, 2);

    if((percentHumidity < 13) && (temperature >= 80.0) && (temperature <= 112.0))
      hi -= ((13.0 - percentHumidity) * 0.25) * sqrt((17.0 - abs(temperature - 95.0)) * 0.05882);

    else if((percentHumidity > 85.0) && (temperature >= 80.0) && (temperature <= 87.0))
      hi += ((percentHumidity - 85.0) * 0.1) * ((87.0 - temperature) * 0.2);
  }

  return isFahrenheit ? hi : convertFtoC(hi);
}

boolean DHT::read(bool force) {
  // Check if sensor was read less than two seconds ago and return early
  // to use last reading.
  uint32_t currenttime = millis();
  if (!force && ((currenttime - DHT_LASTREAD_TIME) < 2000)) {
    return DHT_LAST_RESULT; // return last correct measurement
  }
  DHT_LASTREAD_TIME = currenttime;

  // Reset 40 bits of received data to zero.
  data[0] = data[1] = data[2] = data[3] = data[4] = 0;

  // Send start signal.  See DHT datasheet for full signal diagram:
  // Go into high impedence state to let pull-up raise data line level and
  // start the reading process.
  digitalWrite(DHT_PIN, HIGH);
  delay(250);

  // First set data line low for 20 milliseconds.
  pinMode(DHT_PIN, OUTPUT);
  digitalWrite(DHT_PIN, LOW);
  delay(20);

  uint32_t cycles[80];
  {
    // Turn off interrupts temporarily because the next sections are timing critical
    // and we don't want any interruptions.
    InterruptLock lock;

    // End the start signal by setting data line high for 40 microseconds.
    digitalWrite(DHT_PIN, HIGH);
    delayMicroseconds(40);

    // Now start reading the data line to get the value from the DHT sensor.
    pinMode(DHT_PIN, INPUT_PULLUP);
    delayMicroseconds(10);  // Delay a bit to let sensor pull data line low.

    // First expect a low signal for ~80 microseconds followed by a high signal
    // for ~80 microseconds again.
    if (expectPulse(LOW) == 0) {
      DEBUGln_HBI(F("Timeout waiting for start signal low pulse."));
      DHT_LAST_RESULT = false;
      return DHT_LAST_RESULT;
    }
    if (expectPulse(HIGH) == 0) {
      DEBUGln_HBI(F("Timeout waiting for start signal high pulse."));
      DHT_LAST_RESULT = false;
      return DHT_LAST_RESULT;
    }

    // Now read the 40 bits sent by the sensor.  Each bit is sent as a 50
    // microsecond low pulse followed by a variable length high pulse.  If the
    // high pulse is ~28 microseconds then it's a 0 and if it's ~70 microseconds
    // then it's a 1.  We measure the cycle count of the initial 50us low pulse
    // and use that to compare to the cycle count of the high pulse to determine
    // if the bit is a 0 (high state cycle count < low state cycle count), or a
    // 1 (high state cycle count > low state cycle count). Note that for speed all
    // the pulses are read into a array and then examined in a later step.
    for (int i=0; i<80; i+=2) {
      cycles[i]   = expectPulse(LOW);
      cycles[i+1] = expectPulse(HIGH);
    }
  } // Timing critical code is now complete.

  // Inspect pulses and determine which ones are 0 (high state cycle count < low
  // state cycle count), or 1 (high state cycle count > low state cycle count).
  for (int i=0; i<40; ++i) {
    uint32_t lowCycles  = cycles[2*i];
    uint32_t highCycles = cycles[2*i+1];
    if ((lowCycles == 0) || (highCycles == 0)) {
      DEBUGln_HBI(F("Timeout waiting for pulse."));
      DHT_LAST_RESULT = false;
      return DHT_LAST_RESULT;
    }
    data[i/8] <<= 1;
    // Now compare the low and high cycle times to see if the bit is a 0 or 1.
    if (highCycles > lowCycles) {
      // High cycles are greater than 50us low cycle count, must be a 1.
      data[i/8] |= 1;
    }
    // Else high cycles are less than (or equal to, a weird case) the 50us low
    // cycle count so this must be a zero.  Nothing needs to be changed in the
    // stored data.
  }
  
     DEBUGln_HBI(F("Received:"));
     DEBUG_HBI(data[0], HEX);    DEBUG_HBI(F(", ")); 
     DEBUG_HBI(data[1], HEX);    DEBUG_HBI(F(", "));
     DEBUG_HBI(data[2], HEX);    DEBUG_HBI(F(", "));
     DEBUG_HBI(data[3], HEX);    DEBUG_HBI(F(", "));
     DEBUG_HBI(data[4], HEX);    DEBUG_HBI(F(" =? "));
     DEBUGln_HBI((data[0] + data[1] + data[2] + data[3]) & 0xFF, HEX);  
  // Check we read 40 bits and that the checksum matches.
  if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
    DHT_LAST_RESULT = true;
    return DHT_LAST_RESULT;
  }
  else {
    if(DEBUG)   DEBUGln_HBI(F("Checksum failure!"));
    DHT_LAST_RESULT = false;
    return DHT_LAST_RESULT;
  }
}

// Expect the signal line to be at the specified level for a period of time and
// return a count of loop cycles spent at that level (this cycle count can be
// used to compare the relative time of two pulses).  If more than a millisecond
// ellapses without the level changing then the call fails with a 0 response.
// This is adapted from Arduino's pulseInLong function (which is only available
// in the very latest IDE versions):
//   https://github.com/arduino/Arduino/blob/master/hardware/arduino/avr/cores/arduino/wiring_pulse.c
uint32_t DHT::expectPulse(bool level) {
  uint32_t count = 0;
  // On AVR platforms use direct GPIO port access as it's much faster and better
  // for catching pulses that are 10's of microseconds in length:
  #ifdef __AVR
    uint8_t portState = level ? _bit : 0;
    while ((*portInputRegister(_port) & _bit) == portState) {
      if (count++ >= DHT_MAX_CYCLES) {
        return 0; // Exceeded timeout, fail.
      }
    }
  // Otherwise fall back to using digitalRead (this seems to be necessary on ESP8266
  // right now, perhaps bugs in direct port access functions?).
  #else
    while (digitalRead(DHT_PIN) == level) {
      if (count++ >= DHT_MAX_CYCLES) {
        return 0; // Exceeded timeout, fail.
      }
    }
  #endif

  return count;
}
