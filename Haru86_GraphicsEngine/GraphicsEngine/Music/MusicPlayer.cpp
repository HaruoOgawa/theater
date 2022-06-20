#include "MusicPlayer.h"
//#include <thread>
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "GraphicsEngine/Message/Console.h"

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>

// some song information
#include "Synth.h"
#define INCLUDE_NODES
#include "64k2Patch.h"
#include "64k2Song.h"

#define SAMPLE_RATE 44100
#define SAMPLE_TYPE float
SAMPLE_TYPE lpSoundBuffer[MAX_SAMPLES * 2 + 44100 * 60]; // add safety buffer for 60s 
HWAVEOUT hWaveOut;

/////////////////////////////////////////////////////////////////////////////////
// initialized data
/////////////////////////////////////////////////////////////////////////////////

WAVEFORMATEX WaveFMT =
{
	WAVE_FORMAT_IEEE_FLOAT,
	2, // channels
	SAMPLE_RATE, // samples per sec
	SAMPLE_RATE * sizeof(SAMPLE_TYPE) * 2, // bytes per sec
	sizeof(SAMPLE_TYPE) * 2, // block alignment;
	sizeof(SAMPLE_TYPE) * 8, // bits per sample
	0 // extension not needed
};

WAVEHDR WaveHDR =
{
	(LPSTR)lpSoundBuffer,
	MAX_SAMPLES * sizeof(SAMPLE_TYPE) * 2,			// MAX_SAMPLES*sizeof(float)*2(stereo)
	0,
	0,
	0,
	0,
	0,
	0
};

MMTIME MMTime =
{
	TIME_SAMPLES,
	0
};

/////////////////////////////////////////////////////////////////////////////////
// crt emulation
/////////////////////////////////////////////////////////////////////////////////

extern "C"
{
	int _fltused = 1;
}

/////////////////////////////////////////////////////////////////////////////////
// entry point for the executable if msvcrt is not used
/////////////////////////////////////////////////////////////////////////////////


MusicPlayer::MusicPlayer() 
{

}

MusicPlayer::~MusicPlayer() 
{

}

bool MusicPlayer::Initialize() {
	Console::Log("Start 64klang\n");
	// init synth and start filling the buffer 
	_64klang_Init(SynthStream, SynthNodes, SynthMonoConstantOffset, SynthStereoConstantOffset, SynthMaxOffset);
#if 1
	Console::Log("Sleep...\n");
	// threaded buffer fill with 5 seconds heads up
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)_64klang_Render, lpSoundBuffer, 0, 0);
	Sleep(5000);
	Console::Log("Wake Up!!!\n");
#else
	// sequential (blocking) buffer fill aka precalc
	_64klang_Render(lpSoundBuffer);
#endif	

	// start audio playback
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &WaveFMT, NULL, 0, CALLBACK_NULL);
	waveOutPrepareHeader(hWaveOut, &WaveHDR, sizeof(WaveHDR));
	waveOutWrite(hWaveOut, &WaveHDR, sizeof(WaveHDR));
	
	Console::Log("Loaded 64klang\n");

	return true;
}

bool MusicPlayer::Update() {

	if ((MMTime.u.sample < MAX_SAMPLES) || GraphicsMain::GetInstance()->GetIsRunning()) {
		//Console::Log("Sound Loop...... %d / %d\n", MMTime, hWaveOut);
		waveOutGetPosition(hWaveOut, &MMTime, sizeof(MMTIME));
		//Sleep(128);
	}

	return true;
}