#ifndef INVADER_H
#define INVADER_H

#include "basics.h"

#define MAX_INVADERS (256)

typedef struct invader_t
{
	byte anim;
	word acid;
	float x,y;
	int freeze;
	int life,maxLife;
} invader_t;

extern invader_t inv[MAX_INVADERS];

void ClearInvaders(void);
void InvaderWave(byte num,int life,float spd,float spacing);
void RenderInvaders(void);
void UpdateInvaders(void);
byte HitInvaders(float x,float y,byte dmg);
byte NoInvaders(void);
word FindTarget(float x,float range,float speed);
byte PullMissile(float x,float y,float *dx,float *dy);
void BlowUpMissile(float x,float y,float radius,word damage);
void InvaderPos(word w,float *x,float *y);
void LaserGuys(float x,float y,float x2,float y2,word dmg,word frz);
byte HitInvadersFrz(float x,float y,byte dmg,word frz);
byte HitInvadersAcid(float x,float y,byte dmg,byte acid,float radius);

#endif