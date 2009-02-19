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
#include <iostream>
using namespace std;

char sprName[NUM_SPRITES][16]={};


void DrawTitle() {
  systemStub->drawSprite(TIT_MOON,     0, 0);
  systemStub->drawSprite(TIT_INVADERS, SCR_WID-377, 0);
  systemStub->drawSprite(TIT_EXIT,     0, SCR_HEI-165);
  systemStub->drawSprite(TIT_BOOK,     SCR_WID-80,SCR_HEI-80);

//	title[0]->blitRect(0,0,251,246,0,0);
//	title[1]->blitRect(0,0,377,296,SCR_WID-377,0);
//	title[2]->blitRect(0,0,162,165,0,SCR_HEI-165);
//	title[3]->blitRect(0,0,80,80,SCR_WID-80,SCR_HEI-80);
}

byte PlayTitle() {
	if(systemStub->_pi.leftClick) {
    systemStub->_pi.leftClick=false;
    if(MouseX()<164 && MouseY()>308) {
			return 0;	// clicked on exit
		}
		if(MouseY()<308) {
			Game();
		}
	}
	return 1;	
}

void InitTitle(void) {
}

void ExitTitle(void) {
}

void Title() {
  cout << "Title State" <<endl;
	InitTitle();
	while(!systemStub->_pi.quit) {
		if(!PlayTitle())
			break;
		DrawTitle();
    systemStub->update();
	}
	ExitTitle();
}
