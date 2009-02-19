#include "title.h"
#include "game.h"
#include "basics.h"
#include "input.h"
#include "sprites.h"
#include "invader.h"
#include "menu.h"
#include "player.h"
#include "tower.h"
#include "bullet.h"
#include "level.h"
#include "sound.h"
#include "systemstub.h"

void DrawGameOver() {
	int i;
	float x,y;
	
	x=0;
	y=0;
	for(i=0;i<player.level-4;i++) {
    systemStub->drawSprite(SPR_INV1,x,y);
		x+=SPR_SIZE;
		if(x>SCR_WID-SPR_SIZE) {
			x=0;
			y+=SPR_SIZE;
		}
	}
}

byte PlayGameOver() {
  return !systemStub->_pi.leftClick;
}

void InitGameOver(void) {
}

void ExitGameOver(void) {
}

void GameOver() {
	InitGameOver();
	while(!systemStub->_pi.quit) {
    if(systemStub->_pi.leftClick)
			break;

    DrawGameOver();
    systemStub->update();
	}
	ExitGameOver();
}
