/*!
 * @file TeamAt_DRV2605.cpp
 *
 * Modification from the original ADAfruit library ( info below)
 *
 * This is a library for the Adafruit DRV2605L Haptic Driver ---->
 * http://www.adafruit.com/products/2305
 *
 * Check out the links above for our tutorials and wiring diagrams.
 *
 * This motor/haptic driver uses I2C to communicate.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * @section author Author
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries.
 *
 * @section license License
 *
 * MIT license, all text above must be included in any redistribution.
 *
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "TeamAt_DRV2605.h"
#include <Wire.h>

/*========================================================================*/
/*                            CONSTRUCTORS                                */
/*========================================================================*/

/**************************************************************************/
/*!
  @brief  Instantiates a new DRV2605 class. I2C, no address adjustments or pins
*/
/**************************************************************************/
TeamAt_DRV2605::TeamAt_DRV2605() {}

/*========================================================================*/
/*                           PUBLIC FUNCTIONS                             */
/*========================================================================*/

/**************************************************************************/
/*!
  @brief Setup HW using a specified Wire
  @param theWire Pointer to a TwoWire object, defaults to &Wire
  @return Return value from init()
*/
/**************************************************************************/
boolean TeamAt_DRV2605::begin(bool mode, TwoWire *theWire) {
  _wire = theWire;
  if(mode == MODE_ERM)
  {
    return initERM();
  }
  else
  {
    return initLRA();
  }
  
}

/**************************************************************************/
/*!
  @brief  Setup the HW
  @return Always true
*/
/**************************************************************************/
boolean TeamAt_DRV2605::initERM() {
  _wire->begin();
  uint8_t id = readRegister8(DRV2605_REG_STATUS);
  // Serial.print("Status 0x"); Serial.println(id, HEX);

  writeRegister8(DRV2605_REG_MODE, 0x00); // out of standby

  writeRegister8(DRV2605_REG_RTPIN, 0x00); // no real-time-playback

   writeRegister8(DRV2605_REG_WAVESEQ1, 0); // strong click
   writeRegister8(DRV2605_REG_WAVESEQ2, 0); // end sequence

  writeRegister8(DRV2605_REG_OVERDRIVE, 0); // no overdrive

  writeRegister8(DRV2605_REG_SUSTAINPOS, 0);
  writeRegister8(DRV2605_REG_SUSTAINNEG, 0);
  writeRegister8(DRV2605_REG_BREAK, 0);
  writeRegister8(DRV2605_REG_AUDIOMAX, 0x64);

  // ERM open loop

  // turn off N_ERM_LRA
  // writeRegister8(DRV2605_REG_FEEDBACK,
  //                readRegister8(DRV2605_REG_FEEDBACK) & 0x7F);
    writeRegister8(DRV2605_REG_FEEDBACK, 
            0 << 7 | // N_ERM_LRA = ERM mode
            3 << 4 | // brake factor default = 2
            2 << 2 | // loop gain default = 2
            2 // B emf gain
  );

                 ///test /// test
 writeRegister8(DRV2605_REG_CONTROL4, 
                                  0 << 6 | // ZC_DET_TIME: — A value of 0 is valid for most actuators.
                                  3 << 4  // AUTO_CAL_TIME = 3
  );

  writeRegister8(DRV2605_REG_CONTROL1,
                                    1 << 7 | //  STARTUP_BOOST
                                    0        // Drive Time (ms) = DRIVE_TIME[4:0] × 0.2 ms + 1 ms
  );

  writeRegister8(DRV2605_REG_CONTROL2,
                                    1 << 7| // Bidirectional input mode (default)
                                    1 << 6| // BRAKE_STABILIZER
                                    3 << 4| // SAMPLE_TIME  : A value of 3 is valid for most actuators.
                                    1 << 2| //BLANKING_TIME : A value of 1 is valid for most actuators.
                                    1       //IDISS_TIME :  A value of 1 is valid for most actuators  
  );

                 ///fin test // 
  // turn on ERM_OPEN_LOOP
  writeRegister8(DRV2605_REG_CONTROL3,
                                    2 << 6 | // noise-gate threshold2: 4% (Default)
                                    1 << 5 | // ERM_OPEN_LOOP 
                                    0   //   LRA_OPEN_LOOP 1: LRA open-loop mode
                 );

  return true;
}

