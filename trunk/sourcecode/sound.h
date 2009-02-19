#ifndef SOUND_H
#define SOUND_H

#include "basics.h"

#define SND_LASER	(0)
#define SND_GUN		(1)
#define SND_BOOM	(2)
#define SND_MISSILE	(3)
#define SND_MISBOOM	(4)
#define SND_SELECT	(5)
#define SND_CANCEL	(6)
#define SND_NOMONEY	(7)
#define SND_MINE	(8)
#define SND_LAND	(9)
#define SND_PLACE	(10)
#define SND_SELL	(11)
#define SND_UPGRADE (12)
#define SND_POWER	(13)
#define SND_ACID	(14)
#define SND_TRUCKSHOOT (15)
#define SND_RESEARCH	(16)
#define SND_SAVE	(17)
#define NUM_SOUNDS	(18)

void InitSound(void);
void ExitSound(void);
void Sound(byte s);
void ToggleSound(void);

#endif