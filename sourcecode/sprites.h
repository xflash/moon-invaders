#ifndef SPRITES_H
#define SPRITES_H

#include "basics.h"


#define SPR_INV1	(0)
#define SPR_INV1B	(1)
#define SPR_GUNTOWER (2)
#define SPR_LIFE	(3)
#define SPR_DAMAGE	(4)
#define SPR_RELOAD	(5)
#define SPR_SELL	(6)
#define SPR_MINE	(7)
#define SPR_DRILL	(8)
#define SPR_TANK	(9)
#define SPR_LAUNCHER (10)
#define SPR_RANGE	(11)
#define SPR_RADIUS	(12)
#define SPR_FREEZE	(13)
#define SPR_POWER	(14)
#define SPR_TIME	(15)
#define SPR_INV2	(16)
#define SPR_INV2B	(17)
#define SPR_TRIPLE	(18)
#define SPR_GRAVITY	(19)
#define SPR_FREEZERSCH (20)
#define SPR_PIERCE	(21)
#define SPR_ACID	(22)
#define SPR_ACIDUP	(23)
#define SPR_INV3	(24)
#define SPR_INV3B	(25)
#define SPR_INV4	(26)
#define SPR_INV4B	(27)
#define SPR_BUNKER	(28)
#define SPR_DISCHARGE (29)
#define TIT_MOON (30)
#define TIT_INVADERS (31)
#define TIT_EXIT (32)
#define TIT_BOOK (33)

#define NUM_SPRITES (34)

void InitSprites(void);
void ExitSprites(void);
	
/*
void DrawSprite(byte s,float x,float y);
void DrawSpriteFade(byte s,float x,float y,float blend);
void DrawSpriteColored(byte s,float x,float y,float r,float g,float b,float blend);
void DrawRect(float x,float y,float x2,float y2,float r,float g,float b,float blend);
void DrawLine(float x,float y,float x2,float y2,float r,float g,float b,float blend);
*/

#endif