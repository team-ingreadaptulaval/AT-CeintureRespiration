/**
 * @file tonePlayer.h
 * @author Simon Latour
 * @brief   The tone player is used to creat a tone base on musical notes and play "songs" : lists of notes / delays
 * @version 1.0
 * @date 2020-11-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef TONEPLAYER_H
    #define TONEPLAYER_H

#define TONE_SONG_END_MARKER  0
#define TONE_SILENCE  1
#define TONE_A4     440<<4//13636
#define TONE_AS4    466<<4//12871
#define TONE_B4     493<<4//12148
#define TONE_C5     523<<4//11466
#define TONE_CS5    554<<4//10823
#define TONE_D5     587<<4//10215
#define TONE_DS5    622<<4//9642
#define TONE_E5     659<<4//9101
#define TONE_F5     698<<4// 8590
#define TONE_FS5    739<<4//8108
#define TONE_G5     784<<4//7653
#define TONE_GS5   830<<4// 7223

#define TONE_A3     (TONE_A4 >> 1)
#define TONE_AS3    (TONE_AS4>> 1)
#define TONE_B3     (TONE_B4 >> 1)
#define TONE_C4     (TONE_C5 >> 1)
#define TONE_CS4    (TONE_CS5>> 1)
#define TONE_D4     (TONE_D5 >> 1)
#define TONE_DS4    (TONE_DS5>> 1)
#define TONE_E4     (TONE_E5 >> 1)
#define TONE_F4     (TONE_F5 >> 1)
#define TONE_FS4    (TONE_FS5>> 1)
#define TONE_G4     (TONE_G5 >> 1)
#define TONE_GS4    (TONE_GS5>> 1)

#define BPM 120 // BPM of the Songe / system

//Notes length whe X is the BPM of the song
#define DUR_QUARTER(x)     (60000/x)
#define DUR_HALF(x)        ((60000/x)* 2)
#define DUR_WHOLE(x)       ((60000/x)* 4)
#define DUR_8TH(x)         ((60000/x) / 2)
#define DUR_16TH(x)        ((60000/x) / 4)
#define DUR_32TH(x)        ((60000/x) / 8)



const unsigned int song_up[][2] = {
                              {TONE_SILENCE,10},// Amplifier not turning on quickly enough so we start with a silence to let time to power up.
                              {TONE_A3,DUR_32TH(120)},
                              {TONE_B3,DUR_32TH(120)},
                              {TONE_C4,DUR_32TH(120)},
                              {TONE_SONG_END_MARKER,TONE_SONG_END_MARKER},
                            };

const unsigned int song_reminder[][2] = {
    //{TONE_SILENCE,1000},// Amplifier not turning on quickly enough so we start with a silence to let time to power up.
    {TONE_F5,DUR_32TH(120)},
    {TONE_SILENCE,DUR_32TH(120)},
    {TONE_F5,DUR_32TH(120)},
    {TONE_SILENCE,800},// Amplifier not turning on quickly enough so we start with a silence to let time to power up.
    {TONE_F5,DUR_32TH(120)},
    {TONE_SILENCE,DUR_32TH(120)},
    {TONE_F5,DUR_32TH(120)},
    {TONE_SILENCE,800},// Amplifier not turning on quickly enough so we start with a silence to let time to power up.
    {TONE_F5,DUR_32TH(120)},
    {TONE_SILENCE,DUR_32TH(120)},
    {TONE_F5,DUR_32TH(120)},
    {TONE_SONG_END_MARKER,TONE_SONG_END_MARKER},
};

const unsigned int song_reminderShort[][2] = {
    //{TONE_SILENCE,1000},// Amplifier not turning on quickly enough so we start with a silence to let time to power up.
    {TONE_F5,DUR_32TH(120)},
    {TONE_SILENCE,DUR_32TH(120)},
    {TONE_F5,DUR_32TH(120)},
    {TONE_SONG_END_MARKER,TONE_SONG_END_MARKER},
};

const unsigned int song_beep[][2] = {
    //{TONE_SILENCE,1000},// Amplifier not turning on quickly enough so we start with a silence to let time to power up.
    {TONE_F5,DUR_32TH(200)},
    {TONE_SONG_END_MARKER,TONE_SONG_END_MARKER},
};

class tonePlayerClass
{
    bool m_playerOn = false;
    bool m_startRequest = false;
    unsigned int m_currentNoteIndex = 0;

    unsigned long m_lastMillis = 0;
    unsigned int const (*m_song)[2];

    public:

    void handle(unsigned long currentMillis);
    bool isPlaying();
    void loadSong(unsigned int const (*song)[2]);
    void start();
    void stop();

    
};

/**
 * @brief This function must be implemented by user. When this function is called, a tone shoudl be emmitted
 * 
 * @param note the comparator value
 */
extern void noteOn(unsigned int note);

/**
 * @brief This function must be implemented by user. 
 */
extern void noteOff();



#endif