#include "hapticFeedback_arduino.h"


#include "arduino.h"


void hapticFeedback_arduino::init(bool mode)
{
   
//   if (haptic.begin() != HAPTIC_SUCCESS) {
//       Serial.println("Haptic: Driver Error: DRV2605L Device not found - check your I2C connections?");
//   } else {
      // okay correct chip, let's initialize it
      //Serial.println("Haptic: DeviceID = DRV2605L @ Address 0x5A was found! ");
      // config the DRV2605 chip
      //   Serial.println("haptic init");
      
 
      // Serial.println("Haptic init.");
      haptic.begin(mode);

     
      haptic.setMode(DRV2605_MODE_INTTRIG);


        if(mode == MODE_ERM)
        {
          haptic.selectLibrary(2); // Use library 2 ( see datasheet)
        }
        else{
          haptic.selectLibrary(6); // Use library 6 for LRA 
        }

        haptic.calibrate(mode); // Use library 2 ( see datasheet)
      
         haptic.begin(mode);

        // waveFormTable[WF_SOFT_CLICK] = 5; 
        // waveFormTable[WF_HARD_CLICK] = 1;
        // waveFormTable[WF_MEDIUM_BUZZ] = DRV_750ms_alert;
        // waveFormTable[WF_HARD_BUZZ] = DRV_1000ms_alert;
        // waveFormTable[WF_HARD_BUZZ2] = DRV_strong_buzz_alert;
        // waveFormTable[WF_DOUBLE_CLICK] = 10;
        // waveFormTable[WF_PULSING_STRONG] = DRV_pulsing_strong_1;
        // waveFormTable[WF_NO_GO] = 12;

//   }       

}

bool hapticFeedback_arduino::calibrate(bool mode)
{
  bool returnVal =  haptic.calibrate(mode);
  haptic.begin(mode);

  return returnVal;
}

void hapticFeedback_arduino::playWaveform(uint8_t waveformId)
{
    haptic.setWaveform(0,waveformId);   // set the first sequence
    haptic.setWaveform(1, 0);           // end the sequence
    haptic.go();				                // play the waveform
}


 void hapticFeedback_arduino::playSequence(uint8_t* sequenceTable,uint8_t sequenceLength)
 {

    if(sequenceLength >= SEQUENCE_LEN_MAX )
    {
      sequenceLength = SEQUENCE_LEN_MAX;
    }

int i;
    for (i = 0; i < sequenceLength; i++)
    {
       haptic.setWaveform(i,sequenceTable[i]);   // set the first sequence
    }

    if(i < SEQUENCE_LEN_MAX)
    {
      haptic.setWaveform(i,0);
    }
    haptic.go();	

 }