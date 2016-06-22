//================================================================================================
//
// Brian Spencer's Hacker (Game Engine 2)
//
// Desc : Music playlist management
//
// Copyright (c) Robert Muresan, exoSyphen Studios (http://www.exosyphen.com)
//================================================================================================
#include "bshAudioPlaylist.h"
#include "heLog.h"
#include "HackerEvolutionDefines.h"
#include <stdio.h>
#include <string.h>

bshAudioPlaylist gameMusic;
extern long      gVolume = 0;

/***********************************************************************************************************/
/* Class constructor */
bshAudioPlaylist::bshAudioPlaylist()
{
    m_trackCount   =  0;
    m_currentTrack = -1;
}
/***********************************************************************************************************/
/* Class destructor */
bshAudioPlaylist::~bshAudioPlaylist()
{
    gApplicationLog.printf("\nbshAudioPlaylist::~bshAudioPlaylist - Shutdown");
}
/***********************************************************************************************************/
/* Load playlist   */
void bshAudioPlaylist::loadPlayList(char* szPlaylistFileName)
{
    FILE*    plFile;
    int      i;
    szString temp;
	char     filePath[1024];

    gApplicationLog.printf("\nbshAudioPlaylist::loadPlayList() - Loading playlist");
    plFile = fopen(szPlaylistFileName,"rt");        
        fscanf(plFile,"%d",&m_trackCount);
        fscanf(plFile,"\n");
        for(i = 0; i < m_trackCount; i++)
        {
            fgets(temp,1024,plFile);
            //if (i < m_trackCount - 1) temp[strlen(temp) - 1] = 0;

            stripNewLine(temp);
            strcpy(m_trackList[i],temp);
            gApplicationLog.printf("\nbshAudioPlaylist::loadPlayList() - (%d/%d) (%s)",i + 1,m_trackCount,temp);

			// Load audio track
			sprintf(filePath,"heuntold-music/%s",m_trackList[i]);
			m_pTrack[i] = nullptr;
			/*
			m_pTrack[i] = Mix_LoadMUS(filePath);
			if(m_pTrack[i] == nullptr)
				gApplicationLog.printf("\nbshAudioPlaylist::loadPlayList() - ERROR(%s)",Mix_GetError());
				*/
        }
    fclose(plFile);

    m_currentTrack = 0;
	gApplicationLog.printf("\nbshAudioPlaylist::loadPlayList() - DONE!");
	
	SetVolume(gSoundVolume);
}
/***********************************************************************************************************/
/* Start playing the playlist */
void bshAudioPlaylist::Play()
{
	//gApplicationLog.printf("\nbshAudioPlaylist::Play(%s/%d)",m_trackList[m_currentTrack],m_currentTrack);

    // Start/Resume playing the playlist
    if(m_trackCount <= 0) return;
	if(m_currentTrack < 0) return;
	//if(m_pTrack[m_currentTrack] == nullptr) return;

	if(m_pTrack[m_currentTrack] != nullptr)
		Mix_FreeMusic(m_pTrack[m_currentTrack]);

	char     filePath[1024];
	sprintf(filePath,"heuntold-music/%s",m_trackList[m_currentTrack]);
	m_pTrack[m_currentTrack] = Mix_LoadMUS(filePath);

	Mix_PlayMusic(m_pTrack[m_currentTrack],0);
 
	//gApplicationLog.printf("\nbshAudioPlaylist::Play() - Done.");
}
/***********************************************************************************************************/
/* Stop playing the playlist */
void bshAudioPlaylist::Stop()
{
	Mix_HaltMusic();
}
//==================================================================================================
// Skin to the next track
void bshAudioPlaylist::nextTrack()
{
	if(m_trackCount == 0)
		return;

	gApplicationLog.printf("\nbshAudioPlaylist::nextTrack() %d -> ",m_currentTrack);
	m_currentTrack++;
	//m_currentTrack %= m_trackCount;
	if(m_currentTrack == m_trackCount) m_currentTrack = 0;
	gApplicationLog.printf("%d",m_currentTrack);

	Stop();
	Play();
	gApplicationLog.printf("/ moved to next track");
}

/***********************************************************************************************************/
/* Update playlist */
void bshAudioPlaylist::Update()
{
    if(Mix_PlayingMusic() == 0) nextTrack();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// * Retrieve track name
char* bshAudioPlaylist::getTrackName()
{
	strcpy(mTrackName,m_trackList[m_currentTrack]);
	return &mTrackName[0];
}

//===========================================================================================================
void bshAudioPlaylist::Pause()
{
	Mix_PauseMusic();
}

//===========================================================================================================
void bshAudioPlaylist::AdjustVolume(int pDirection)
{
	gSoundVolume += pDirection * 5;

	if(gSoundVolume < 0)   gSoundVolume = 0;
	if(gSoundVolume > 100) gSoundVolume = 100;

	SetVolume(gSoundVolume);
}

//===========================================================================================================
void bshAudioPlaylist::SetVolume(int volume)
{
	Mix_VolumeMusic(volume);
}
