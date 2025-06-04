#ifndef HAPTICFEEDBACK_ARDUINO_H
    #define HAPTICFEEDBACK_ARDUINO_H

#include "hapticFeedback.h"
#include "TeamAt_DRV2605.h"

class hapticFeedback_arduino: public hapticFeedback
{

public:

void init(bool mode);
bool calibrate(bool mode);


    void playWaveform(uint8_t waveformId);

    void playSequence(uint8_t* sequenceTable,uint8_t sequenceLength);

private:

TeamAt_DRV2605 haptic;
//uint8_t waveFormTable[WF_ENDLIST];





};

#endif // HAPTICFEED