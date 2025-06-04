/**
 * @file tonePlayer.cpp
 * @author Simon Latour
 * @brief   The tone player is used to creat a tone base on musical notes and play "songs" : lists of notes / delays
 * @version 1.0
 * @date 2020-11-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "tonePlayer.h"
#include "teamATbasic_V1_1.h"

void tonePlayerClass::handle(unsigned long currentMillis)
{

    bool NoteOnRequired = false;  

    if(m_playerOn)
    {
        if( timeElapsed(currentMillis,m_lastMillis) > m_song[m_currentNoteIndex][1] )
        {
            noteOff();

            m_currentNoteIndex ++;

            if(m_song[m_currentNoteIndex][0] != TONE_SONG_END_MARKER) // if song not finished
            {
               m_lastMillis = currentMillis;
               NoteOnRequired = true;
                
            }
            else // Song finished
            {
              stop();

            }
            
        }
    }

    if(m_startRequest)
    {
        m_startRequest = false;
        m_playerOn = true;
        NoteOnRequired = true;
        m_lastMillis = currentMillis;
    }


    if(m_song[m_currentNoteIndex][0] != TONE_SILENCE && NoteOnRequired == true)
    {
        #ifdef DEBUG_INFO_SERIAL 
            Serial.print(m_song[m_currentNoteIndex][0]); Serial.print(" "); Serial.print(m_song[m_currentNoteIndex][1]); Serial.print(" ");
        #endif
        noteOn(m_song[m_currentNoteIndex][0]);
    }

}



bool tonePlayerClass::isPlaying()
{
    return m_playerOn;
}
void tonePlayerClass::loadSong(unsigned int const (*song)[2])
{
    m_playerOn = false;
    m_song = song;
    m_currentNoteIndex = 0;
}

void tonePlayerClass::start(){
    
    m_startRequest = true;
    //digitalWrite(0, 0);

}

void tonePlayerClass::stop(){
    
    noteOff();
    m_currentNoteIndex = 0;
    m_playerOn = false;
    //digitalWrite(0, 1);

}