boolean TeamAt_DRV2605::initLRA() {

  _wire->begin();
  uint8_t id = readRegister8(DRV2605_REG_STATUS);
  // Serial.print("Status 0x"); Serial.println(id, HEX);

  writeRegister8(DRV2605_REG_MODE, 0x00); // out of standby, internal trigger mode.

  writeRegister8(DRV2605_REG_RTPIN, 0x00); // no real-time-playback

   writeRegister8(DRV2605_REG_WAVESEQ1, 0); // strong click
   writeRegister8(DRV2605_REG_WAVESEQ2, 0); // end sequence

  writeRegister8(DRV2605_REG_OVERDRIVE, 0); // no overdrive

  writeRegister8(DRV2605_REG_SUSTAINPOS, 0);
  writeRegister8(DRV2605_REG_SUSTAINNEG, 0);
  writeRegister8(DRV2605_REG_BREAK, 0);
  writeRegister8(DRV2605_REG_AUDIOMAX, 0x64);

  // ERM open loop

  // turn off N_ERM_LRA
  // writeRegister8(DRV2605_REG_FEEDBACK,
  //                readRegister8(DRV2605_REG_FEEDBACK) & 0x7F);
    writeRegister8(DRV2605_REG_FEEDBACK, 
            1 << 7 | // N_ERM_LRA = LRA mode
            3 << 4 | // brake factor default = 2
            2 << 2 | // loop gain default = 2
            2 // B emf gain
  );

                 ///test /// test
 writeRegister8(DRV2605_REG_CONTROL4, 
                                  0 << 6 | // ZC_DET_TIME: — A value of 0 is valid for most actuators.
                                  3 << 4  // AUTO_CAL_TIME = 3
  );

  writeRegister8(DRV2605_REG_CONTROL1,
                                    1 << 7 | //  STARTUP_BOOST
                                    10        // Drive Time (ms) = DRIVE_TIME[4:0] × 0.2 ms + 1 ms   pour VG1040003D @170hz optimal =2.94ms  donc (2.9-1)/.2 = 9.5 --> 10
  );

  writeRegister8(DRV2605_REG_CONTROL2,
                                    3 << 4| // SAMPLE_TIME  : A value of 3 is valid for most actuators.
                                    1 << 2| //BLANKING_TIME : A value of 1 is valid for most actuators.
                                    1       //IDISS_TIME :  A value of 1 is valid for most actuators  
  );

  ///fin test // 
  // turn on LRA_OPEN_LOOP
  writeRegister8(DRV2605_REG_CONTROL3,
                                    2 << 6 | // noise-gate threshold2: 4% (Default)
                                    0 << 5 | // ERM_OPEN_LOOP 
                                    1   //   LRA_OPEN_LOOP 1: LRA open-loop mode
                 );

  return true;
}


bool TeamAt_DRV2605::calibrate(bool mode) {
  
  _wire->begin();

  writeRegister8(DRV2605_REG_MODE, 0x07);

if(mode == MODE_ERM)
{
    // turn off N_ERM_LRA
    writeRegister8(DRV2605_REG_FEEDBACK, 
              0 << 7 | // N_ERM_LRA = ERM mode
              3 << 4 | // brake factor default = 2
              2 << 2 | // loop gain default = 1
              2 // B emf gain
    );

  // writeRegister8(DRV2605_REG_RATEDV,155); // en théorie inutilisé en poen loop 3.3v/21.18*10^-3

  writeRegister8(DRV2605_REG_CLAMPV,182); // Open-Loop Overdrive Voltage = Voverdrive * 255 / 5.6 4*255/5.6

    writeRegister8(DRV2605_REG_CONTROL4, 
                                    0 << 6 | // ZC_DET_TIME: — A value of 0 is valid for most actuators.
                                    3 << 4  // AUTO_CAL_TIME = 3
    );

      writeRegister8(DRV2605_REG_CONTROL1,
                                      1 << 7 | //  STARTUP_BOOST
                                      10        // Drive Time (ms) = DRIVE_TIME[4:0] × 0.2 ms + 1 ms   pour VG1040003D @170hz optimal =2.94ms  donc (2.9-1)/.2 = 9.5 --> 10
    );

    writeRegister8(DRV2605_REG_CONTROL2,
                                      1 << 7| // Bidirectional input mode (default)
                                      3 << 4| // SAMPLE_TIME  : A value of 3 is valid for most actuators.
                                      1 << 2| //BLANKING_TIME : A value of 1 is valid for most actuators.
                                      1       //IDISS_TIME :  A value of 1 is valid for most actuators  
    );

}
else
{
    // turn off N_ERM_LRA
      writeRegister8(DRV2605_REG_FEEDBACK, 
                1 << 7 | // N_ERM_LRA = ERM mode
                3 << 4 | // brake factor default = 2
                2 << 2 | // loop gain default = 1
                2 // B emf gain
      );

    //writeRegister8(DRV2605_REG_RATEDV,155); // en théorie inutilisé en open loop 3.3v/21.18*10^-3

    writeRegister8(DRV2605_REG_CLAMPV,114); // Open-Loop Overdrive Voltage = Voverdrive * 255 / 5.6 2.5*255/5.6

      writeRegister8(DRV2605_REG_CONTROL4, 
                                      0 << 6 | // ZC_DET_TIME: — A value of 0 is valid for most actuators.
                                      3 << 4  // AUTO_CAL_TIME = 3
      );

        writeRegister8(DRV2605_REG_CONTROL1,
                                        1 << 7 | //  STARTUP_BOOST
                                        20        // Drive Time (ms) = DRIVE_TIME[4:0] × 0.2 ms + 1 ms   pour VG1040003D @170hz optimal =2.94ms  donc (2.9-1)/.2 = 9.5 --> 10
      );

      writeRegister8(DRV2605_REG_CONTROL2,
                                        1 << 7| // Bidirectional input mode (default)

                                        3 << 4| // SAMPLE_TIME  : A value of 3 is valid for most actuators.
                                        1 << 2| //BLANKING_TIME : A value of 1 is valid for most actuators.
                                        1       //IDISS_TIME :  A value of 1 is valid for most actuators  
      );
}


  writeRegister8(DRV2605_REG_GO, 1);

  uint8_t id;
  do
  {
    id = readRegister8(DRV2605_REG_GO);
  } while (id == 1);
 
   id = readRegister8(DRV2605_REG_STATUS);

    writeRegister8(DRV2605_REG_MODE, 0x00); // out of standby, internal trigger mode.
   if(id & 0b100)
   {
      return false;
   }

   // Ensuite il faut enregistrer les valeurs des registres 0x1A, 0x18 et 0x19 pour les réécrire au prochain init

   return true;

}



