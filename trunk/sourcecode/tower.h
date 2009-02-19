#ifndef TOWER_H
#define TOWER_H

#include "basics.h"

#define MAX_TOWERS	(32)

#define TWR_NONE	(0)
#define TWR_GUN		(1)
#define TWR_MINE	(2)
#define TWR_MISSILE (3)
#define TWR_CRYO	(4)
#define TWR_POWER	(5)
#define TWR_ACID	(6)
#define TWR_TYPES	(7)

#define TOWER_Y		(SCR_HEI-SPR_SIZE-4)

#define UG_DMG		(1)
#define UG_RELOAD	(2)
#define UG_DRILL	(3)
#define UG_RADIUS	(4)
#define UG_RANGE	(5)
#define UG_FREEZE	(6)
#define UG_TIME		(7)
#define UG_BUNKER	(8)

typedef struct tower_t
{
	float x;
	byte type;
	word reload;
	byte upg[10];
	byte upgCost;
	byte dmg;
	float range;
	word freeze;
	word maxReload;
	word charge;
	word chargeAmt;
} tower_t;

extern byte towerPrice[TWR_TYPES];
extern tower_t tower[MAX_TOWERS];

void ClearTowers(void);
void PlaceTower(float x,byte type);
byte RoomForYou(float x);
byte TowerPic(byte type);
void RenderTowers(void);
void UpdateTowers(void);
byte OnTower(void);
void SellTower(void);
void UpgradeTower(byte upType);
byte SaveLife(void);

#endif