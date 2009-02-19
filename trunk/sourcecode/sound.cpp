#include "sound.h"

//KSound *snd[NUM_SOUNDS];
byte doSounds;

void InitSound(void)
{
/*
int i;
	char sndName[NUM_SOUNDS][16]={"laser.wav","gun.wav","boom.wav","missile.wav","misboom.wav","select.wav","cancel.wav",
		"nomoney.wav","mine.wav","land.wav","place.wav","sell.wav","upgrade.wav","power.wav","acid.wav","truckshoot.wav",
		"research.wav","save.wav"};
	char s[32];

	for(i=0;i<NUM_SOUNDS;i++)
	{
		snd[i]=new KSound;
		sprintf(s,"snd\\%s",sndName[i]);
		snd[i]->loadSample(KMiscTools::makeFilePath(s));
	}
	doSounds=1;
  */
}

void ExitSound(void)
{
/*
int i;
	for(i=0;i<NUM_SOUNDS;i++)
		delete snd[i];
*/
}

void Sound(byte s)
{
/*
if(doSounds)
		snd[s]->playSample();
*/
}

void ToggleSound(void)
{
	doSounds=1-doSounds;
}