/**************************************************************************/
/*!
  @brief Select the haptic waveform to use.
  @param slot The waveform slot to set, from 0 to 7
  @param w The waveform sequence value, refers to an index in the ROM library.

    Playback starts at slot 0 and continues through to slot 7, stopping if it
  encounters a value of 0. A list of available waveforms can be found in
  section 11.2 of the datasheet: http://www.adafruit.com/datasheets/DRV2605.pdf
*/
/**************************************************************************/
void TeamAt_DRV2605::setWaveform(uint8_t slot, uint8_t w) {

  writeRegister8(DRV2605_REG_WAVESEQ1 + slot, w);
}

/**************************************************************************/
/*!
  @brief Select the waveform library to use.
  @param lib Library to use, 0 = Empty, 1-5 are ERM, 6 is LRA.

    See section 7.6.4 in the datasheet for more details:
  http://www.adafruit.com/datasheets/DRV2605.pdf
*/
/**************************************************************************/
void TeamAt_DRV2605::selectLibrary(uint8_t lib) {
  writeRegister8(DRV2605_REG_LIBRARY, lib);
}

/**************************************************************************/
/*!
  @brief Start playback of the waveforms (start moving!).
*/
/**************************************************************************/
void TeamAt_DRV2605::go() { writeRegister8(DRV2605_REG_GO, 1); }

/**************************************************************************/
/*!
  @brief Stop playback.
*/
/**************************************************************************/
void TeamAt_DRV2605::stop() { writeRegister8(DRV2605_REG_GO, 0); }

/**************************************************************************/
/*!
  @brief Set the device mode.
  @param mode Mode value, see datasheet section 7.6.2:
  http://www.adafruit.com/datasheets/DRV2605.pdf

    0: Internal trigger, call go() to start playback\n
    1: External trigger, rising edge on IN pin starts playback\n
    2: External trigger, playback follows the state of IN pin\n
    3: PWM/analog input\n
    4: Audio\n
    5: Real-time playback\n
    6: Diagnostics\n
    7: Auto calibration
*/
/**************************************************************************/
void TeamAt_DRV2605::setMode(uint8_t mode) {
  writeRegister8(DRV2605_REG_MODE, mode);
}

/**************************************************************************/
/*!
  @brief Set the realtime value when in RTP mode, used to directly drive the
  haptic motor.
  @param rtp 8-bit drive value.
*/
/**************************************************************************/
void TeamAt_DRV2605::setRealtimeValue(uint8_t rtp) {
  writeRegister8(DRV2605_REG_RTPIN, rtp);
}

/**************************************************************************/
/*!
  @brief Read an 8-bit register.
  @param reg The register to read.
  @return 8-bit value of the register.
*/
/**************************************************************************/
uint8_t TeamAt_DRV2605::readRegister8(uint8_t reg) {
  uint8_t x;

  // use i2c
  _wire->beginTransmission(DRV2605_ADDR);
  _wire->write((byte)reg);
  _wire->endTransmission();
  _wire->requestFrom((byte)DRV2605_ADDR, (byte)1);
  x = _wire->read();

  //  Serial.print("$"); Serial.print(reg, HEX);
  //  Serial.print(": 0x"); Serial.println(x, HEX);

  return x;
}

/**************************************************************************/
/*!
  @brief Write an 8-bit register.
  @param reg The register to write.
  @param val The value to write.
*/
/**************************************************************************/
void TeamAt_DRV2605::writeRegister8(uint8_t reg, uint8_t val) {
  // use i2c
  _wire->beginTransmission(DRV2605_ADDR);
  _wire->write((byte)reg);
  _wire->write((byte)val);
  _wire->endTransmission();
}

/**************************************************************************/
/*!
  @brief Use ERM (Eccentric Rotating Mass) mode.
*/
/**************************************************************************/
void TeamAt_DRV2605::useERM() {
  writeRegister8(DRV2605_REG_FEEDBACK,
                 readRegister8(DRV2605_REG_FEEDBACK) & 0x7F);
}

/**************************************************************************/
/*!
  @brief Use LRA (Linear Resonance Actuator) mode.
*/
/**************************************************************************/
void TeamAt_DRV2605::useLRA() {
  writeRegister8(DRV2605_REG_FEEDBACK,
                 readRegister8(DRV2605_REG_FEEDBACK) | 0x80);
}
