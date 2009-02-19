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
#include "gameover.h"
#include "systemstub.h"
#include <iostream>

void DrawGame() {
	RenderInvaders();
	RenderTowers();
	RenderBullets();
	RenderMenu();
}

void PlayGame() {
	NextLevel();
	UpdateInvaders();
	UpdatePlayer();
	UpdateBullets();
	UpdateTowers();
  if(systemStub->_pi.s) {
    systemStub->_pi.s=false;
		ToggleSound();
	}
}

void InitGame(void) {
	InitPlayer();
	ClearTowers();
	ClearBullets();
	ClearInvaders();
}

void ExitGame(void) {
}

void ShowScore() {
	int i;
	int16 x,y;
	
	x=0;
	y=0;
	for(i=0;i<player.level-4;i++) {
    systemStub->drawSprite(SPR_INV2B, x,y);
		x+=SPR_SIZE;
		if(x>SCR_WID-SPR_SIZE) {
			x=0;
			y+=SPR_SIZE;
		}
	}
}

void Game() {
  cout << "Game State" <<endl;
	InitGame();
  int pause=0;

	while(!systemStub->_pi.quit) {

		if(systemStub->_pi.escape) {
      systemStub->_pi.escape=false;
			if(pause==1)
				pause=2;
			if(pause==3)
				pause=0;
		} else {
			if(pause==0)
				pause=1;
			if(pause==2)
				pause=3;
		}

		if(pause==2 || pause==3)
			ShowScore();
		else {
			PlayGame();
			DrawGame();
			if(player.life==0) {
        break;
			}
		}
    systemStub->update();
	}
	ExitGame();

  GameOver();
}
