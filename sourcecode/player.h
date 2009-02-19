#ifndef PLAYER_H
#define PLAYER_H

#include "basics.h"

#define MAX_LIFE (20)
#define MAX_MONEY (240)

#define PM_PLAY		(0)
#define PM_PLACE	(1)
#define PM_UPGRADE	(2)

#define RS_GRAVITY	(1)
#define RS_TRIPLE	(2)
#define RS_FREEZE	(3)
#define RS_PIERCE	(4)
#define RS_CORRODE	(5)
#define RS_DISCHARGE (6)
#define MAX_RESEARCH (7)

#define RS_COST		(160)

typedef struct player_t
{
	byte gameSpeed;
	byte level;
	word money;
	byte life;
	byte mode;
	byte placingType;
	float placeX;
	byte oldBtn,oldRBtn;
	byte upTower;
	byte research[MAX_RESEARCH];
} player_t;

extern player_t player;

void InitPlayer(void);
//void UpdatePlayer(KWindow *win);
void UpdatePlayer();

#endif