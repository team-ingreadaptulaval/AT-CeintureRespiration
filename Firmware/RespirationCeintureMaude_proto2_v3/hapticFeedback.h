#ifndef HAPTICFEEDBACK_H
    #define HAPTICFEEDBACK_H


#include <stdint.h>

// #define DRV_1000ms_alert 16
// #define DRV_750ms_alert 15 
// #define DRV_strong_buzz_alert 14
// #define DRV_pulsing_strong_1 52 

#define SEQ_WARNING_1 1
#define SEQ_WARNING_2 2

#define SEQUENCE_LEN_MAX 6

class hapticFeedback
{

    public:

    virtual void init(bool mode);

    virtual void playWaveform(uint8_t waveformId);
    virtual void playSequence(uint8_t* sequenceTable,uint8_t sequenceLength);

    private:


};

#endif // HAPTICFEEDz