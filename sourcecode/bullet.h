#ifndef BULLET_H
#define BULLET_H

#include "basics.h"
#include "tower.h"

#define MAX_BULLETS	(400)

#define BL_NONE	(0)
#define BL_GUN	(1)	// from gun tower
#define BL_MISSILE (2) 
#define BL_LASER (3)
#define BL_MISBOOM	(4)
#define BL_FREEZER (5)
#define BL_ACID		(6)
#define BL_ACIDBOOM (7)
#define BL_DISCHARGE (8)

typedef struct bullet_t
{
	float x,y;
	float dx,dy;
	byte type;
	byte dmg;
	word acid;
	float radius;
} bullet_t;

void ClearBullets(void);
void FireBullet(tower_t *who,word tgt);
void UpdateBullets(void);
void RenderBullets(void);
int AddBullet(byte type,float x,float y,float dx,float dy,byte dmg,float radius);

#endif